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
		Camera->SetTranslation({-3.8, 0.2, -0.15});

		auto RightWall= world.SpawnActor<StaticMeshActor>("RightWall", BasicShapesLibrary::GenerateCuboid(FVector{2., 0.2, 2.}));
		RightWall->SetTranslation(FVector{0, 1, 0}); RightWall->GetMaterial()->SetBaseColor(RGB(0, 255, 0));
		auto LeftWall = world.SpawnActor<StaticMeshActor>("LeftWall", BasicShapesLibrary::GenerateCuboid(FVector{2., 0.2, 2.}));
		LeftWall->SetTranslation(FVector{0, -1, 0}); LeftWall->GetMaterial()->SetBaseColor(RGB(255, 0, 0));
		world.SpawnActor<StaticMeshActor>("BackWall", BasicShapesLibrary::GenerateCuboid(FVector{0.2, 2, 2.}))
		->SetTranslation(FVector{1, 0, 0});
		world.SpawnActor<StaticMeshActor>("Ceiling", BasicShapesLibrary::GenerateCuboid(FVector{2, 2, 0.2}))
		->SetTranslation(FVector{0, 0, -1});
		world.SpawnActor<StaticMeshActor>("Floor", BasicShapesLibrary::GenerateCuboid(FVector{2, 2, 0.2}))
		->SetTranslation(FVector{0, 0, 1});

		// auto Light = world.SpawnActor<PointLightActor>("PointLight");
		// Light->SetTranslation({0, 0, 0.6});
		// Light->GetPointLightComponent()->SetIntensity(3.);


		auto Light = world.SpawnActor<AreaLightActor>("AreaLight");
		Light->SetTranslation({0, 0, 0.899});
		Light->GetLightComponent()->SetSize({0.6, 0.6});


		// world.SpawnActor<StaticMeshActor>("Bunny", StaticMesh::LoadObj(Path("stanford-bunny.obj")))->SetScale(FVector::Constant(5.0))
		// ->SetTranslation(FVector{0, 0, -1})->SetRotation(FVector{M_PI * 0.5, 0, 0});

		world.SpawnActor<StaticMeshActor>("Cube", BasicShapesLibrary::GenerateCuboid(FVector::Constant(0.5)))
		->SetTranslation(FVector{0,0,-0.5});

	};
}