//
// Created by MarvelLi on 2024/3/27.
//

#include "RayTracingScene.h"
#include <luisa/runtime/rtx/accel.h>
#include "Render/Core/ray_tracing_hit.h"
#include "Render/Core/VertexData.h"
#include "Render/ViewportInterface.h"
#include "Render/Core/bxdf_context.h"
#include "Render/Core/math_function.h"
#include "Render/material/shading_function.h"
#include "Render/SceneProxy/StaticMeshSceneProxy.h"
#include "Render/SceneProxy/TransformProxy.h"
#include "Render/SceneProxy/CameraSceneProxy.h"
#include "Render/SceneProxy/LightSceneProxy.h"
#include "Render/SceneProxy/MaterialSceneProxy.h"
#include "Render/SceneProxy/LineSceneProxy.h"

namespace MechEngine::Rendering
{

RayTracingScene::RayTracingScene(Stream& stream, Device& device, ImGuiWindow* InWindow, ViewportInterface* InViewport) noexcept:
GPUSceneInterface(stream, device), Window(InWindow)
{
	CameraProxy = luisa::make_unique<CameraSceneProxy>(*this);
	LightProxy = luisa::make_unique<LightSceneProxy>(*this);
	TransformProxy = luisa::make_unique<TransformSceneProxy>(*this);
	StaticMeshProxy = luisa::make_unique<StaticMeshSceneProxy>(*this);
	MaterialProxy = luisa::make_unique<MaterialSceneProxy>(*this);
	LineProxy = luisa::make_unique<LineSceneProxy>(*this);
	auto size = Window->framebuffer().size();
	g_buffer.base_color = device.create_image<float>(PixelStorage::BYTE4,
		Window->framebuffer().size().x, Window->framebuffer().size().y);
	g_buffer.normal = device.create_image<float>(PixelStorage::BYTE4,
		Window->framebuffer().size().x, Window->framebuffer().size().y);
	g_buffer.depth = device.create_buffer<float>(size.x * size.y);
	g_buffer.instance_id = device.create_image<uint>(PixelStorage::INT1,
		Window->framebuffer().size().x, Window->framebuffer().size().y);
	g_buffer.material_id = device.create_image<uint>(PixelStorage::INT1,
		Window->framebuffer().size().x, Window->framebuffer().size().y);
	g_buffer.frame_buffer = &Window->framebuffer();
	LOG_INFO("Init render frame buffer: {} {}",Window->framebuffer().size().x, Window->framebuffer().size().y);
	Viewport = InViewport;

	RayTracingScene::CompileShader();
}

void RayTracingScene::UploadRenderData()
{
	auto UpdateBindlessArrayIfDirty = [&]() {
		if(bindlessArray.dirty())
		{
			stream << bindlessArray.update();
		}
	};

	// Make sure static mesh data is uploaded before transform data
	// Because need to allocate instance id from accel
	StaticMeshProxy->UploadDirtyData(stream);
	UpdateBindlessArrayIfDirty();

	CameraProxy->UploadDirtyData(stream);
	UpdateBindlessArrayIfDirty();

	MaterialProxy->UploadDirtyData(stream);
	UpdateBindlessArrayIfDirty();

	LightProxy->UploadDirtyData(stream);
	UpdateBindlessArrayIfDirty();

	// This may also update transform in accel
	TransformProxy->UploadDirtyData(stream);
	UpdateBindlessArrayIfDirty();

	LineProxy->UploadDirtyData(stream);
	UpdateBindlessArrayIfDirty();

	if (rtAccel.dirty()) stream << rtAccel.build() << synchronize();
}

void RayTracingScene::Render()
{
	stream << (*MainShader)(CameraProxy->GetCurrentViewData(), LightProxy->LightCount()).dispatch(GetWindosSize());

	if(ViewMode != ViewMode::FrameBuffer)
		stream << (*ViewModePass)(static_cast<uint>(ViewMode)).dispatch(GetWindosSize());

	LineProxy->PostRenderPass(stream);

	stream << synchronize();
}

ImageView<float> RayTracingScene::frame_buffer() noexcept
{
	return Window->framebuffer();
}

uint2 RayTracingScene::GetWindosSize() const noexcept
{
	return Window->framebuffer().size();
}

void RayTracingScene::CompileShader()
{
	// Compile base shaders
	GPUSceneInterface::CompileShader();

	// Main pass shader
	MainShader = luisa::make_unique<Shader2D<view_data, uint>>(device.compile<2>(
		[&](Var<view_data> view, UInt LightCount) noexcept {
			// Calc view space cordination, left bottom is (-1, -1), right top is (1, 1). Forwards is +Z
			auto pixel_coord = dispatch_id().xy();
			auto pixel = make_float2(pixel_coord) + .5f;

			// Fill with default
			g_buffer.set_default(pixel_coord, make_float4(1.f));
			frame_buffer()->write(pixel_coord, make_float4(1.f));

			// Ray trace rasterization
			auto ray = view->generate_ray(pixel_coord);
			auto intersection = intersect(ray, view);
			intersection.pixel_coord = pixel_coord; // Ugly, to do something
			Float transmission = 1.f;
			Float3 pixel_color = make_float3(0.f);
			$while(intersection.valid())
			{
				auto material_data = MaterialProxy->get_material_data(intersection.material_id);
				// Draw wireframe pass
				//@see https://developer.download.nvidia.com/whitepapers/2007/SDK10/SolidWireframe.pdf
				//@see https://www2.imm.dtu.dk/pubdb/edoc/imm4884.pdf
				$if(material_data->show_wireframe == 1)
				{
					auto TestWireFrame = IsWireFrame(view, pixel,
						intersection.vertex_ndc[0],
						intersection.vertex_ndc[1],
						intersection.vertex_ndc[2], 0.8f);
					$if(TestWireFrame)
					{
						frame_buffer()->write(pixel_coord, make_float4(0.f)); // Write line color as black
						transmission = 0.f;
						$break;
					};
				};

				/************************************************************************
				 *								Shading
				 ************************************************************************/
				auto x = intersection.position_world;
				auto w_o = normalize(-ray->direction());

				// Rendering equation : L_o(x, w_0) = L_e(x, w_0) + \int_{\Omega} bxdf(x, w_i, w_0) L_i(x, w_i) (n \cdot w_i) dw_i
				bxdf_context context{
					.ray = ray,
					.intersection = intersection,
					.w_o = w_o,
					.material_data = MaterialProxy->get_material_data(intersection.material_id)
				};

				material_parameters bxdf_parameters;
				MaterialProxy->material_virtual_call.dispatch(
					material_data.material_type, [&](const material_base* material) {
						bxdf_parameters = material->calc_material_parameters(context);
					});
				g_buffer.write(pixel_coord, bxdf_parameters, intersection);

				auto normal = bxdf_parameters.normal;
				auto reflect_dir = reflect(-w_o, normal);

				Float3 pixel_radiance = make_float3(0.f);
				$for(light_id, LightCount)
				{
					// First calculate light color, as rendering equation is L_i(x, w_i)
					auto light_data = LightProxy->get_light_data(light_id);
					auto light_transform = TransformProxy->get_transform_data(light_data.transform_id);
					auto light_dir = normalize(light_transform->get_location() - x);

					auto calc_lighting = [&](const Float3& w_i) {
						Float3 light_color = make_float3(0.f);
						Float3 mesh_color = make_float3(0.f);
						$if(dot(w_i, normal) >= 0.f)
						{
							context.w_i = w_i;
							// Dispatch light evaluate polymorphically, so that we can have different light type
							LightProxy->light_virtual_call.dispatch(
								light_data.light_type, [&](const light_base* light) {
									light_color = light->l_i(light_data, light_transform.transformMatrix, x, w_i);
								});
							MaterialProxy->material_virtual_call.dispatch(
								material_data.material_type, [&](const material_base* material) {
									mesh_color = material->bxdf(context, bxdf_parameters);
								});
						};
						return mesh_color * light_color * max(dot(w_i, normal), 0.001f);
					};

					// Only two sample, one is light dir, one is reflected dir
					pixel_radiance += calc_lighting(light_dir) * 0.95f + calc_lighting(reflect_dir) * 0.05f;
				};
				auto alpha = material_data.alpha;
				pixel_color += pixel_radiance * alpha * transmission;
				transmission *= 1.f - alpha;
				$if(alpha != 1.f & transmission > 1e-2f)
				{
					ray->set_origin(offset_ray_origin(x, intersection.triangle_normal_world, ray->direction()));
					intersection = intersect(ray, view);
					$continue;
				};
				$break;
			};
			frame_buffer()->write(pixel_coord, make_float4(linear_to_srgb(pixel_color + transmission * make_float3(1.f)), 1.f));
		}));

}
}
