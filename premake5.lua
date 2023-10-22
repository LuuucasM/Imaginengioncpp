workspace "Imaginengion"
    architecture "x64"
    startproject "MyApp"
    configurations{
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- include directories relative to root folder (solution dir)
IncludeDir = {}
IncludeDir["GLFW"] = "Imaginengion/Vendor/GLFW/include"
IncludeDir["Glad"] = "Imaginengion/Vendor/Glad/include"
IncludeDir["imgui"] = "Imaginengion/Vendor/imgui"

include "Imaginengion/Vendor/GLFW"
include "Imaginengion/Vendor/Glad"
include "Imaginengion/Vendor/imgui"

project "Imaginengion"
    location "Imaginengion"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "impch.h"
    pchsource "Imaginengion/src/impch.cpp"
    
    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs{
        "%{prj.name}/src",
        "%{prj.name}/Vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.imgui}"
    }

    links{
        "GLFW",
        "Glad",
        "Imgui",
        "opengl32.lib",
        "dwmapi.lib"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines{
            "IMAGINE_PLATFORM_WINDOWS",
            "IMAGINE_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        postbuildcommands{
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/MyApp")
        }
    
    filter "configurations:Debug"
        defines "IMAGINE_DEBUG"
        symbols "On"
        staticruntime "off"
        runtime "Debug"

    filter "configurations:Release"
        defines "IMAGINE_RELEASE"
        optimize "On"
        staticruntime "off"
        runtime "Release"
        
    filter "configurations:Dist"
        defines "IMAGINE_DIST"
        optimize "On"
        staticruntime "off"
        runtime "Release"

project "MyApp"
    location "MyApp"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files{
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp"
    }

    includedirs{
        "Imaginengion/Vendor/spdlog/include",
        "Imaginengion/src"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines{
            "IMAGINE_PLATFORM_WINDOWS"
        }
    
    links{
        "Imaginengion"
    }
    
    filter "configurations:Debug"
        defines {
            "IMAGINE_DEBUG",
            "IMAGINE_ENABLE_ASSERTS"
        }
        symbols "On"
    filter "configurations:Release"
        defines {
            "IMAGINE_RELEASE",
            "IMAGINE_ENABLE_ASSERTS"
        }
        optimize "On"
    filter "configurations:Dist"
        defines "IMAGINE_DIST"
        optimize "On"