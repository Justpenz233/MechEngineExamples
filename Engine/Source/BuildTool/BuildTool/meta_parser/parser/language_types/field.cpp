#include "common/precompiled.h"

#include "class.h"
#include "field.h"
#include "meta/meta_data_config.h"

Field::Field(const Cursor& cursor, const Namespace& current_namespace, Class* parent) :
    TypeInfo(cursor, current_namespace), m_is_const(cursor.getType().IsConst()), m_parent(parent),
    m_name(cursor.getSpelling()), m_display_name(Utils::getNameWithoutFirstM(m_name)),
    m_type(Utils::getTypeNameWithoutNamespace(cursor.getType()))
{
	Utils::replaceAll(m_type, "class ", "");
}

bool Field::shouldCompile(void) const { return isAccessible(); }

bool Field::isAccessible(void) const
{
    return m_meta_data.getFlag(NativeProperty::MPROPERTY);
}
