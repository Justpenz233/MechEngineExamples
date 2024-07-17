//
// Created by MarvelLi on 2024/6/12.
//

#pragma once
#include "LambdaUIWidget.h"
#include "Game/World.h"
#include "ImguiPlus.h"
#include "Game/StaticMeshActor.h"
#include "Mesh/StaticMesh.h"

inline auto ExtrudeMeshExample()
{
	return [](World& world) {
		auto Bunny = StaticMesh::LoadObj("stanford-bunny.obj");

		auto OffsetNormal = world.SpawnActor<StaticMeshActor>("OffsetByNormal", Bunny);
		world.AddWidget<LambdaUIWidget>([OffsetNormal, Bunny]() {
			static float Offset = 0.;
			if(ImGui::Begin("Extrude Mesh Example", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
			{
				if(ImGui::InputFloat("Offeset by normal distance: ", &Offset, 0.01))
				{
					auto OffsetMesh = NewObject<StaticMesh>(*Bunny)->OffsetVertex(Offset); // Copy the mesh and offset the vertices
					OffsetNormal->GetStaticMeshComponent()->SetMeshData(OffsetMesh);
				}
				ImGui::End();
			}
		});
	};
}