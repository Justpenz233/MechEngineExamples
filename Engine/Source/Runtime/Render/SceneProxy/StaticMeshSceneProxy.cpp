//
// Created by MarvelLi on 2024/4/4.
//

#include "StaticMeshSceneProxy.h"

#include "MaterialSceneProxy.h"
#include "TransformProxy.h"
#include "Components/StaticMeshComponent.h"
#include "Render/RayTracing/RayTracingScene.h"

namespace MechEngine::Rendering
{

StaticMeshSceneProxy::StaticMeshSceneProxy(RayTracingScene& InScene)
	: SceneProxy(InScene)
{
	StaticMeshDatas.resize(instance_max_number);
	StaticMeshResource.resize(instance_max_number);
	std::tie(data_buffer, data_buffer_id) = Scene.RegisterBindlessBuffer<static_mesh_data>(instance_max_number);
}
bool StaticMeshSceneProxy::IsDirty()
{
	return !NewMeshes.empty() || !DirtyMeshes.empty() || !DirtyGeometryMeshes.empty();
}

void StaticMeshSceneProxy::AddStaticMesh(StaticMeshComponent* InMesh, uint InTransformID)
{
	ASSERTMSG(!MeshIndexMap.count(InMesh), "StaticMeshComponent already exist in scene!");
	if(!Scene.GetTransformProxy()->IsExist(InTransformID))
	{
		LOG_ERROR("Transform Actorname: {} not exist in scene!", InMesh->GetOwnerName());
		return;
	}
	TransformMeshMap[InTransformID] = InMesh;
	MeshIndexMap[InMesh] = ~0u; // Mark a temporary index, for the update call at the same frame
	NewMeshes.insert(InMesh);
}

void StaticMeshSceneProxy::UpdateStaticMesh(StaticMeshComponent* InMesh)
{
	ASSERTMSG(MeshIndexMap.count(InMesh), "StaticMeshComponent not exist in scene!");
	DirtyMeshes.insert(InMesh);
}

void StaticMeshSceneProxy::UpdateStaticMeshGeometry(StaticMeshComponent* InMesh)
{
	ASSERTMSG(MeshIndexMap.count(InMesh), "StaticMeshComponent not exist in scene!");
	if(NewMeshes.count(InMesh)) return;
	DirtyGeometryMeshes.insert(InMesh);
}

void StaticMeshSceneProxy::UploadDirtyData(Stream& stream)
{
	bFrameUpdated = false;
	if (!IsDirty()) return;
	auto GetFlattenMeshData = [&](StaticMesh* MeshData)
	{
		int VertexNum = MeshData->GetVertexNum();
		int TriangleNum = MeshData->GetFaceNum();
		vector<Vertex> Vertices(VertexNum);
		vector<Triangle> Triangles(TriangleNum);
		vector<float3> CornerNormals(TriangleNum * 3);
		for (int i = 0; i < VertexNum; i++)
		{
			auto VertexData = MeshData->verM.row(i);
			Vertices[i].px = static_cast<float>(VertexData[0]);
			Vertices[i].py = static_cast<float>(VertexData[1]);
			Vertices[i].pz = static_cast<float>(VertexData[2]);
			auto VertexNormal = MeshData->VertexNormal.row(i);
			Vertices[i].nx = static_cast<float>(VertexNormal[0]);
			Vertices[i].ny = static_cast<float>(VertexNormal[1]);
			Vertices[i].nz = static_cast<float>(VertexNormal[2]);
		}
		for (int i = 0; i < TriangleNum; i++)
		{
			auto FaceData = MeshData->triM.row(i);
			Triangles[i].i0 = FaceData[0];
			Triangles[i].i1 = FaceData[1];
			Triangles[i].i2 = FaceData[2];
		}
		ASSERTMSG(MeshData->CornerNormal.rows() == TriangleNum * 3, "Corner normal size not match! {} != {}",
			MeshData->CornerNormal.rows(), TriangleNum * 3);
		for (int i = 0; i < TriangleNum * 3; i ++)
		{
			CornerNormals[i].x = MeshData->CornerNormal.row(i).x();
			CornerNormals[i].y = MeshData->CornerNormal.row(i).y();
			CornerNormals[i].z = MeshData->CornerNormal.row(i).z();
		}
		return std::tuple{Vertices, Triangles, CornerNormals};
	};
	// Upload new mesh data
	for (auto MeshComponent: NewMeshes)
	{
		auto MeshData = MeshComponent->MeshData;
		if(!MeshData || MeshData->IsEmpty())
		{
			LOG_WARNING("Add an empty mesh to scene: {}", MeshComponent->GetOwnerName());
			continue;
		}
		bFrameUpdated = true;
		auto [Vertices, Triangles, CornerNormals] = GetFlattenMeshData(MeshData.get());
		auto VBuffer = Scene.create<Buffer<Vertex>>(Vertices.size());
		auto TBuffer = Scene.create<Buffer<Triangle>>(Triangles.size());
		auto CornerlNormalBuffer = Scene.create<Buffer<float3>>(CornerNormals.size());
		auto AccelMesh = Scene.create<Mesh>(*VBuffer, *TBuffer, AccelOption{});

		stream << VBuffer->copy_from(Vertices.data())
		<< TBuffer->copy_from(Triangles.data())
		<< CornerlNormalBuffer->copy_from(CornerNormals.data())
		<< commit()
		<< AccelMesh->build();

		auto VBindlessid = Scene.RegisterBindless(VBuffer->view());
		auto TBindlessid = Scene.RegisterBindless(TBuffer->view());
		auto CNBindlessid = Scene.RegisterBindless(CornerlNormalBuffer->view());
		auto MaterialID = Scene.GetMaterialProxy()->AddMaterial(MeshData->GetMaterial());

		accel.emplace_back(*AccelMesh);
		auto Id = accel.size() - 1;
		MeshIndexMap[MeshComponent] = Id;
		StaticMeshDatas[Id].vertex_id = VBindlessid;
		StaticMeshDatas[Id].triangle_id = TBindlessid;
		StaticMeshDatas[Id].material_id = MaterialID;
		StaticMeshDatas[Id].corner_normal_id = CNBindlessid;
		StaticMeshResource[Id] = {AccelMesh, VBuffer, TBuffer, CornerlNormalBuffer};
	}
	if(bFrameUpdated)
	{
		ASSERTMSG(accel.size() <= instance_max_number, "Too many static mesh in scene!");
		stream << data_buffer.subview(0, StaticMeshDatas.size()).copy_from(StaticMeshDatas.data());
	}
	NewMeshes.clear();

	// Update mesh geometry
	for (auto MeshComponent : DirtyGeometryMeshes)
	{
		auto MeshData = MeshComponent->MeshData;
		if(!MeshData || MeshData->IsEmpty())
			continue;
		bFrameUpdated = true;
		auto Id = MeshIndexMap[MeshComponent];
		auto [Vertices, Triangles, CornerNormals] = GetFlattenMeshData(MeshData.get());

		// Register and upload new data buffer
		auto VBuffer = Scene.create<Buffer<Vertex>>(Vertices.size());
		auto TBuffer = Scene.create<Buffer<Triangle>>(Triangles.size());
		auto CornerlNormalBuffer = Scene.create<Buffer<float3>>(CornerNormals.size());
		auto AccelMesh = Scene.create<Mesh>(*VBuffer, *TBuffer, AccelOption{});
		stream << VBuffer->copy_from(Vertices.data())
		<< TBuffer->copy_from(Triangles.data())
		<< CornerlNormalBuffer->copy_from(CornerNormals.data())
		<< commit()
		<< AccelMesh->build();

		auto VBindlessid = StaticMeshDatas[Id].vertex_id;
		auto TBindlessid = StaticMeshDatas[Id].triangle_id;
		auto CNBindlessid = StaticMeshDatas[Id].corner_normal_id;
		bindlessArray.emplace_on_update(VBindlessid, VBuffer->view());
		bindlessArray.emplace_on_update(TBindlessid, TBuffer->view());
		bindlessArray.emplace_on_update(CNBindlessid, CornerlNormalBuffer->view());
		accel.set_mesh(Id, *AccelMesh);

		// Remove old data buffer
		auto PreVBuffer = StaticMeshResource[MeshIndexMap[MeshComponent]].VertexBuffer;
		auto PreTBuffer = StaticMeshResource[MeshIndexMap[MeshComponent]].TriangleBuffer;
		auto PreCNBuffer = StaticMeshResource[MeshIndexMap[MeshComponent]].CornerNormalBuffer;
		auto PreMesh = StaticMeshResource[MeshIndexMap[MeshComponent]].AccelMesh;
		Scene.destroy(PreVBuffer); Scene.destroy(PreTBuffer); Scene.destroy(PreCNBuffer); Scene.destroy(PreMesh);
	}
	DirtyGeometryMeshes.clear();

	// Update mesh visibility
	for (auto MeshComponent: DirtyMeshes)
	{
		ASSERTMSG(MeshIndexMap[MeshComponent] != ~0u, "StaticMeshComponent not exist in scene!");
		accel.set_visibility_on_update(MeshIndexMap[MeshComponent], MeshComponent->IsVisible());
		// auto MaterialID = Scene.GetMaterialProxy()->AddMaterial(MeshComponent->GetMeshData()->GetMaterial());
		// StaticMeshDatas[MeshIndexMap[MeshComponent]].material_id = MaterialID;
	}
	DirtyMeshes.clear();
	if(accel.dirty()) stream << accel.build();
}

}