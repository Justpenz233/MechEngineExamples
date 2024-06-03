//
// Created by MarvelLi on 2024/6/3.
//
#pragma once
#include "tinysplinecxx.h"
#include "Actors/CurveActor.h"

inline auto SplineExample()
{
	return [](World& world)
	{
		auto CurveData = Curve::TrefoilKnot();
		world.SpawnActor<CurveActor>("OriginalCurve", CurveData);

		std::vector<double> points(CurveData->Num() * 3);
		std::memcpy(points.data(), CurveData->GetCurveData().data(), CurveData->Num() * 3 * sizeof(double));

		auto Spline = tinyspline::BSpline::interpolateCubicNatural(points, 3);
		auto Knots = Spline.equidistantKnotSeq(20);
		auto Frames = Spline.computeRMF(Knots);
		for(int i = 0; i < Frames.size();i ++)
		{
			double t = i / double(Frames.size() - 1);
			auto Frame = Frames.at(i);
			auto Pos = Frame.position();
			auto Normal = Pos + Frame.normal();
			world.DebugDrawPoint(FVector{Pos.x(), Pos.y(), Pos.z()}, 20, FVector{0, 1, 0} * t);
			world.DebugDrawLine(FVector{Pos.x(), Pos.y(), Pos.z()}, FVector{Normal.x(), Normal.y(), Normal.z()}, FVector{1, 0, 0} * t, 1.5);
		}
	};
}