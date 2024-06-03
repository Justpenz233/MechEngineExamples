//
// Created by MarvelLi on 2024/2/28.
//

#pragma once

#include "CoreMinimal.h"
#include "ImguiPlus.h"
#include "LambdaUIWidget.h"
#include "Actors/CurveActor.h"
#include "Game/StaticMeshActor.h"
#include "Game/World.h"
#include "Math/Intersect.h"
#include "Mesh/BasicShapesLibrary.h"
#include "spdlog/stopwatch.h"

/****************************************************************************************
 *
 * This example demonstrates how to use the intersection functions
 *
 ****************************************************************************************/

inline auto IntersectionExample()
{
	return [](World& World) {
		auto MeshActor1 = World.SpawnActor<StaticMeshActor>("Mesh1",BasicShapesLibrary::GenerateHemisphere(1.5));
		auto MeshActor2 = World.SpawnActor<StaticMeshActor>("Mesh2", BasicShapesLibrary::GenerateCuboid({1., 1., 3.}));
		MeshActor1->GetStaticMeshComponent()->GetMeshData()->GetMaterial()->SetAlpha(0.2);
		MeshActor2->GetStaticMeshComponent()->GetMeshData()->GetMaterial()->SetAlpha(0.2);

		auto Mesh1 = MeshActor1->GetStaticMeshComponent()->GetMeshData();
		auto Mesh2 = MeshActor2->GetStaticMeshComponent()->GetMeshData();
		auto [intersect, IntersectTriangles] =
			Math::MeshIntersectMesh(Mesh1, Mesh2, false);

		if(intersect)
		{
			for(int i = 0; i < IntersectTriangles.rows(); i ++)
			{
				auto Index1 = IntersectTriangles(i, 0);
				auto Index2 = IntersectTriangles(i, 1);

				auto Triangle1 = Mesh1->GetTriangle(Index1);
				auto Triangle2 = Mesh2->GetTriangle(Index2);

				auto VertexA0 = Mesh1->GetVertex(Triangle1.x());
				auto VertexA1 = Mesh1->GetVertex(Triangle1.y());
				auto VertexA2 = Mesh1->GetVertex(Triangle1.z());

				auto VertexB0 = Mesh2->GetVertex(Triangle2.x());
				auto VertexB1 = Mesh2->GetVertex(Triangle2.y());
				auto VertexB2 = Mesh2->GetVertex(Triangle2.z());

				auto[Result, SegA, SegB] = TriangleIntersectTriangle(VertexA0, VertexA1, VertexA2, VertexB0, VertexB1, VertexB2);
				if(Result) // This check is necessary, because the result from MeshIntersectMesh may be false positive
					World.DebugDrawLine(SegA, SegB, FVector(1, 0, 0) ,2);
			}
		}
	};
}