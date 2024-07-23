#include "common/precompiled.h"

#include "generator/reflection_generator.h"

#include "language_types/class.h"
#include "template_manager/template_manager.h"
#include <vector>
#include <map>
#include <set>

#include "language_types/enum.h"

namespace Generator
{
    ReflectionGenerator::ReflectionGenerator(std::string                             source_directory,
                                             std::string                             out_dir,
                                             std::string                             build_tool_dir,
                                             std::function<std::string(std::string)> get_include_function) :
        GeneratorInterface(out_dir + "/_generated/reflection", source_directory, build_tool_dir, get_include_function)
    {
        prepareStatus(m_out_path);
    }
    void ReflectionGenerator::prepareStatus(std::string path)
    {
        GeneratorInterface::prepareStatus(path);
        TemplateManager::getInstance()->loadTemplates(m_build_tool_dir, "commonReflectionFile");
        TemplateManager::getInstance()->loadTemplates(m_build_tool_dir, "allReflectionFile");
        return;
    }

    std::string ReflectionGenerator::processFileName(std::string path)
    {
        auto relativeDir = fs::path(path).filename().replace_extension("reflection.gen.h").string();
        return m_out_path + "/" + relativeDir;
    }

    int ReflectionGenerator::generate(std::string path, SchemaMoudle schema)
    {
        static const std::string vector_prefix = "vector<";
    	static const std::vector<std::string> pointer_prefix =
    		{"shared_ptr<", "unique_ptr<", "weak_ptr<", "*",
    		 "UniquePtr<", "SharedPtr<", "WeakPtr<", "ObjectPtr<"};

        std::string    file_path = processFileName(path);

        Mustache::data mustache_data;
        Mustache::data include_headfiles(Mustache::data::type::list);
        Mustache::data class_defines(Mustache::data::type::list);

        include_headfiles.push_back(
            Mustache::data("headfile_name", Utils::makeRelativePath(m_root_path, path).string()));

        std::map<std::string, bool> class_names;
        // class defs
        for (auto class_temp : schema.classes)
        {
            if (!class_temp->shouldCompile())
                continue;

            class_names.insert_or_assign(class_temp->getClassName(), false);
            class_names[class_temp->getClassName()] = true;

            std::vector<std::string>                                   field_names;
            std::map<std::string, std::pair<std::string, std::string>> vector_map;
        	std::set<std::string> enum_type_names;

            Mustache::data class_def;
            Mustache::data vector_defines(Mustache::data::type::list);
        	Mustache::data enum_defines(Mustache::data::type::list);
        	Mustache::data pointer_defines(Mustache::data::type::list);

            genClassRenderData(class_temp, class_def);
            for (auto field : class_temp->m_fields)
            {
                if (!field->shouldCompile())
                    continue;
                field_names.emplace_back(field->m_name);
                bool is_array = field->m_type.find(vector_prefix) == 0;
                if (is_array)
                {
                    std::string array_useful_name = field->m_type;

                    Utils::formatQualifiedName(array_useful_name);

                    std::string item_type = field->m_type;

                    item_type = Utils::getNameWithoutContainer(item_type);

                    vector_map[field->m_type] = std::make_pair(array_useful_name, item_type);
                }

            	bool is_enum = EnumClass::Get().IsEnumClass(field->m_type);
            	if(is_enum)
				{
					enum_type_names.insert(field->m_type);
				}

            	bool is_pointer = false;
            	for (const auto& prefix : pointer_prefix)
				{
					if (field->m_type.find(prefix) != std::string::npos)
					{
						is_pointer = true;
						break;
					}
				}
            	if (is_pointer)
            	{
            		Mustache::data pointer_define;
            		pointer_define.set("pointer_full_type_name", field->m_type);
            		pointer_define.set("pointer_useful_name", Utils::formatQualifiedName(field->m_type));
            		pointer_defines.push_back(pointer_define);
            	}
            }

            if (!vector_map.empty())
            {
                if (nullptr == class_def.get("vector_exist"))
                {
                    class_def.set("vector_exist", true);
                }
                for (const auto& vector_item : vector_map)
                {
                    std::string    array_useful_name = vector_item.second.first;
                    std::string    item_type         = vector_item.second.second;
                    Mustache::data vector_define;
                    vector_define.set("vector_useful_name", array_useful_name);
                    vector_define.set("vector_type_name", vector_item.first);
                    vector_define.set("vector_element_type_name", item_type);
                    vector_defines.push_back(vector_define);
                }
            }
        	if(!enum_type_names.empty())
        	{
        		if (nullptr == class_def.get("enum_exist"))
        		{
        			class_def.set("enum_exist", true);
        		}
        		for (const auto& enum_type_name : enum_type_names)
				{
					Mustache::data enum_define;
					enum_define.set("enum_type_name", enum_type_name);
					enum_defines.push_back(enum_define);
				}
        	}
        	if (!pointer_defines.is_empty_list())
        	{
        		class_def.set("pointer_exist", true);
        	}
        	class_def.set("pointer_defines", pointer_defines);
            class_def.set("vector_defines", vector_defines);
        	class_def.set("enum_defines", enum_defines);
            class_defines.push_back(class_def);
        }

        mustache_data.set("class_defines", class_defines);
        mustache_data.set("include_headfiles", include_headfiles);

        std::string tmp = Utils::convertNameToUpperCamelCase(fs::path(path).stem().string(), "_");
        mustache_data.set("sourefile_name_upper_camel_case", tmp);

        std::string render_string =
            TemplateManager::getInstance()->renderByTemplate("commonReflectionFile", mustache_data);
        Utils::saveFile(render_string, file_path);
        m_sourcefile_list.insert(tmp);

        m_head_file_list.insert(Utils::makeRelativePath(m_root_path, file_path).string());
        return 0;
    }
    void ReflectionGenerator::finish()
    {
        Mustache::data mustache_data;
        Mustache::data include_headfiles = Mustache::data::type::list;
        Mustache::data sourefile_names    = Mustache::data::type::list;
		for (auto& path : MetaDB::Get().GetAllShouldCompileHeader())
		{
			m_head_file_list.insert(Utils::makeRelativePath(m_root_path, processFileName(path)).string());
			m_sourcefile_list.insert(Utils::convertNameToUpperCamelCase(fs::path(path).stem().string(), "_"));
		}
        for (auto& head_file : m_head_file_list)
        {
            include_headfiles.push_back(Mustache::data("headfile_name", head_file));
        }

        for (auto& sourefile_name_upper_camel_case : m_sourcefile_list)
        {
            sourefile_names.push_back(Mustache::data("sourefile_name_upper_camel_case", sourefile_name_upper_camel_case));
        }
        mustache_data.set("include_headfiles", include_headfiles);
        mustache_data.set("sourefile_names", sourefile_names);
        std::string render_string =
            TemplateManager::getInstance()->renderByTemplate("allReflectionFile", mustache_data);
        Utils::saveFile(render_string, m_out_path + "/all_reflection.h");
    }

    ReflectionGenerator::~ReflectionGenerator() {}
} // namespace Generator