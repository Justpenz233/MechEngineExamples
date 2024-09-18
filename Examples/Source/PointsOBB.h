//
// Created by MarvelLi on 2024/9/18.
//

#pragma once
#include <Curve/Curve.h>

inline auto PointsOBB()
{
    return [](World& world)
    {
        TArray<FVector> Points = Curve::PanCake()->SampleWithEqualChordLength(64);
        world.SpawnActor<CurveActor>("Curve", Points);
        auto T = Algorithm::GeometryProcess::EstimatePointsOBB(Points);
        auto OBB = world.SpawnActor<StaticMeshActor>("OBB", BasicShapesLibrary::GenerateCuboid(FVector::Constant(1.)))->SetTransform(T);

    };
}