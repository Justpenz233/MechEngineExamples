//
// Created by marvelli on 6/2/24.
//

#pragma once
#include "Log/Log.h"
#include "Object/Object.h"
#include "Game/World.h"
#include "Game/StaticMeshActor.h"
#include "Mesh/BasicShapesLibrary.h"

MCLASS(MyCylinder)
class MyCylinder : public StaticMeshActor
{
	REFLECTION_BODY(MyCylinder)
	MyCylinder() = default;

public:
	// Init the cylinder
	virtual void Init() override
	{
		StaticMeshActor::Init();
		GetStaticMeshComponent()->SetMeshData(BasicShapesLibrary::GenerateCylinder(Length, Radius, Samples));
	}

protected:
	MPROPERTY(Slide_(0.1f, 1.f), Category_("MyProperty"))
	float Radius = 0.3f;

	MPROPERTY(Step_(0.5f, 2.f), Category_("MyProperty"))
	double Length = 1.f;

	MPROPERTY(Slide_(4, 64), Category_("MyProperty"))
	int Samples = 8;

	// When the property is edited, update the mesh data
	virtual void PostEdit(Reflection::FieldAccessor& Field) override
	{
		if (Field == NAME(Radius) || Field == NAME(Length) || Field == NAME(Samples))
			GetStaticMeshComponent()->SetMeshData(BasicShapesLibrary::GenerateCylinder(Length, Radius, Samples));
	}
};

inline auto MetaDataExample()
{
	return [](World& World)
	{
		auto Object = World.SpawnActor<MyCylinder>("MyCylinder");

		// Iterate all properties
		for(auto Property : Object->GetAllPropertyAceessors())
		{
			MechEngine::LOG_TEMP("Property type:{} name:{}", Property.getTypeName(), Property.getFieldName());
			for (auto Tag : Property.GetPropertyTags())
			{
				if (auto Slide = dynamic_cast<const Slide_*>(Tag))
				{
					MechEngine::LOG_TEMP("Drag tag, min:{} max:{}", Slide->GetMin(), Slide->GetMax());
				}
				if (auto Category = dynamic_cast<const Category_*>(Tag))
				{
					MechEngine::LOG_TEMP("Category tag, category:{}", Category->ParseCategory()[0]);
				}
			}
		}
	};
};