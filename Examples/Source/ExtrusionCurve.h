//
// Created by MarvelLi on 2024/6/25.
//

#pragma once
#include "Mesh/BasicShapesLibrary.h"

inline auto ExtrusionCurve()
{

	return [](World& World) {
		auto Triangle = [](double t) -> FVector2 {
			t *= 3.;
			if (t < 1.)
				return FVector2(-0.5 + t, t*0.5);
			else if (t < 2.)
				return FVector2(0.5, 0.5-(t - 1));
			else
				return FVector2(0.5-(t-2), -0.5 + (t - 2.)*0.5);
		};

		auto Trajectory = Curve::Circle(1);

		World.SpawnActor<StaticMeshActor>("TriangleExtude", BasicShapesLibrary::GenerateExtrudeMesh(Trajectory->GetCurveData(), Triangle, 3, 64))
		->SetRotation({0, M_PI * 0.5, 0});
	};


}