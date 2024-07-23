target("reproc")
_config_project({
    project_kind = "static"
})
add_headerfiles("reproc/include/**.h")
add_headerfiles("reproc++/include/**.hpp")
add_includedirs("reproc/include/", "reproc++/include/", {
    public = true
})
add_rules("c++.build", "c.build")
on_config(function(target)
    local src_path = path.join(os.scriptdir(), "reproc/src")
    local keyword
    if is_plat("windows") then
        keyword = ".windows"
    else
        keyword = ".posix"
    end
    for _, filepath in ipairs(os.files(path.join(src_path, "*.c"))) do
        local file_name = path.filename(filepath)
        file_name = file_name:sub(1, #file_name - 2)
        local ext = path.extension(file_name)
        if (#ext == 0 or ext == keyword) then
            target:add("files", filepath)            
        end
    end
    if is_plat("windows") then
        target:add("links", "ws2_32", {public = true})
    end
end)
add_files("reproc++/src/reproc.cpp")
target_end()
