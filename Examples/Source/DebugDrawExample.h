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
		auto BoxActor = world.SpawnActor<StaticMeshActor>("Box", BasicShapesLibrary::GenerateCuboid(FVector::Constant(8.0)));
		auto LineComponent = BoxActor->AddComponent<LinesComponent>();
		// Build the cube box lines with 12 lines
		LineComponent->AddLine(FVector(-4, -4, -4), FVector(4, -4, -4), RGB(255, 0, 0), 2);
		LineComponent->AddLine(FVector(-4, -4, -4), FVector(-4, 4, -4), RGB(255, 0, 0), 2);
		LineComponent->AddLine(FVector(-4, -4, -4), FVector(-4, -4, 4), RGB(255, 0, 0), 2);
		LineComponent->AddLine(FVector(4, 4, 4), FVector(-4, 4, 4), RGB(255, 0, 0), 2);
		LineComponent->AddLine(FVector(4, 4, 4), FVector(4, -4, 4), RGB(255, 0, 0), 2);
		LineComponent->AddLine(FVector(4, 4, 4), FVector(4, 4, -4), RGB(255, 0, 0), 2);
		LineComponent->AddLine(FVector(-4, 4, 4), FVector(-4, -4, 4), RGB(255, 0, 0), 2);
		LineComponent->AddLine(FVector(-4, 4, 4), FVector(-4, 4, -4), RGB(255, 0, 0), 2);
		LineComponent->AddLine(FVector(4, -4, 4), FVector(-4, -4, 4), RGB(255, 0, 0), 2);
		LineComponent->AddLine(FVector(4, -4, 4), FVector(4, -4, -4), RGB(255, 0, 0), 2);
		LineComponent->AddLine(FVector(4, -4, -4), FVector(-4, -4, -4), RGB(255, 0, 0), 2);

		LineComponent->AddPoint(FVector(0, 0, 0), 10, RGB(255, 0, 0));

	};
}
