//
// Created by Mayn on 2024/9/14.
//

#pragma once
#include "Actors/LightActor.h"

inline auto CornellBox()
{
	return [](World& world)
	{
		auto Camera = world.SpawnActor<CameraActor>("MainCamera");
		Camera->SetTranslation({-10.760, 0.162, 1}); Camera->GetCameraComponent()->SetFovH(19.5);

		auto LeftWall = world.SpawnActor<StaticMeshActor>("LeftWall", BasicShapesLibrary::GenerateCuboid(FVector{2., 0.02, 2.}));
		LeftWall->SetTranslation(FVector{0, -1, 1}); LeftWall->GetMaterial()->SetBaseColor({0.63, 0.065, 0.05});

		auto RightWall= world.SpawnActor<StaticMeshActor>("RightWall", BasicShapesLibrary::GenerateCuboid(FVector{2., 0.02, 2.}));
		RightWall->SetTranslation(FVector{0, 1, 1}); RightWall->GetMaterial()->SetBaseColor({0.14, 0.45, 0.091});

		auto Floor = world.SpawnActor<StaticMeshActor>("Floor", BasicShapesLibrary::GenerateCuboid(FVector{2, 2, 0.02}));
		Floor->GetMaterial()->SetBaseColor({0.725, 0.71, 0.68});

		auto Ceiling = world.SpawnActor<StaticMeshActor>("Ceiling", BasicShapesLibrary::GenerateCuboid(FVector{2, 2, 0.02}));
		Ceiling->SetTranslation(FVector{0, 0, 2}); Ceiling->GetMaterial()->SetBaseColor({0.725, 0.71, 0.68});

		auto BackWall = world.SpawnActor<StaticMeshActor>("BackWall", BasicShapesLibrary::GenerateCuboid(FVector{0.02, 2, 2.}));
		BackWall->SetTranslation(FVector{1, 0, 1}); BackWall->GetMaterial()->SetBaseColor({0.725, 0.71, 0.68});

		auto ShotBox = world.SpawnActor<StaticMeshActor>("ShotBox", BasicShapesLibrary::GenerateCuboid(FVector{0.594811, 0.6, 0.604394}));
		ShotBox->SetTranslation(FVector{-0.328631, 0.374592, 0.3})->SetRotation(FVector{0, 0, DegToRad(-163.36)});
		ShotBox->GetMaterial()->SetBaseColor({0.725, 0.71, 0.68});

		auto TallBox = world.SpawnActor<StaticMeshActor>("TallBox", BasicShapesLibrary::GenerateCuboid(FVector{0.607289, 0.597739, 1.2}));
		TallBox->SetTranslation(FVector{0.335439, -0.291415, 0.6,})->SetRotation(FVector{0, 0, DegToRad(160.812)});
		TallBox->GetMaterial()->SetBaseColor({0.725, 0.71, 0.68});


		auto Light = world.SpawnActor<AreaLightActor>("AreaLight");
		Light->SetTranslation({-0.005, -0.03, 1.98});
		Light->GetLightComponent()->SetSize({0.47, 0.38});
		Light->GetLightComponent()->SetIntensity({17, 12, 4});
	};
}