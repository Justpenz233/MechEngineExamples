#include "common/MetaDB.h"
#include "common/precompiled.h"

#include "language_types/class.h"
#include "language_types/enum.h"
#include "generator/reflection_generator.h"
#include "generator/serializer_generator.h"

#include "parser.h"

#include <utility>

#include "language_types/class_tag.h"

#define RECURSE_NAMESPACES(kind, cursor, method, namespaces) \
    { \
        if (kind == CXCursor_Namespace) \
        { \
            auto display_name = cursor.getDisplayName(); \
            if (!display_name.empty()) \
            { \
                namespaces.emplace_back(display_name); \
                method(cursor, namespaces); \
                namespaces.pop_back(); \
            } \
        } \
    }

#define TRY_ADD_LANGUAGE_TYPE(handle, container) \
    { \
        if (handle->shouldCompile()) \
        { \
            auto file = handle->getSourceFile(); \
            m_schema_modules[file].container.emplace_back(handle); \
            m_type_table[handle->m_display_name] = file; \
        } \
    }

void MetaParser::prepare(void) {}

std::string MetaParser::getIncludeFile(std::string name)
{
    auto iter = m_type_table.find(name);
    return iter == m_type_table.end() ? std::string() : iter->second;
}

MetaParser::MetaParser(const std::string project_input_file,
                       const std::string include_file_path,
                       const std::string include_path,
                       const std::string sys_include,
                       const std::string module_name,
                       const std::string build_tool_dir,
                       const std::string out_dir,
                       bool              is_show_errors) :
        m_project_input_file(project_input_file),
        m_source_include_file_name(include_file_path), m_index(nullptr), m_translation_unit(nullptr),
        m_sys_include(sys_include), m_module_name(module_name), m_is_show_errors(is_show_errors), m_build_tool_dir(build_tool_dir), m_out_dir(out_dir)
{
    m_work_paths = Utils::split(include_path, ",");
    for (auto& work_path : m_work_paths)
	    work_path = std::filesystem::path(work_path).lexically_normal().string();
	std::cout << "Work Paths: " << m_work_paths[0] << std::endl;
    MetaDB::InitSingleton(out_dir);
    m_generators.emplace_back(new Generator::SerializerGenerator(
            m_work_paths[0], out_dir, build_tool_dir, std::bind(&MetaParser::getIncludeFile, this, std::placeholders::_1)));
    m_generators.emplace_back(new Generator::ReflectionGenerator(
            m_work_paths[0], out_dir, build_tool_dir, std::bind(&MetaParser::getIncludeFile, this, std::placeholders::_1)));
}

MetaParser::~MetaParser(void)
{
    for (auto item : m_generators)
    {
        delete item;
    }
    m_generators.clear();

    if (m_translation_unit)
        clang_disposeTranslationUnit(m_translation_unit);

    if (m_index)
        clang_disposeIndex(m_index);
}

void MetaParser::finish(void)
{
    for (auto generator_iter : m_generators)
    {
        generator_iter->finish();
    }
}

MetaParser::ParseStatus MetaParser::parseProject()
{
    std::cout << "Parsing project file: " << m_project_input_file << std::endl;

    std::fstream include_txt_file(m_project_input_file, std::ios::in);

    if (include_txt_file.fail())
    {
        std::cout << "Could not load file: " << m_project_input_file << std::endl;
        return ParseStatus::Failed;
    }

    std::stringstream buffer;
    buffer << include_txt_file.rdbuf();

    std::string context = buffer.str();

    auto         inlcude_files = Utils::split(context, ";");
    std::fstream include_file;

    include_file.open(m_source_include_file_name, std::ios::out);
    if (!include_file.is_open())
    {
        std::cout << "Could not open the Source Include file: " << m_source_include_file_name << std::endl;
        return ParseStatus::Failed;
    }

    std::cout << "Generating the Source Include file: " << m_source_include_file_name << std::endl;

    std::string output_filename = Utils::getFileName(m_source_include_file_name);

    if (output_filename.empty())
    {
        output_filename = "META_INPUT_HEADER_H";
    }
    else
    {
        Utils::replace(output_filename, ".", "_");
        Utils::replace(output_filename, " ", "_");
        Utils::toUpper(output_filename);
    }
    include_file << "#ifndef __" << output_filename << "__" << std::endl;
    include_file << "#define __" << output_filename << "__" << std::endl;

	std::vector<std::string> AllIncludeFiles;
	bool IsUpdated = false;
    for (auto include_item : inlcude_files)
    {
        std::string temp_string(include_item);
        Utils::replace(temp_string, '\\', '/');
        // Remove endl
        temp_string.erase(std::remove(temp_string.begin(), temp_string.end(), '\n'), temp_string.end());

    	// Filter out generated files
        if(temp_string.find("_generated") != std::string::npos) continue;

    	// Filter out should compile files
		if(!Utils::ShouldCompileFile(temp_string)) continue;

        std::string IncludeFileCode ="#include  \"" + temp_string + "\"\n";

        if( MetaDB::Get().IsUpdated(temp_string) )
        {
        	include_file << IncludeFileCode;
        	IsUpdated = true;
        }
    }

    include_file << "#endif" << std::endl;
    include_file.close();
	if (IsUpdated)
		return Successful;
	else
		return NotUpdated;
}

void printDiagnostics(CXTranslationUnit translationUnit) {
    int nbDiag = clang_getNumDiagnostics(translationUnit);
    printf("There are %i diagnostics:\n",nbDiag);

    bool foundError = false;
    for (unsigned int currentDiag = 0; currentDiag < nbDiag; ++currentDiag) {
        CXDiagnostic diagnotic = clang_getDiagnostic(translationUnit, currentDiag);
        CXString errorString = clang_formatDiagnostic(diagnotic,clang_defaultDiagnosticDisplayOptions());
        std::string tmp{clang_getCString(errorString)};
        clang_disposeString(errorString);
        if (tmp.find("error:") != std::string::npos) {
            foundError = true;
        }
        std::cerr << tmp << std::endl;
    }
    if (foundError) {
        std::cerr << "Please resolve these issues and try again." <<std::endl;
    }
}

int MetaParser::parse(void)
{
    auto parse_include_ = parseProject();
    if (parse_include_ == Failed)
    {
        std::cerr << "Parsing project file error! " << std::endl;
        return -1;
    }
	// if (parse_include_ == NotUpdated)
	// {
	// 	std::cout << "No file is updated, skip parsing" << std::endl;
	// 	return 1;
	// }

    std::cerr << "Parsing the whole project..." << std::endl;
    int is_show_errors      = m_is_show_errors ? 1 : 0;
    m_index                 = clang_createIndex(true, is_show_errors);
    std::string pre_include = "-I";
    std::string sys_include_temp;
    if (!(m_sys_include == "*"))
    {
        sys_include_temp = pre_include + m_sys_include;
        arguments.emplace_back(sys_include_temp.c_str());
    }

    auto paths = m_work_paths;
    for (int index = 0; index < paths.size(); ++index)
    {
        paths[index] = pre_include + paths[index];

        arguments.emplace_back(paths[index].c_str());
    }

    fs::path input_path(m_source_include_file_name);
    if (!fs::exists(input_path))
    {
        std::cerr << input_path << " is not exist" << std::endl;
        return -2;
    }

    m_translation_unit = clang_createTranslationUnitFromSourceFile(
            m_index, m_source_include_file_name.c_str(), static_cast<int>(arguments.size()), arguments.data(), 0, nullptr);

//    printDiagnostics(m_translation_unit);

    auto cursor = clang_getTranslationUnitCursor(m_translation_unit);

    Namespace temp_namespace;

    buildClassAST(cursor, temp_namespace);

    temp_namespace.clear();

    return 0;
}

void MetaParser::generateFiles(void)
{
    std::cerr << "Start generate runtime schemas( Size=" << m_schema_modules.size() << ")..." << std::endl;
    bool IsUpdated = false;
    for (auto& schema : m_schema_modules)
    {
        for (auto& generator_iter : m_generators)
        {
            IsUpdated = true;
            // std::cout << "Genreate : " << schema.first << std::endl;
            generator_iter->generate(schema.first, schema.second);
        }
    }

    if(IsUpdated)
        finish();
}

void MetaParser::buildClassAST(const Cursor& cursor, Namespace& current_namespace)
{
    static class_tag LastTagVar;
    for (auto& child : cursor.getChildren())
    {
        auto kind = child.getKind();
        // actual definition and a class or struct
        if(child.isDefinition() && kind == CXCursor_VarDecl)
        {
            LastTagVar = class_tag(child);
        }
    	else if(child.isDefinition() && kind == CXCursor_EnumDecl)
    	{
    		EnumClass::Get().RegisterEnumClass(child.getDisplayName());
    	}
    	else if (child.isDefinition() && (kind == CXCursor_ClassDecl || kind == CXCursor_StructDecl))
        {
            auto class_ptr = std::make_shared<Class>(child, current_namespace);
            class_ptr->TrySetClassTag(std::move(LastTagVar));
            TRY_ADD_LANGUAGE_TYPE(class_ptr, classes);
        	if(class_ptr->shouldCompile())
        		MetaDB::Get().RegisterClass(class_ptr->m_name, class_ptr->getSourceFile());
        }
        else
        {
            RECURSE_NAMESPACES(kind, child, buildClassAST, current_namespace);
        }
    }
}
