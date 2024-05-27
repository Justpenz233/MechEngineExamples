//
// Created by MarvelLi on 2024/2/3.
//

#pragma once
#include "Core/CoreMinimal.h"
#include "Game/StaticMeshActor.h"
#include "Game/World.h"
#include "Mesh/BasicShapesLibrary.h"
#include "Mesh/MeshBoolean.h"

inline auto MeshBooleanTest()
{
	return [](World& World)
	{

		ObjectPtr<StaticMesh> Mesh1 = BasicShapesLibrary::GenerateCylinder(1., 0.5);
		ObjectPtr<StaticMesh> Mesh2 = BasicShapesLibrary::GenerateSphere(0.5);
		Mesh2->Translate({0,0,0.5});
		ObjectPtr<StaticMesh> MeshR1 = MeshBoolean::MeshUnion(Mesh1, Mesh2);
		ObjectPtr<StaticMesh> MeshR2 = MeshBoolean::MeshMinus(Mesh1, Mesh2);
		ObjectPtr<StaticMesh> MeshR3 = MeshBoolean::MeshConnect(Mesh1, Mesh2);


		auto Cylinder = World.SpawnActor<StaticMeshActor>("Mesh1");
		Cylinder->GetStaticMeshComponent()->SetMeshData(Mesh1);

		auto Sphere = World.SpawnActor<StaticMeshActor>("Mesh2");
		Sphere->GetStaticMeshComponent()->SetMeshData(Mesh2);

		auto Result2 = World.SpawnActor<StaticMeshActor>("ResultMinus");
		Result2->GetStaticMeshComponent()->SetMeshData(MeshR2);
		Result2->GetStaticMeshComponent()->GetMeshData()->GetMaterial()->SetShowWireframe(true);
		Result2->SetTranslation({0.,-2.,0.});

		auto Result = World.SpawnActor<StaticMeshActor>("ResultUnion");
		Result->GetStaticMeshComponent()->SetMeshData(MeshR1);
		Result->GetStaticMeshComponent()->GetMeshData()->GetMaterial()->SetShowWireframe(true);
		Result->SetTranslation({0.,0.,-2.});

		auto Result3 = World.SpawnActor<StaticMeshActor>("ResultConnect");
		Result3->GetStaticMeshComponent()->SetMeshData(MeshR3);
		Result3->GetStaticMeshComponent()->GetMeshData()->GetMaterial()->SetShowWireframe(true);
		Result3->SetTranslation({0.,0.,2.});

	};
}