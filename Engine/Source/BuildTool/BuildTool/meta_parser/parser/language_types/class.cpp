#include "common/precompiled.h"

#include "class.h"
#include "meta/meta_data_config.h"
#include <algorithm>

std::map<std::string, std::vector<std::string>> Class::BaseClassMap;

BaseClass::BaseClass(const Cursor& cursor) : name(Utils::getTypeNameWithoutNamespace(cursor.getType())) {}

Class::Class(const Cursor& cursor, const Namespace& current_namespace) :
    TypeInfo(cursor, current_namespace), m_name(cursor.getDisplayName()),
    m_qualified_name(Utils::getTypeNameWithoutNamespace(cursor.getType())),
    m_display_name(Utils::getNameWithoutFirstM(m_qualified_name))
{
	bIsClass = (cursor.getKind() == CXCursor_ClassDecl);
    Utils::replaceAll(m_name, " ", "");
    std::vector<std::string> BaseNames;
    for (auto& child : cursor.getChildren())
    {
        switch (child.getKind())
        {
            case CXCursor_CXXBaseSpecifier: {
                auto base_class = new BaseClass(child);
                m_base_classes.emplace_back(base_class);
                BaseNames.emplace_back(base_class->name);
                // For Object, we ignore all base class, for user may inherit from shared_from this
                if(m_name == "Object") {
                    m_base_classes.clear();
                    BaseNames.clear();
                }
            }
            break;
            // field
            case CXCursor_FieldDecl:
                m_fields.emplace_back(new Field(child, current_namespace, this));
                break;
            // method
            case CXCursor_CXXMethod:
                m_methods.emplace_back(new Method(child, current_namespace, this));
            default:
                break;
        }
    }
    BaseClassMap[m_name] = BaseNames;
}

bool ContainBase(std::string ClassName, std::string BaseName) 
{
    if(Class::BaseClassMap.count(ClassName) == 0) return false;
    for(auto i : Class::BaseClassMap[ClassName]){
        if(i == BaseName) return true;
        if(ContainBase(i, BaseName)) return true;
    }
    return false;
}

bool Class::IsDerivedFrom(std::string BaseName) const
{
    return ContainBase(m_name, BaseName);
}

void Class::TrySetClassTag(class_tag&& InTag)
{
    if (!InTag.Valid) return;
    if (m_name != InTag.GetClassName()) return;
    if (InTag.SourceFile != getSourceFile()) return;
    if (InTag.SourceLine > getSourceLine()) return;
    Tag = InTag;
}

// With MCLASS tag and derive from Object
bool Class::shouldCompile(void) const 
{ 
    if(m_name == "Object") return true;
	// Is class, derived from Object, has MCLASS tag
    if(bIsClass && IsDerivedFrom("Object")
    	&& Tag.getFlag(NativeProperty::MCLASS))
    	return true;
	// Is struct, has MSTRUCT tag
	if(!bIsClass && Tag.getFlag(NativeProperty::MSTRUCT))
		return true;

    return false;
}

bool Class::shouldCompileFields(void) const
{
    for(auto Filed : m_fields)
    {
        if(Filed->shouldCompile())
            return true;
    }
    return false;
}

bool Class::shouldCompileMethods(void) const
{
    for(auto Method : m_methods)
    {
        if(Method->shouldCompile())
            return true;
    }
    return false;
}

std::string Class::getClassName(void) { return m_name; }

bool Class::isAccessible(void) const { return m_enabled; }