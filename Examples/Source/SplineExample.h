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
		auto Camera = world.SpawnActor<CameraActor>("Camera");
		Camera->SetTranslation({0,0,15});
		Camera->LookAt();
		{ // 3D curve
			auto CurveData = Curve::TrefoilKnot();
			world.SpawnActor<CurveActor>("3DCurve", CurveData);

			std::vector<double> points(CurveData->Num() * 3);
			std::memcpy(points.data(), CurveData->GetCurveData().data(), CurveData->Num() * 3 * sizeof(double));

			auto Spline = tinyspline::BSpline::interpolateCubicNatural(points, 3);
			auto Knots = Spline.equidistantKnotSeq(20);
			auto Frames = Spline.computeRMF(Knots);

			LOG_INFO("3D Curve Length: {}", Spline.chordLengths().arcLength());

			for(int i = 0; i < Frames.size();i ++)
			{
				auto Frame = Frames.at(i);
				auto Pos = Frame.position();
				auto Normal = Pos + Frame.normal();
				world.DebugDrawPoint(FVector{Pos.x(), Pos.y(), Pos.z()}, 20, FVector{0, 1, 0});
				world.DebugDrawLine(FVector{Pos.x(), Pos.y(), Pos.z()}, FVector{Normal.x(), Normal.y(), Normal.z()}, FVector{1, 0, 0}, 1.5);
			}
		}

		{// 2D curve
			auto CurveData = Curve::Rect(2., 1.);
			world.SpawnActor<CurveActor>("2DCurve", CurveData);

			std::vector<double> points(CurveData->Num() * 2);
			for (int i = 0; i < CurveData->Num(); i++)
			{
				points[i * 2] = CurveData->GetCurveData()[i].x();
				points[i * 2 + 1] = CurveData->GetCurveData()[i].y();
			}

			auto Spline = tinyspline::BSpline::interpolateCubicNatural(points, 2);
			auto Knots = Spline.equidistantKnotSeq(20);
			auto Frames = Spline.computeRMF(Knots);

			LOG_INFO("2D Curve Length: {}", Spline.chordLengths().arcLength());

			for(int i = 0; i < Frames.size();i ++)
			{
				auto Frame = Frames.at(i);
				auto Pos = Frame.position();
				auto Normal = Pos + Frame.normal();
				world.DebugDrawPoint(FVector{Pos.x(), Pos.y(), Pos.z()}, 20, FVector{0, 1, 0});
				world.DebugDrawLine(FVector{Pos.x(), Pos.y(), Pos.z()}, FVector{Normal.x(), Normal.y(), Normal.z()}, FVector{1, 0, 0}, 1.5);
			}

		}
	};
}