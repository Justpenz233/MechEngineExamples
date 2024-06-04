//
// Created by marvelli on 6/2/24.
//

#pragma once
#include "Log/Log.h"
#include "Object/Object.h"
#include "Game/World.h"
MCLASS(TestMetadataClass)
class TestMetadataClass : public Object
{
	REFLECTION_BODY(TestMetadataClass)
	TestMetadataClass() = default;

protected:
	MPROPERTY()
	int TestInt = 0;

	MPROPERTY(Drag_(0.f, 1.f), Category_("Test"))
	float TestFloat = 0.0f;
};

inline auto MetaDataExample()
{
	return [](::World& World)
	{
		auto Object = NewObject<TestMetadataClass>();

		for(auto Property : Object->GetAllPropertyAceessors())
		{
			MechEngine::LOG_TEMP("Property type:{} name:{}", Property.getTypeName(), Property.getFieldName());
		}
	};
};