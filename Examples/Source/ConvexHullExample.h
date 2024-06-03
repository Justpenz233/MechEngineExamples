//
// Created by MarvelLi on 2024/6/3.
//

#pragma once
#include "Game/World.h"
#include "Math/Geometry.h"
#include "Mesh/StaticMesh.h"

inline auto ConvexHullExample()
{
	return [](World& World) {
		auto Rabbit = StaticMesh::LoadObj( "stanford-bunny.obj");
		Rabbit->RotateEuler({M_PI_2, 0., 0.});
		Rabbit->Scale(2.);
		auto ConvexHull = Math::ConvexHull(Rabbit);
		ConvexHull->Scale(1.001); // Avoid z-fighting
		ConvexHull->GetMaterial()->SetBaseColor({ 0.5, 0, 0 });
		ConvexHull->GetMaterial()->SetAlpha(0.5);
		ConvexHull->GetMaterial()->SetShowWireframe(true);
		World.SpawnActor<StaticMeshActor>("Rabbit", Rabbit);
		World.SpawnActor<StaticMeshActor>("ConvexHull", ConvexHull);
	};
}