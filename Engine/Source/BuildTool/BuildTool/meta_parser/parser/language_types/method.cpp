#include "common/precompiled.h"

#include "class.h"
#include "meta/meta_data_config.h"
#include "method.h"

Method::Method(const Cursor& cursor, const Namespace& current_namespace, Class* parent) :
    TypeInfo(cursor, current_namespace), m_parent(parent), m_name(cursor.getSpelling())
{}

bool Method::shouldCompile(void) const { return isAccessible(); }

bool Method::isAccessible(void) const
{
    return m_meta_data.getFlag(NativeProperty::MFUNCTION);
}
