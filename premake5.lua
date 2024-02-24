workspace "Imaginengion"
    architecture "x86_64"
    startproject "MyApp"
    configurations{
        "Debug",
        "Release",
        "Dist"
    }
    flags{
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- include directories relative to root folder (solution dir)
IncludeDir = {}
IncludeDir["GLFW"] = "Imaginengion/Vendor/GLFW/include"
IncludeDir["Glad"] = "Imaginengion/Vendor/Glad/include"
IncludeDir["Imgui"] = "Imaginengion/Vendor/Imgui"
IncludeDir["glm"] = "Imaginengion/Vendor/glm"
IncludeDir["stb"] = "Imaginengion/Vendor/stb"
IncludeDir["yaml_cpp"] = "Imaginengion/Vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "Imaginengion/Vendor/ImGuizmo"

include "Imaginengion/Vendor"

project "Imaginengion"
    location "Imaginengion"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "impch.h"
    pchsource "Imaginengion/src/impch.cpp"
    
    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/Vendor/stb/**.h",
        "%{prj.name}/Vendor/stb/**.cpp",
        "%{prj.name}/Vendor/glm/glm/**.hpp",
        "%{prj.name}/Vendor/glm/glm/**.ini",
        "%{prj.name}/Vendor/ImGuizmo/ImGuizmo.h",
        "%{prj.name}/Vendor/ImGuizmo/ImGuizmo.cpp"
    }

    includedirs{
        "%{prj.name}/src",
        "%{prj.name}/Vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.Imgui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.yaml_cpp}",
        "%{IncludeDir.ImGuizmo}"
    }

    links{
        "GLFW",
        "Glad",
        "Imgui",
        "opengl32.lib",
        "dwmapi.lib",
        "yaml-cpp"
    }

    filter "files:Imaginengion/Vendor/ImGuizmo/ImGuizmo.cpp"
        flags {"NoPCH"}

    filter "system:windows"

        staticruntime "On"
        systemversion "latest"

        defines{
            "IMAGINE_BUILD_DLL",
            "GLFW_INCLUDE_NONE",
            "YAML_CPP_STATIC_DEFINE"
        }
    
    filter "configurations:Debug"
        defines "IMAGINE_DEBUG"
        symbols "on"
        runtime "Debug"

    filter "configurations:Release"
        defines "IMAGINE_RELEASE"
        optimize "on"
        runtime "Release"
        
    filter "configurations:Dist"
        defines "IMAGINE_DIST"
        optimize "on"
        runtime "Release"

project "MyApp"
    location "MyApp"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files{
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp"
    }

    includedirs{
        "Imaginengion/Vendor/spdlog/include",
        "Imaginengion/src",
        "Imaginengion/Vendor",
        "%{IncludeDir.glm}",
    }

    links{
        "Imaginengion"
    }

    filter "system:windows"
        systemversion "latest"
    
    filter "configurations:Debug"
        defines "IMAGINE_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "IMAGINE_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "IMAGINE_DIST"
        optimize "On"

project "SangSang"
    location "SangSang"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files{
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp"
    }

    includedirs{
        "Imaginengion/Vendor/spdlog/include",
        "Imaginengion/src",
        "Imaginengion/Vendor",
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGuizmo}"
    }

    links{
        "Imaginengion"
    }

    filter "system:windows"
        systemversion "latest"
    
    filter "configurations:Debug"
        defines "IMAGINE_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "IMAGINE_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "IMAGINE_DIST"
        optimize "On"project "MyApp"
        location "MyApp"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++20"
        staticruntime "on"
    
        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
        files{
            "%{prj.name}/**.h",
            "%{prj.name}/**.cpp"
        }
    
        includedirs{
            "Imaginengion/Vendor/spdlog/include",
            "Imaginengion/src",
            "Imaginengion/Vendor",
            "%{IncludeDir.glm}"
        }
    
        links{
            "Imaginengion"
        }
    
        filter "system:windows"
            systemversion "latest"
        
        filter "configurations:Debug"
            defines "IMAGINE_DEBUG"
            symbols "On"
    
        filter "configurations:Release"
            defines "IMAGINE_RELEASE"
            optimize "On"
    
        filter "configurations:Dist"
            defines "IMAGINE_DIST"
            optimize "On"