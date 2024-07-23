includes("build_proj.lua")
target("spdlog")
_config_project({
	project_kind = "static",
	batch_size = 64,
	no_rtti = true
})
on_load(function(target)
	local function rela(p)
		return path.relative(path.absolute(p, os.scriptdir()), os.projectdir())
	end
	target:add("includedirs", rela("include"), {
		public = true
	})
	target:add("defines", "SPDLOG_NO_EXCEPTIONS", "SPDLOG_NO_THREAD_ID", "SPDLOG_DISABLE_DEFAULT_LOGGER", "FMT_SHARED", "FMT_CONSTEVAL=constexpr", "FMT_USE_CONSTEXPR=1", "FMT_EXCEPTIONS=0", {
						public = true
					})
	target:add("defines", "FMT_EXPORT", "spdlog_EXPORTS", "SPDLOG_COMPILED_LIB")
end)
add_headerfiles("include/**.h")
add_files("src/*.cpp")
target_end()
