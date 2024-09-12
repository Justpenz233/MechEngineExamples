//
// Created by MarvelLi on 2024/6/4.
//

#pragma once
#include "Actors/ParametricMeshActor.h"
#include "Game/StaticMeshActor.h"
#include "Game/World.h"
#include "Mesh/BasicShapesLibrary.h"

/************************************************************************
 * Project a 3D point to 2D surface                                     *
 ************************************************************************/

inline auto ProjectToSurfaceExample()
{
	return [](World& World) {

		// auto Surface = World.SpawnActor<ParametricMeshActor>("Spot", StaticMesh::LoadObj("Spot.obj"), SphereicalConformal);
		// Surface->GetParametricMeshComponent()->GetMeshData()->GetMaterial()->SetAlpha(0.4);

		auto Surface = World.SpawnActor<ParametricMeshActor>("OpenBunny", StaticMesh::LoadObj("openbunny.obj"), BoxBorderConformal);
		Surface->SetScale(FVector::Constant(10.));
		Surface->GetParametricMeshComponent()->GetMeshData()->GetMaterial()->SetAlpha(0.4);

		// auto Surface = World.SpawnActor<ParametricMeshActor>("Cone", NewObject<ConeSurface>());
		// Surface->GetParametricMeshComponent()->GetMeshData()->GetMaterial()->SetAlpha(0.4);

		auto TargetPoint = World.SpawnActor<StaticMeshActor>("3D Point", BasicShapesLibrary::GenerateSphere(0.02));

		auto ProjectPoint = World.SpawnActor<StaticMeshActor>("Projected Point", BasicShapesLibrary::GenerateSphere(0.02));
		ProjectPoint->TickFunction = [TargetPoint, Surface](double DeltaTime, Actor* actor) {
			actor->SetTranslation(Surface->Sample(Surface->Projection(TargetPoint->GetTranslation())));
		};
	};
}