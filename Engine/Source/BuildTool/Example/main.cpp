#include <iostream>
#include "Reflection/meta_example.h"
#include "Reflection/reflection/reflection_register.h"

int main()
{
    //IMPORTANT Call this method to register meta info in global scope
    Reflection::TypeMetaRegister::metaRegister();
    std::shared_ptr<Test2> b = std::make_shared<Test2>();
    std::shared_ptr<Object> t = b;
    std::cout << "Class Name : ";
    std::cout << t->GetClassName() << std::endl;

    std::cout << "Base Class Direct : " << std::endl;
    for(auto i : t->GetBaseClassDirect()) {
        std::cout << i << std::endl;
    }
    std::cout << "--------------------" << std::endl;

    std::cout << "Base Class Recursive : " << std::endl;
    for (auto i : t->GetBaseClassRecursive()) {
        std::cout << i << std::endl;
    }
    std::cout << "--------------------" << std::endl;

    std::cout << "All Property : " << std::endl;
    for (auto i : t->GetAllPropertyAceessors()) {
        std::cout << i.getFieldName() << " : " << i.getFieldTypeName() << " in Class : " << std::endl;
    }
    std::cout << "--------------------" << std::endl;

    std::cout << "All Method : " << std::endl;
    for (auto i : t->GetAllMethodAceessors()) {
        std::cout << i.getMethodName() << std::endl;
    }
    t->InvokeFunction("test_void", std::string("HelloWorld!"));
    std::cout << t->InvokeFunction<int>("test_ret", 10, 8); // 18
}