#include <filesystem>
#include "template_manager.h"

void TemplateManager::loadTemplates(std::string path, std::string template_name)
{
    std::filesystem::path BasePath(path);
    BasePath = BasePath / ("meta_parser/template/" + template_name + ".mustache");
    //TODO replace to passing in parameter
    std::string template_file = Utils::loadFile(BasePath.string());

    if(template_file == "")
        std::cerr << "Load template file error, error path:" << BasePath.string() << std::endl;
    m_template_pool.insert_or_assign(template_name,
                                     template_file);
}

std::string TemplateManager::renderByTemplate(std::string template_name, Mustache::data& template_data)
{
    if (m_template_pool.end() == m_template_pool.find(template_name))
    {
        return "";
    }
    Mustache::mustache tmpl(m_template_pool[template_name]);
    return tmpl.render(template_data);
}
