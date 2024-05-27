#pragma once
#include "Actors/CameraActor.h"
#include "Materials/Material.h"
#include "Game/StaticMeshActor.h"
#include "Game/World.h"
#include "Mesh/BasicShapesLibrary.h"

/***********************************************************************************************************
 *  This is a simple example that demonstrates how to create basic shapes using the BasicShapesLibrary.    *
 *           The example creates a cylinder, a sphere, a hollow cylinder,								   *
 *              a cube, a capsule, a hemisphere, and a one-hat capsule. 								   *
 ***********************************************************************************************************/

inline auto BasicShapesExample()
{
    return [](World& World)
    {
    	World.SpawnActor<CameraActor>("Camera")->SetTranslation({-10,1,1});
        auto Cylinder = World.SpawnActor<StaticMeshActor>("Cylinder"); Cylinder->SetTranslation({0,-1,0});
        Cylinder->GetStaticMeshComponent()->SetMeshData(BasicShapesLibrary::GenerateCylinder(1., 0.5));
    	Cylinder->GetStaticMeshComponent()->GetMeshData()->GetMaterial()->SetShowWireframe(true);

        auto Sphere = World.SpawnActor<StaticMeshActor>("Sphere"); Sphere->SetTranslation({0,1,0});
        Sphere->GetStaticMeshComponent()->SetMeshData(BasicShapesLibrary::GenerateSphere(0.5));
    	Sphere->GetStaticMeshComponent()->GetMeshData()->GetMaterial()->SetShowWireframe(true);

        auto HollowCylinder = World.SpawnActor<StaticMeshActor>("HollowCylinder"); HollowCylinder->SetTranslation({0,2,0});
        HollowCylinder->GetStaticMeshComponent()->SetMeshData(BasicShapesLibrary::GenerateHollowCylinder(0.5, 0.25, 0.75));
    	HollowCylinder->GetStaticMeshComponent()->GetMeshData()->GetMaterial()->SetShowWireframe(true);
    
        auto Cube = World.SpawnActor<StaticMeshActor>("Cube"); Cube->SetTranslation({0,-2,0});
        Cube->GetStaticMeshComponent()->SetMeshData(BasicShapesLibrary::GenerateCuboid({0.75, 0.5, 0.25}));
    	Cube->GetStaticMeshComponent()->GetMeshData()->GetMaterial()->SetShowWireframe(true);

    	auto Capsule = World.SpawnActor<StaticMeshActor>("Capsule"); Capsule->SetTranslation({0,0,1});
		Capsule->GetStaticMeshComponent()->SetMeshData(BasicShapesLibrary::GenerateCapsule(0.5, 1.));
    	Capsule->GetStaticMeshComponent()->GetMeshData()->GetMaterial()->SetShowWireframe(true);

    	auto Hemisphere = World.SpawnActor<StaticMeshActor>("Hemisphere"); Hemisphere->SetTranslation({0,1,1});
    	Hemisphere->GetStaticMeshComponent()->SetMeshData(BasicShapesLibrary::GenerateHemisphere(0.5, false));
    	Hemisphere->GetStaticMeshComponent()->GetMeshData()->GetMaterial()->SetShowWireframe(true);

    	auto OneHatCapsule = World.SpawnActor<StaticMeshActor>("OneHatCapsule"); OneHatCapsule->SetTranslation({0,2,2});
    	OneHatCapsule->GetStaticMeshComponent()->SetMeshData(BasicShapesLibrary::GenerateOneHatCapsule(0.5, 1., false));
    	OneHatCapsule->GetStaticMeshComponent()->GetMeshData()->GetMaterial()->SetShowWireframe(true);
    };
}