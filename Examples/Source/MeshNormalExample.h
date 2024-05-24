//
// Created by MarvelLi on 2024/5/24.
//

#pragma once
#include "Game/StaticMeshActor.h"
#include "Game/World.h"
#include "Materials/Material.h"
#include "Mesh/BasicShapesLibrary.h"

auto MeshNormalExample(){
	return [](World& World) {
		auto FaceNormalActor = World.SpawnActor<StaticMeshActor>("FaceNormal", BasicShapesLibrary::GenerateCylinder(1., 0.2, 64));
		FaceNormalActor->GetStaticMeshComponent()->GetMeshData()->GetMaterial()->SetNormalType(FaceNormal);
		FaceNormalActor->SetTranslation({0, -1, 0});

		auto VertexNormalActor = World.SpawnActor<StaticMeshActor>("VertexNormal", BasicShapesLibrary::GenerateCylinder(1., 0.2, 64));
		VertexNormalActor->GetStaticMeshComponent()->GetMeshData()->GetMaterial()->SetNormalType(VertexNormal);

		auto CornerNormalActor = World.SpawnActor<StaticMeshActor>("CornerNormal", BasicShapesLibrary::GenerateCylinder(1., 0.2, 64));
		CornerNormalActor->GetStaticMeshComponent()->GetMeshData()->GetMaterial()->SetNormalType(CornerNormal);
		CornerNormalActor->SetTranslation({0, 1, 0});
	};
};