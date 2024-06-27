//
// Created by MarvelLi on 2024/5/28.
//

#pragma once
#include "Game/StaticMeshActor.h"
#include "Game/World.h"
#include "Materials/Material.h"
#include "Mesh/BasicShapesLibrary.h"

inline auto WireFrameMaterialExample()
{
	return [](World& world) {
		auto SphereMaterial = world.SpawnActor<StaticMeshActor>("Sphere", BasicShapesLibrary::GenerateSphere(1.5))
		->GetStaticMeshComponent()->GetMeshData()->GetMaterial();
		SphereMaterial->SetShowWireframe(true); SphereMaterial->SetAlpha(0.2f);

		auto CubeMaterial = world.SpawnActor<StaticMeshActor>("Cube", BasicShapesLibrary::GenerateCuboid(3))
			->GetStaticMeshComponent()->GetMeshData()->GetMaterial();
		CubeMaterial->SetShowWireframe(true); CubeMaterial->SetAlpha(0.4f);

		world.SpawnActor<StaticMeshActor>("Rabit", StaticMesh::LoadObj("stanford-bunny.obj")->Normalized());

	};
}