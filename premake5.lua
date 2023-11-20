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
IncludeDir["Imgui"] = "Imaginengion/Vendor/Imgui"
IncludeDir["glm"] = "Imaginengion/Vendor/glm"

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
        "%{prj.name}/src/**.cpp"
    }

    includedirs{
        "%{prj.name}/src",
        "%{prj.name}/Vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.Imgui}",
        "%{IncludeDir.glm}"
    }

    links{
        "GLFW",
        "Glad",
        "Imgui",
        "opengl32.lib",
        "dwmapi.lib"
    }
    filter "system:windows"

        staticruntime "On"
        systemversion "latest"

        defines{
            "IMAGINE_PLATFORM_WINDOWS",
            "IMAGINE_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
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
        "%{IncludeDir.glm}"
    }

    links{
        "Imaginengion"
    }

    filter "system:windows"
        systemversion "latest"
        defines{
            "IMAGINE_PLATFORM_WINDOWS"
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