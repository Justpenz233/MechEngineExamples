//
// Created by MarvelLi on 2024/5/1.
//

/************************************************************************************
 * SphericalLinkageExample
 * The example show a spherical 4 bar linkage mechanism, the config is from paper's section 5.1 example 1
 * @see Motion generation of spherical four-bar mechanism using harmonic characteristic parameters
 * @see https://www.sciencedirect.com/science/article/pii/S0094114X15002050
 ************************************************************************************/

#pragma once
#include "Actors/CameraActor.h"
#include "Actors/CurveActor.h"
#include "Animation/IKController.h"
#include "Components/ConstPointLightComponent.h"
#include "Game/StaticMeshActor.h"
#include "Game/World.h"
#include "Mechanisms/SpatialJoints.h"
#include "Mesh/BasicShapesLibrary.h"
#include "Mechanisms/ClosedChainIKSolver.h"


inline auto CalcJointTransform (const FVector& Translation)
{
    // Z axis head to the origin
    FQuat Rotation = FQuat::FromTwoVectors(FVector(0, 0, 1), -Translation);
    return FTransform(Translation, Rotation);
}

/**
 * Calculate the joint position according to previous joint position and given arc angle and meridional angle
 * @see https://www.sciencedirect.com/science/article/pii/S0307904X06001582
 */
inline auto TransformByPoint(const FVector& Point,const FVector& Axis, double ArcAngle, double MeridionalAngle)
{
    AngleAxisd rotation1(MeridionalAngle, Axis.normalized());
    auto NewPoint = rotation1 * Point;
	NewPoint.normalize();

    auto Axis2 = Axis.normalized().cross(NewPoint).normalized();
    AngleAxisd rotation2(ArcAngle, Axis2.normalized());
    return rotation2 * Axis.normalized();
}

inline TArray<FVector> ReadTargetTrajectory(Path TrajectoryFilePath)
{
	std::fstream InFile(TrajectoryFilePath, std::ios::in);
	if (!InFile.is_open())
	{
		LOG_ERROR("Failed to open file: {}", TrajectoryFilePath.string());
		return {};
	}
	TArray<FVector> Result;
	for (int i = 0;i < 64;i ++)
	{
		double x,y,z,w;
		InFile >> x >> y >> z >> w;
		Result.emplace_back(x, -y, z);
	}
	return Result;
}

inline auto SphericalLinkageExample()
{
    return [&](World& world) {
        constexpr double α = DegToRad(18.);
        constexpr double γ = DegToRad(50.);
        constexpr double β = DegToRad(39.);
        constexpr double ξ = DegToRad(56.);
        constexpr double θp = DegToRad(56.);
        constexpr double θp0 = DegToRad(25.);
        constexpr double μpq = DegToRad(60.);
        constexpr double θx = DegToRad(30.);
        constexpr double θ1 = DegToRad(28.);
        constexpr double R = 1.;

        auto CalcAngleBDC = [&](FVector B, FVector D) {
        	double BD = (B - D).norm();
        	double BC = γ;
        	double CD = β;
			return acos((BD * BD + CD * CD - BC * BC) / (2. * BD * CD));
        };

        auto Camera = world.SpawnActor<CameraActor>("MainCamera");
        Camera->SetTranslation({5, 2, 0}); Camera->LookAt();
        Camera->AddComponent<ConstPointLightComponent>()->SetIntensity(2.5);

        auto Sphere = world.SpawnActor<StaticMeshActor>("Sphere", BasicShapesLibrary::GenerateSphere(R));

    	auto Trajectory = world.SpawnActor<CurveActor>("Trajectory",
    		ReadTargetTrajectory(Path::ProjectContentDir() / "SphericalLinkageTrajectory.txt"));
    	Trajectory->GetCurveComponent()->SetRadius(0.001f);

        FVector JointAPos(R, 0, 0);
        auto JointA = world.SpawnActor<SpatialJointActor>("A", 'R', CalcJointTransform(JointAPos));
    	JointA->GetJointComponent()->SetColor({1.f, 0.f, 0.f});

        FVector JointDPos = TransformByPoint(FVector(0, 1, 0),JointAPos, ξ, θx);
        auto JointD = world.SpawnActor<SpatialJointActor>("D", 'R', CalcJointTransform(JointDPos));
    	JointD->GetJointComponent()->SetColor({0.8f, 0.8f, 0.8f});

        FVector JointBPos = TransformByPoint(JointDPos, JointAPos, α, θ1);
        auto JointB = world.SpawnActor<SpatialJointActor>("B", 'R', CalcJointTransform(JointBPos));
    	JointB->GetJointComponent()->SetColor({0.f, 1.f, 0.f});

        FVector JointCPos = TransformByPoint(JointBPos, JointDPos, β, -CalcAngleBDC(JointBPos, JointDPos));
        auto JointC = world.SpawnActor<SpatialJointActor>("C", 'R', CalcJointTransform(JointCPos));
    	JointC->GetJointComponent()->SetColor({0.f, 0.f, 1.f});

        FVector JointPPos = TransformByPoint(JointCPos, JointBPos, θp0, θp);
        auto JointP = world.SpawnActor<SpatialJointActor>("P", 'E', CalcJointTransform(JointPPos));

        FVector JointQPos = TransformByPoint(JointBPos, JointPPos, 0.5, μpq);
        auto JointQ = world.SpawnActor<SpatialJointActor>("Q", 'E', CalcJointTransform(JointQPos));

        JointA->GetJointComponent()->GetIKJoint()->SetIsRoot(true);
        JointA->AddNextJoint(JointB);
        JointB->AddNextJoint(JointC);
        JointC->AddNextJoint(JointD);
        JointD->AddNextJoint(JointA);
        JointB->AddNextJoint(JointP);
    	// JointP->AddNextJoint(JointQ);

	     auto Controller =
	         world.SpawnActor<IKController<ClosedChainIKSolver>>("Controller");
	     Controller->AddTranslationGlobal({0,0,100});
	     Controller->AddJoints({JointA, JointB, JointC, JointD});
	     Controller->Run();
    };
}
