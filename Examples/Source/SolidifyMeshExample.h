//
// Created by MarvelLi on 2024/6/27.
//

#pragma once
#include "LambdaUIWidget.h"
#include "Algorithm/GeometryProcess.h"
#include "Game/StaticMeshActor.h"
#include "Misc/Path.h"

/**************************************************************************************************
 * SolidifyMeshExample
 * This example demonstrates how to solidify a mesh by offsetting the mesh with vertex normal direction.
 * User can provide either a closed mesh or a open mesh, the algorithm will work for both cases.
 **************************************************************************************************/

inline auto SolidifyMeshExample()
{
	return [](World& World)
	{
		auto Mesh = StaticMesh::LoadObj( Path::ProjectContentDir() / "openbunny.obj");
		LOG_TEMP("{}", Mesh->GetVertexNum());
		auto Actor = World.SpawnActor<StaticMeshActor>("SolidifiedBunny", Mesh);
		World.AddWidget<LambdaUIWidget>([Mesh, Actor]() {
			if(ImGui::Begin("Adjust Solidify Mesh Thickness"))
			{
				static float Thickness = 0;
				if(ImGui::SliderFloat("Thickness", &Thickness, -1, 1))
				{
					auto NewMesh = Algorithm::GeometryProcess::SolidifyMesh(Mesh, Thickness);
					Actor->GetStaticMeshComponent()->SetMeshData(NewMesh);
				}
				ImGui::End();
			}
		});
	};
}