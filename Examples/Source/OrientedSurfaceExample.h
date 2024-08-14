//
// Created by marvel on 9/8/24.
//

#pragma once
#include "Surface/OrientedSurfaceComponent.h"

class OrientedSurfaceActor : public Actor
{

public:
	OrientedSurfaceActor(const ObjectPtr<StaticMesh>& DisplayMesh, const ObjectPtr<StaticMesh>& SurfaceMesh)
	{
		MeshComponent = AddComponent<StaticMeshComponent>().get();
		SurfaceComponent = AddComponent<OrientedSurfaceComponent>(SurfaceMesh).get();

		MeshComponent->SetMeshData(DisplayMesh);
	}

	bool IsInside(const FVector& Point) const
	{
		return SurfaceComponent->Inside(GetFTransform().ToLocalSpace(Point));
	}

	double SignedDistance(const FVector& Point) const
	{
		ASSERT(SurfaceComponent != nullptr);
		return SurfaceComponent->SignedDistance(GetFTransform().ToLocalSpace(Point));
	}

	double Distance(const FVector& Point) const
	{
		ASSERT(SurfaceComponent != nullptr);
		return SurfaceComponent->Distance(GetFTransform().ToLocalSpace(Point));
	}

	StaticMeshComponent* GetMeshComponent() const
	{
		return MeshComponent;
	}

protected:
	StaticMeshComponent* MeshComponent;
	OrientedSurfaceComponent* SurfaceComponent;
};

inline auto OrientedSurfaceExample()
{
	return [](World& world)
	{
		auto Mesh = StaticMesh::LoadObj(Path::ProjectContentDir() / "spot.obj");

		auto OrientedSurface = world.SpawnActor<OrientedSurfaceActor>("Surface", Mesh, Mesh);
		OrientedSurface->GetMeshComponent()->GetMeshData()->GetMaterial()->SetAlpha(0.5);


		auto Indicator = world.SpawnActor<StaticMeshActor>("Indicator", BasicShapesLibrary::GenerateSphere(0.02));
		Indicator->GetStaticMeshComponent()->GetMeshData()->GetMaterial()->SetBaseColor(FColor{1., 0, 0});


		world.AddWidget<LambdaUIWidget>([Indicator, OrientedSurface]() {
			if(ImGui::Begin("Orientation Surface Example"))
			{
				double SF = OrientedSurface->SignedDistance(Indicator->GetLocation());
				if(SF < 0)
					ImGui::Text("Inside");
				else
					ImGui::Text("Outside");
				ImGui::Text("Distance: %f", SF);
				ImGui::End();
			}
		});
	};
}