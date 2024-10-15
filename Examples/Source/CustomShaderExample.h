#pragma once

#include <luisa/luisa-compute.h>
#include "Game/World.h"
#include "Materials/Material.h"
#include "Render/GPUSceneInterface.h"
#include "Render/material/disney_material.h"

using luisa::compute::Float3;
class my_shader : public Rendering::disney_material
{

    [[nodiscard]] virtual Float3 sample_base_color(const Rendering::bxdf_context& context) const
    {
        auto bary = context.intersection.barycentric;
        return {1.f - bary.x - bary.y, bary.x, bary.y};
    }
};

class reverse_color_shader : public Rendering::disney_material
{
	virtual Float3 evaluate(const Rendering::material_parameters &material_data, const Float3 &w_o, const Float3 &w_i) const override
	{
	    auto color = Rendering::disney_material::evaluate(material_data, w_o, w_i);
	    return luisa::make_float3(1.f) - color;
	}
};

inline auto CustomShaderExample()
{
    return [](World& world)
    {
        auto Mesh1 = world.SpawnActor<StaticMeshActor>("Mesh1", BasicShapesLibrary::GenerateCylinder(1., 0.5));

        auto Material = Mesh1->GetStaticMeshComponent()->GetMeshData()->GetMaterial();
        Material->BindShader<my_shader>();


        auto Mesh2 = world.SpawnActor<StaticMeshActor>("Mesh2", BasicShapesLibrary::GenerateCapsule(1., 0.5));
        Mesh2->SetTranslation({0, 1., 0});
        auto Material2 = Mesh2->GetStaticMeshComponent()->GetMeshData()->GetMaterial();
        Material2->BindShader<reverse_color_shader>();



        // Remember to recompile the shader after binding
        world.GetScene()->CompileShader();
    };
}