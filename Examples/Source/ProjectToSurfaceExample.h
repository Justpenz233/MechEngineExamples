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
		auto Surface1 = World.SpawnActor<ParametricMeshActor>("Cone", "Catenoid");
		Surface1->GetParametricMeshComponent()->GetMeshData()->GetMaterial()->SetAlpha(0.4);

		auto TargetPoint = World.SpawnActor<StaticMeshActor>("3D Point", BasicShapesLibrary::GenerateSphere(0.02));

		auto ProjectPoint = World.SpawnActor<StaticMeshActor>("Projected Point", BasicShapesLibrary::GenerateSphere(0.02));
		ProjectPoint->TickFunction = [TargetPoint, Surface1](double DeltaTime, Actor* actor) {
			actor->SetTranslation(Surface1->Sample(Surface1->Projection(TargetPoint->GetTranslation())));
		};
	};
}