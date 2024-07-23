#include "common/precompiled.h"

#include "parser/parser.h"
#include <regex>
#include "meta_info.h"

MetaInfo::MetaInfo(const Cursor& cursor)
{
	for (auto& child : cursor.getChildren())
	{

		if (child.getKind() != CXCursor_AnnotateAttr)
			continue;

		for (auto& prop : extractProperties(child))
			m_properties.insert(prop);
	}
}

bool MetaInfo::getProperty(const std::string& key) const
{
	return m_properties.find(key) != m_properties.end();
}

bool MetaInfo::getFlag(const std::string& key) const { return m_properties.find(key) != m_properties.end(); }

std::vector<MetaInfo::Property> MetaInfo::extractProperties(const Cursor& cursor)
{
	std::vector<Property> ret_list;

	auto propertyList = cursor.getDisplayName();

	// std::string input = "class0, class1(pro1), class2(pro1, pro2), class3()";
	std::regex	pattern("(\\w+(\\(([^\\)]+)?\\))?)"); // regex pattern to match Classname(property1, property2)
	std::smatch matches;

	// Iterating over matches
	auto it = propertyList.cbegin();
	auto end = propertyList.cend();
	while (std::regex_search(it, end, matches, pattern))
	{
		std::string TagConstructor = matches[1];
		ret_list.push_back(TagConstructor);
		it = matches[0].second; // Update the iterator to search for the next match
	}
	return ret_list;
}

bool MetaInfo::IsReservedTag(const std::string& tag)
{
	for (auto& reserved_tag : NativeProperty::MTags)
	{
		if (tag == reserved_tag)
			return true;
	}
	return false;
}
