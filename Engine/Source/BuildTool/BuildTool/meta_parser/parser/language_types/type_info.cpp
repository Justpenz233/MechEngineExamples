#include "common/precompiled.h"

#include "type_info.h"

TypeInfo::TypeInfo(const Cursor& cursor, const Namespace& current_namespace) :
    m_meta_data(cursor), m_enabled(true), m_root_cursor(cursor),
    m_namespace(current_namespace)
{}

const MetaInfo& TypeInfo::getMetaData(void) const { return m_meta_data; }

std::string TypeInfo::getSourceFile(void) const { return m_root_cursor.getSourceFile(); }
int TypeInfo::getSourceLine(void) const { return m_root_cursor.getSourceLine(); }

Namespace TypeInfo::getCurrentNamespace() const { return m_namespace; }

Cursor& TypeInfo::getCurosr() { return m_root_cursor; }
