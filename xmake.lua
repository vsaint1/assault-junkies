add_rules("mode.debug", "mode.release")

add_requires("minhook","imgui","spdlog",{config = {opengl32 = true,win32  =true}})

set_languages("c++20", "c11")

local base_dir = "$(projectdir)"

add_rules("plugin.compile_commands.autoupdate", { outputdir = ".vscode" })

target("ac_trainer")
    set_kind("shared")
    set_arch("x86")
    add_defines("_WIN32")   
    add_files("src/*.cpp","src/**/*.cpp")
    add_links("user32","opengl32")
    add_packages("minhook","imgui","spdlog")