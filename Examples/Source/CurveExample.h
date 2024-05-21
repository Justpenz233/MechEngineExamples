//
// Created by MarvelLi on 2024/5/21.
//

#pragma once
#include "Game/World.h"
#include "Materials/Material.h"

inline auto CurveExample()
{
	return [](World& world)
	{
		auto OringinalCurveMaterial = NewObject<Material>();
		OringinalCurveMaterial->SetBaseColor({ 0, 1, 0 });

		auto SimplifiedCurveMaterial = NewObject<Material>();
		SimplifiedCurveMaterial->SetBaseColor({ 1, 0, 0 });


		auto OriginalCurve = Curve::TrefoilKnot();
		auto OriginalCurveActor = world.SpawnActor<CurveActor>("OriginalCurve", OriginalCurve);
		OriginalCurveActor->GetCurveComponent()->SetMaterial(OringinalCurveMaterial);

		std::vector<double> points(OriginalCurve->Num() * 3);
		std::memcpy(points.data(), OriginalCurve->GetCurveData().data(), OriginalCurve->Num() * 3 * sizeof(double));
		auto Spline = tinyspline::BSpline::interpolateCatmullRom(points, 3);

		static constexpr int DownSampleNumber = 20;
		auto SimplifiedPoints = Spline.sample(DownSampleNumber);
		TArray<FVector> DownSamplePoint(DownSampleNumber);
		std::memcpy(DownSamplePoint.data(), SimplifiedPoints.data(), DownSampleNumber * 3 * sizeof(double));
		auto SimplifiedCurve = NewObject<Curve>(DownSamplePoint, 3);
		auto SimplifiedCurveActor = world.SpawnActor<CurveActor>("SimplifiedCurve", SimplifiedCurve);
		SimplifiedCurveActor->GetCurveComponent()->SetMaterial(SimplifiedCurveMaterial);

	};
}