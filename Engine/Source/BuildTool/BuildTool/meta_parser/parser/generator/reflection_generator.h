#pragma once
#include "generator/generator.h"
namespace Generator
{
    class ReflectionGenerator : public GeneratorInterface
    {
    public:
        ReflectionGenerator() = delete;
        ReflectionGenerator(std::string source_directory, std::string out_dir, std::string build_tool_dir, std::function<std::string(std::string)> get_include_function);
        virtual int  generate(std::string path, SchemaMoudle schema) override;
        virtual void finish() override;
        virtual ~ReflectionGenerator() override;

    protected:
        virtual void        prepareStatus(std::string path) override;
        virtual std::string processFileName(std::string path) override;

    private:
        std::set<std::string> m_head_file_list;
        std::set<std::string> m_sourcefile_list;
    };
} // namespace Generator
