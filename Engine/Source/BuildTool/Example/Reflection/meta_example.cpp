#include "meta_example.h"
#include "_generated/serializer/all_serializer.h"
#include <filesystem>
#include <fstream>
#include <set>
#include <iostream>

Reflection::ReflectionInstance Object::GetMetaInfo()
{
    return GetMetaInfo(GetClassName(), this);
}


Reflection::ReflectionInstance Object::GetMetaInfo(const Class& ClassName, void* Instance)
{
    return Reflection::ReflectionInstance(
        Reflection::TypeMeta::newMetaFromName(ClassName), Instance);
}

std::vector<Object::Class> Object::GetBaseClassDirect()
{
    auto meta = GetMetaInfo();
    Reflection::ReflectionInstance* list;
    int Count = meta.m_meta.getBaseClassReflectionInstanceList(list, this);
    std::vector<Object::Class>  ret;
    for(int i = 0;i < Count;i ++) {
        ret.emplace_back(list[i].m_meta.getTypeName());
    }
    return ret;
}

std::vector<Object::Class> Object::GetBaseClassRecursive()
{
    std::function<std::set<std::string>(Class CurrentClass)> RecursiveBase;
    RecursiveBase = [this, &RecursiveBase] (Object::Class CurrentClass)
    {
        std::set<std::string> ret;
        auto meta = Object::GetMetaInfo(CurrentClass, this);
        Reflection::ReflectionInstance* list;
        int Count = meta.m_meta.getBaseClassReflectionInstanceList(list, this);
        for(int i = 0;i < Count;i ++) {
            ret.insert(list[i].m_meta.getTypeName());
            auto temp = RecursiveBase(list[i].m_meta.getTypeName());
            ret.insert(temp.begin(), temp.end());
        }
        return ret;
    };
    auto Ret = RecursiveBase(GetClassName());
    return {Ret.begin(), Ret.end()};

}

std::vector<Reflection::FieldAccessor> Object::GetAllPropertyAceessors(EAccessorFlag Flag)
{
    std::function<void(Class CurrentClass)> RecursiveBase;
    std::vector<Reflection::FieldAccessor> ret;

    // Get this propery
    auto ThisMeta = Object::GetMetaInfo(GetClassName(), this);
    auto ThisProperty = ThisMeta.m_meta.getFieldsList();
    ret.insert(ret.end(), ThisProperty.begin(), ThisProperty.end());

    if (Flag != ExculdeParent) {
        for(auto ParaentClass : GetBaseClassRecursive()) {
            auto Meta = Object::GetMetaInfo(ParaentClass, this);
            auto Property = Meta.m_meta.getFieldsList();
            ret.insert(ret.end(), Property.begin(), Property.end());
        }
    }
    return ret;
}

std::vector<Reflection::MethodAccessor> Object::GetAllMethodAceessors(EAccessorFlag Flag)
{
    std::function<void(Class CurrentClass)> RecursiveBase;
    std::vector<Reflection::MethodAccessor> ret;

    // Get this propery
    auto ThisMeta = Object::GetMetaInfo(GetClassName(), this);
    auto ThisMethod = ThisMeta.m_meta.getMethodsList();
    ret.insert(ret.end(), ThisMethod.begin(), ThisMethod.end());

    if (Flag != ExculdeParent) {
        for(auto ParaentClass : GetBaseClassRecursive()) {
            auto Meta = Object::GetMetaInfo(ParaentClass, this);
            auto Property = Meta.m_meta.getMethodsList();
            ret.insert(ret.end(), Property.begin(), Property.end());
        }
    }
    return ret;
}

void metaExample()
{
    Test1 test1_in;
    test1_in.m_int  = 12;
    test1_in.m_char = 'g';
    int i           = 1;
    test1_in.m_int_vector.emplace_back(&i);

    Test1 test1_out;
    // test on array
    Test2 test2_in;
    test2_in.m_test_base_array.emplace_back("Test1", &test1_in);
    Test1 Test2_temp;
    test2_in.m_test_base_array.emplace_back("Test1", &Test2_temp);

    // serializer & deserializer

    // write Test1_in (object) to Test1_json_in (json)
    auto test1_json_in = Serializer::write(test1_in);

    std::string test1_context = test1_json_in.dump();

    // read Test1_context (json) to Test1_out (object)
    std::string err;

    auto&& Test1_json = Json::parse(test1_context, err);
    Serializer::read(Test1_json, test1_out);

    auto        Test2_json_in = Serializer::write(test2_in);
    std::string test2_context = Test2_json_in.dump();

    std::fstream out_put("out.txt", std::ios::out);
    out_put << test2_context;
    out_put.flush();
    out_put.close();

    Test2  test2_out;
    auto&& test2_json = Json::parse(test2_context, err);
    Serializer::read(test2_json, test2_out);

    // reflection
    auto                       meta = TypeMetaDef(Test2, &test2_out);
    auto fields = meta.m_meta.getFieldsList();
    for (int i = 0; i < fields.size(); ++i)
    {
        auto filed_accesser = fields[i];
        std::cout << filed_accesser.getFieldTypeName() << " " << filed_accesser.getFieldName() << " "
                  << (char*)filed_accesser.get(meta.m_instance) << std::endl;
        if (filed_accesser.isArrayType())
        {
            Reflection::ArrayAccessor array_accesser;
            if (Reflection::TypeMeta::newArrayAccessorFromName(filed_accesser.getFieldTypeName(), array_accesser))
            {
                void* field_instance = filed_accesser.get(meta.m_instance);
                int   count          = array_accesser.getSize(field_instance);
                auto  typeMetaItem   = Reflection::TypeMeta::newMetaFromName(array_accesser.getElementTypeName());
                for (int index = 0; index < count; ++index)
                {
                    std::cout << ":L:" << index << ":R:" << (int*)array_accesser.get(index, field_instance)
                              << std::endl;
                    ;
                }
            }
        }
    }
}
