workspace "Imaginengion"
    architecture "x64"
    startproject "MyApp"
    configurations{
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Imaginengion"
    location "Imaginengion"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs{
        "%{prj.name}/Vendor/spdlog/include/"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines{
            "IMAGINE_PLATFORM_WINDOWS",
            "IMAGINE_BUILD_DLL"
        }

        postbuildcommands{
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/MyApp")
        }
    
    filter "configurations:Debug"
        defines "IMAGINE_DEBUG"
        symbols "On"
    filter "configurations:Release"
        defines "IMAGINE_RELEASE"
        optimize "On"
    filter "configurations:Dist"
        defines "IMAGINE_DIST"
        optimize "On"

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
        defines "IMAGINE_DEBUG"
        symbols "On"
    filter "configurations:Release"
        defines "IMAGINE_RELEASE"
        optimize "On"
    filter "configurations:Dist"
        defines "IMAGINE_DIST"
        optimize "On"