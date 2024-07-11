//
// Created by MarvelLi on 2024/5/12.
//

#pragma once
#include "Components/LinesComponent.h"
#include "Game/StaticMeshActor.h"
#include "Game/World.h"
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
		world.DebugDrawLine(FVector{-20,-20,-20}, FVector{20,20,20}, RGB(0, 0, 0), 2);
	};
}
