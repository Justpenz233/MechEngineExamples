//
// Created by MarvelLi on 2024/4/7.
//

#pragma once
#include <set>

class EnumClass
{
public:
	static EnumClass& Get()
	{
		static EnumClass instance;
		return instance;
	}

	void RegisterEnumClass(const std::string& enum_class_name)
	{
		enum_class_names.insert(enum_class_name);
	}

	bool IsEnumClass(const std::string& enum_class_name) const
	{
		return enum_class_names.count(enum_class_name) > 0;
	}

	std::set<std::string> enum_class_names;
};
