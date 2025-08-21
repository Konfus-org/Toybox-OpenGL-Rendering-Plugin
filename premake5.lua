project "OpenGL Rendering"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "Off"

    RegisterDynamicPlugin("OpenGL Rendering")

    files
    {
        "./**.hpp",
        "./**.cpp",
        "./**.h",
        "./**.c",
        "./**.md",
        "./**.plugin"
    }
    includedirs
    {
        "./Source",
        _MAIN_SCRIPT_DIR .. "/Dependencies/glad/include"
    }
    links
    {
        "glad"
    }
