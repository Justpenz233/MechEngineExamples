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
#include "LambdaUIWidget.h"
#include "Actors/CameraActor.h"
#include "Actors/CurveActor.h"
#include "Animation/IKController.h"
#include "Components/ConstPointLightComponent.h"
#include "Game/StaticMeshActor.h"
#include "Game/World.h"
#include "Math/Math.h"
#include "Mechanisms/SpatialJoints.h"
#include "Mesh/BasicShapesLibrary.h"
#include "Mechanisms/ClosedChainIKSolver.h"
#include "Mechanisms/SphericalLinkage.h"
#include "ImguiPlus.h"

inline auto CalcJointTransform (const FVector& Translation, double Radius = 1.f)
{
    // Z axis head to the origin
	FVector Location = Translation.normalized() * Radius;
    FQuat Rotation = FQuat::FromTwoVectors(FVector(0, 0, 1), -Location);
    return FTransform(Location, Rotation);
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

inline TArray<FVector> ReadTargetTrajectory(const Path& TrajectoryFilePath)
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
		Result.emplace_back(x, y, z);
	}
	return Result;
}

inline void WriteSimulationToFile(const TArray<FVector>& Trajectory, const Path& OutputFilePath)
{
	std::fstream OutFile(OutputFilePath, std::ios::out);
	if (!OutFile.is_open())
	{
		LOG_ERROR("Failed to open file: {}", OutputFilePath.string());
		return;
	}
	for (int i = 0;i < Trajectory.size();i ++)
	{
		FVector NormalPoint = Trajectory[i].normalized();
		OutFile << NormalPoint.x() << " " << NormalPoint.y() << " " << NormalPoint.z() << " " << i << std::endl;
	}
}

inline void WriteJointMotionToFile(const TArray<FTransform>& Motions, const Path& OutputFilePath)
{
	std::fstream OutFile(OutputFilePath, std::ios::out);
	if (!OutFile.is_open())
	{
		LOG_ERROR("Failed to open file: {}", OutputFilePath.string());
		return;
	}
	for (int i = 0;i < Motions.size();i ++)
	{
		auto Matrix = Motions[i].GetMatrix();
		for (int Row = 0; Row < 4; Row ++)
		{
			for (int Col = 0; Col < 4; Col ++)
			{
				OutFile << Matrix(Row, Col) << " ";
			}
		}
		OutFile << "\n";
	}
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
        // Camera->AddComponent<ConstPointLightComponent>()->SetIntensity(2.5);

        auto Sphere = world.SpawnActor<StaticMeshActor>("Sphere", BasicShapesLibrary::GenerateSphere(R, 128));

        FVector JointAPos = FVector(1, 0, 0).normalized();
        auto JointA = world.SpawnActor<SphericalLinkageActor>("A", CalcJointTransform(JointAPos, 1.f - SphericalLinkageComponent::Thickness), true);
    	JointA->GetJointComponent()->SetColor({1.f, 0.f, 0.f});

        FVector JointDPos = TransformByPoint(FVector(0, 1, 0),JointAPos, ξ, θx);
        auto JointD = world.SpawnActor<SphericalLinkageActor>("D", CalcJointTransform(JointDPos, 1.f - SphericalLinkageComponent::Thickness * 2.f));
    	JointD->GetJointComponent()->SetColor({0.8f, 0.8f, 0.8f});

        FVector JointBPos = TransformByPoint(JointDPos, JointAPos, α, θ1);
        auto JointB = world.SpawnActor<SphericalLinkageActor>("B",  CalcJointTransform(JointBPos, 1.f));
    	JointB->GetJointComponent()->SetColor({0.f, 1.f, 0.f});

        FVector JointCPos = TransformByPoint(JointBPos, JointDPos, β, -CalcAngleBDC(JointBPos, JointDPos));
        auto JointC = world.SpawnActor<SphericalLinkageActor>("C",  CalcJointTransform(JointCPos, 1.f - SphericalLinkageComponent::Thickness));
    	JointC->GetJointComponent()->SetColor({0.f, 0.f, 1.f});

        FVector JointPPos = TransformByPoint(JointCPos, JointBPos, θp0, θp);
        auto JointP = world.SpawnActor<SpatialJointActor>("P", 'E', CalcJointTransform(JointPPos));
    	Cast<EffectorJoint>(JointP->GetJointComponent())->Radius = 0.01;

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
	         world.SpawnActor<IKController>("Controller", NewObject<ClosedChainIKSolver>());
	     Controller->AddTranslationGlobal({0,0,100});
	     Controller->AddJoints({JointA, JointB, JointC, JointD, JointP});
	     Controller->Run();

    	auto SimulatedSequence = Controller->GetSimulatedTrajectory(JointP->GetJointComponent().get(), 361,
    		[](Joint* Root) {
    			Root->GlobalTransform.AddRotationLocal(MMath::QuaternionFromEulerXYZ(DegToRad(FVector(0, 0, 1))));
    	});

    	auto Trajectory = world.SpawnActor<CurveActor>("Trajectory", SimulatedSequence.Trajectory);
    	Trajectory->GetCurveComponent()->SetRadius(0.001f);

    	world.AddWidget<LambdaUIWidget>([=, &world]()  {
    		ImGui::Begin("Export Mechanism");

			if(ImGui::Button("Export motion and mesh"))
			{
				auto ExportFolderPath = SelectFolderDialog("Select export folder", Path::ProjectContentDir());
				WriteSimulationToFile(SimulatedSequence.Trajectory, Path::ProjectContentDir() / "OutputTrajectory.txt");
				Path ExportFolder = ExportFolderPath.result();
				Path ModelFolder = ExportFolder / "Model";
				Path::CreateDirectory(ModelFolder);
				Path MotionFolder = ExportFolder / "Motion";
				Path::CreateDirectory(MotionFolder);
				JointA->GetJointComponent()->GetMeshData()->SaveOBJ(ModelFolder / "A.obj");
				JointB->GetJointComponent()->GetMeshData()->SaveOBJ(ModelFolder / "B.obj");
				JointC->GetJointComponent()->GetMeshData()->SaveOBJ(ModelFolder / "C.obj");
				JointD->GetJointComponent()->GetMeshData()->SaveOBJ(ModelFolder / "D.obj");
				WriteJointMotionToFile(SimulatedSequence.JointTransforms.at(JointA->GetJointComponent()->GetJoint().get()), MotionFolder / "A.pmt");
				WriteJointMotionToFile(SimulatedSequence.JointTransforms.at(JointB->GetJointComponent()->GetJoint().get()), MotionFolder / "B.pmt");
				WriteJointMotionToFile(SimulatedSequence.JointTransforms.at(JointC->GetJointComponent()->GetJoint().get()), MotionFolder / "C.pmt");
				WriteJointMotionToFile(SimulatedSequence.JointTransforms.at(JointD->GetJointComponent()->GetJoint().get()), MotionFolder / "D.pmt");
			}
    		ImGui::End();
    	});

    };
}
