//
// Created by MarvelLi on 2024/3/13.
//
#pragma once
#include <imgui.h>
#include "LambdaUIWidget.h"
#include "Actors/CameraActor.h"
#include "Actors/ParametricMeshActor.h"
#include "Game/StaticMeshActor.h"
#include "Game/World.h"
#include "Materials/Material.h"
#include "Mesh/BasicShapesLibrary.h"
#include "Misc/Path.h"


/****************************************************************************************
 * ParametricMeshExamples
 *
 * [[AI GENERATED]]
 * This function demonstrates how to use the ParametricMeshActor to create a parametric
 * mesh with different parametrization methods. The function creates a bunny mesh and a
 * catenoid mesh, and allows the user to interact with the meshes by changing the UV
 * coordinates. The function also creates a spot mesh with a spherical conformal
 * parametrization method, and allows the user to interact with the mesh by changing the
 * UV coordinates.
 * The UV coordinates can be changed by dragging the sliders in the ImGui window.
 *
 *
 * Methods of parametrization:
 * 1. SCAF Parametrization Method will provide a conformal map with a circular boundary. Guarantee global injective. Require an open mesh.
 * 2. BoxBorderConformal Parametrization Method will provide a conformal map with a box boundary. Guarantee global injective. Require an open mesh.
 * 3. SphereicalConformal Parametrization Method will provide a conformal map with a spherical boundary. Guarantee global injective. Require a closed mesh with genus 0, overlapping free.
 * 4. Directly math defined parametric surface. See more at ParametricSurface.h
 ****************************************************************************************/


inline auto ParametricMeshExamples()
{
    return [](World& World)
    {
        auto Camera = World.SpawnActor<CameraActor>("MainCamera");
        Camera->SetTranslation({-5, 0, 0}); Camera->LookAt();

        auto Surface = World.SpawnActor<ParametricMeshActor>("Bunny_SCAF", StaticMesh::LoadObj(Path("openbunny.obj"))->Normalize()->GetThis<StaticMesh>(), BoxBorderConformal);
    	Surface->SetTranslation({0,2,0});
        auto BunnyUVIndicator = World.SpawnActor<StaticMeshActor>("BunnyUVIndicator", BasicShapesLibrary::GenerateSphere(0.03, 64));
        BunnyUVIndicator->GetStaticMeshComponent()->GetMeshData()->GetMaterial()->SetBaseColor({1, 0, 0});
        World.AddWidget<LambdaUIWidget>([Surface, BunnyUVIndicator]() {
            ImGui::Begin("Parametrization Example", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
            static float UV[2] = {0, 0};
            ImGui::DragFloat2("Bunny UV", UV, 0.01, 0., 1.);
            if (Surface->ValidUV(UV[0], UV[1]))
            {
                auto P = Surface->Sample(UV[0], UV[1]);
                BunnyUVIndicator->SetTranslation(P);
                ImGui::Text("Pos: %lf ,%lf ,%lf", P[0], P[1], P[2]);
            }
            else
            {
                ImGui::Text("UV invalid!");
            }
            ImGui::End();
        });

        auto Surface2 = World.SpawnActor<ParametricMeshActor>("ParametrizationSurface", NewObject<CatenoidSurface>()); Surface2->SetScale({0.5, 0.5, 0.5});
        auto CatenoidUVIndicator = World.SpawnActor<StaticMeshActor>("CatenoidUVIndicator", BasicShapesLibrary::GenerateSphere(0.05, 64));
        CatenoidUVIndicator->GetStaticMeshComponent()->GetMeshData()->GetMaterial()->SetBaseColor({1, 0, 0});

        Surface2->SetTranslation({0.5, 0, 0});
        World.AddWidget<LambdaUIWidget>([Surface2, CatenoidUVIndicator]() {
            ImGui::Begin("Parametrization Example", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
            static float UV[2] = {0, 0};
            ImGui::DragFloat2(" Catenoid UV", UV, 0.01, 0., 1.);
            if (Surface2->ValidUV(UV[0], UV[1]))
            {
                auto P = Surface2->Sample(UV[0], UV[1]);
                CatenoidUVIndicator->SetTranslation(P);
                ImGui::Text("Pos: %lf ,%lf ,%lf", P[0], P[1], P[2]);
            }
            else
            {
                ImGui::Text("UV invalid!");
            }
            ImGui::End();
        });


    	auto Spot = World.SpawnActor<ParametricMeshActor>("Spot_SphereicalConformal", StaticMesh::LoadObj(Path("spot.obj"))->Normalize()->GetThis<StaticMesh>(), SphereicalConformal);
    	Spot->SetTranslation({0, -2, 0}); Spot->SetRotation({M_PI *0.5, 0, 0});
    	auto SpotUVIndicator = World.SpawnActor<StaticMeshActor>("BunnyUVIndicator", BasicShapesLibrary::GenerateSphere(0.03, 64));
    	SpotUVIndicator->GetStaticMeshComponent()->GetMeshData()->GetMaterial()->SetBaseColor({1, 0, 0});
    	World.AddWidget<LambdaUIWidget>([Spot, SpotUVIndicator]() {
			ImGui::Begin("Parametrization Example", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
			static float UV[2] = {0, 0};
			ImGui::DragFloat2("Spot UV", UV, 0.01, 0., 1.);
			if (Spot->ValidUV(UV[0], UV[1]))
			{
				auto P = Spot->Sample(UV[0], UV[1]);
				SpotUVIndicator->SetTranslation(P);
				ImGui::Text("Pos: %lf ,%lf ,%lf", P[0], P[1], P[2]);
			}
			else
			{
				ImGui::Text("UV invalid!");
			}
			ImGui::End();
		});
    };
}