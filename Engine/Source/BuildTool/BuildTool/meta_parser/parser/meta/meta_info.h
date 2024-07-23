#pragma once

#include "cursor/cursor.h"

class MetaInfo
{
public:
    MetaInfo(const Cursor& cursor);

    bool getProperty(const std::string& key) const;

    bool getFlag(const std::string& key) const;

	std::unordered_set<std::string> getProperties() const { return m_properties; }

	static bool IsReservedTag(const std::string& tag);
private:
    typedef std::string Property;

    std::unordered_set<std::string> m_properties;

private:
	static std::vector<Property> extractProperties(const Cursor& cursor);

};