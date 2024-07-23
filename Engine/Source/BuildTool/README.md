# Yet another CPP reflection lib
## Reference
- [Piccolo Engine](https://github.com/BoomingTech/Piccolo/tree/main/engine/source/meta_parser)
- [CPP Reflection](https://github.com/AustinBrunkHorst/CPP-Reflection)

## Install

We only provide cmake usage for now.   
Support all platforms including MSVC(Windows) and Clang-Arm(MacOS).

```cmake
# First add the subdirectory 
add_subdirectory(BuildTool)

# Then use the following function to register your target for parsing

# --------- Reflection setup ------------
# IMPORTANT: Set PARSE_HEADERS property for precompiler to read, this var should point to all your headers
set_target_properties(TestLib PROPERTIES PARSE_HEADERS "${HEADER_FILES}")
# Add library to precompile
# The first parameter is the path which you want to place the generated code
# Usually this path is the same as your target's source directory in your source tree 
PreCompile("${PATH_TO_PLACE_GENERATED_CODE}" TestLib)
# !
# If you have multi target needs to be parsed, call with a list
# set(PrecompileList TestLib1 TestLib2)
# PreCompile("${PATH_TO_PLACE_GENERATED_CODE}" "${PrecompileList}")
# ---------------------------------------

```

## Usage
```CPP
MCLASS(BaseTest)
class BaseTest : public Object
{
    REFLECTION_BODY(BaseTest);

 public:
        MPROPERTY()
        int m_int;
        
        MPROPERTY()
        std::vector<int*> m_int_vector;
        
        MFUNCTION()
        void test();
};
```
If you are familiar with Unreal Engine, you will find that the usage of this lib is very similar to Unreal Engine's reflection system. You can use MCLASS, MPROPERTY, MFUNCTION to declare a class, property, function. And you can use REFLECTION_BODY to implement the reflection system.

NOTE:
> * All refelction class should inherit from class named Object. 
> * We would use this common base class as the pointer type. 
> * All reflection class should contain at least one MPROPERTY 

## How did it work

We use libclang, a C++ parser, to parse the source code and generate template code to register the class, property, function.  
About the tag MCLASS, is a tricky implementation by string processing.

## TODO
- [🔜] A base object class , provide interfaces to access meta info
- [🚧] Support object pointer when serializing and deserializing
- [🚧] Better error handling and reporting when parsing source code
- [🚧] Doc for easy tag extension and implementation