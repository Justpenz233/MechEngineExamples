//
// Created by MarvelLi on 2024/5/12.
//

#pragma once
#include "Components/LinesComponent.h"
#include "Game/StaticMeshActor.h"
#include "Game/World.h"
#include "Math/Random.h"
#include "Mesh/BasicShapesLibrary.h"

inline auto DebugDrawExample()
{
	return
	[](World& world) {
		auto BoxActor = world.SpawnActor<StaticMeshActor>("Box", BasicShapesLibrary::GenerateCuboid(FVector::Constant(1.0)));

		// Draw cube
		world.DebugDrawCube(FVector::Zero(), FVector::Constant(1.0), RGB(255, 0, 0), 2);

		// Draw point at center
		world.DebugDrawPoint(FVector(0, 0, 0), 20, RGB(0, 255, 0));

		auto BBox = BoxActor->GetStaticMeshComponent()->GetMeshData()->GetBoundingBox();

		// Draw vertices
		for (const auto& Vertex : BBox.GetVertex())
		{
			world.DebugDrawPoint(Vertex, 10, RGB(0, 255, 0));
		}

		// Super long line, test clipping and culling
		// When turn off cliping, this will result crash.
		for(int i = 0;i < 1000;i ++)
		{
			FVector Start = Random::RandomFVector() * 50;
			world.DebugDrawLine(-1. * Start, Start, RGB(0, 0, 0), 2);
		}
	};
}
