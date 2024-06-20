//
// Created by MarvelLi on 2024/6/20.
//

#pragma once
#include "Surface/ParametricSurface.h"

inline auto BuiltinSurfaces()
{
	return [](World& World) {
		//Closed surfaces
		World.SpawnActor<ParametricMeshActor>("Cone", NewObject<ConeSurface>());
		World.SpawnActor<ParametricMeshActor>("Cylinder", NewObject<CylinderSurface>())->SetTranslation(FVector{ 2, 0, 0 });
		World.SpawnActor<ParametricMeshActor>("MobiusStrip", NewObject<MobiusStripSurface>())->SetTranslation(FVector{ 0, 2.3, 0 });
		World.SpawnActor<ParametricMeshActor>("Catenoid", NewObject<CatenoidSurface>())->SetTranslation(FVector{ 0, 5, 0 });
		World.SpawnActor<ParametricMeshActor>("Ellipsoid", NewObject<EllipsoidSurface>())->SetTranslation(FVector{ 3, 2.5, 0 });

		//Open surfaces
		World.SpawnActor<ParametricMeshActor>("Plane", NewObject<PlaneSurface>())->SetTranslation(FVector{ 2, -2, 0 });
		World.SpawnActor<ParametricMeshActor>("PluckeConoidSurface", NewObject<PluckeConoidSurface>())->SetTranslation(FVector{ 0, -2, 0 });
		World.SpawnActor<ParametricMeshActor>("MonkeySaddle", NewObject<MonkeySaddleSurface>())->SetTranslation(FVector{ 0, -4, 0 });
		World.SpawnActor<ParametricMeshActor>("HorseSaddle", NewObject<HorseSaddleSurface>())->SetTranslation(FVector{ 2, -4, 0 });
		World.SpawnActor<ParametricMeshActor>("ParabolicCylinder", NewObject<ParabolicCylinderSurface>())->SetTranslation(FVector{ 2, -6, 0 });
		World.SpawnActor<ParametricMeshActor>("CosConoid", NewObject<CosConoidSurface>())->SetTranslation(FVector{ 0, -6, 0 });
		World.SpawnActor<ParametricMeshActor>("DevelopableHelicoid", NewObject<DevelopableHelicoidSurface>())->SetTranslation(FVector{ 0, -8, 0 });

	};
}