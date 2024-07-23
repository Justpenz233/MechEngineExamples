#pragma once

#include "class_tag.h"
#include "cursor/cursor.h"
#include "type_info.h"

#include "field.h"
#include "method.h"

struct BaseClass
{
    BaseClass(const Cursor& cursor);

    std::string name;
};

class Class : public TypeInfo
{
    // to access m_qualifiedName
    friend class Field;
    friend class Method;
    friend class MetaParser;

public:
    Class(const Cursor& cursor, const Namespace& current_namespace);

    // Can obtain the classes inherit tree
    static std::map<std::string, std::vector<std::string>> BaseClassMap;

    virtual bool shouldCompile(void) const;

    bool shouldCompileFields(void) const;
    bool shouldCompileMethods(void) const;
    bool IsDerivedFrom(std::string BaseName) const;
    void TrySetClassTag(class_tag&& InTag);

    template<typename T>
    using SharedPtrVector = std::vector<std::shared_ptr<T>>;

    std::string getClassName(void);

    SharedPtrVector<BaseClass> m_base_classes;

public:
	bool bIsClass; // Class or struct

    std::string m_name;

    std::string m_qualified_name;

    SharedPtrVector<Field> m_fields;
    SharedPtrVector<Method> m_methods;

    class_tag Tag;

    std::string m_display_name;

    bool isAccessible(void) const;


};
