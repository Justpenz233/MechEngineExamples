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
		auto ConvexHull = Math::ConvexHull(Rabbit);
		ConvexHull->GetMaterial()->SetBaseColor({ 0.5, 0, 0 });
		ConvexHull->GetMaterial()->SetAlpha(0.3);
		ConvexHull->GetMaterial()->SetShowWireframe(true);
		World.SpawnActor<StaticMeshActor>("Rabbit", Rabbit);
		World.SpawnActor<StaticMeshActor>("ConvexHull", ConvexHull);
	};
}