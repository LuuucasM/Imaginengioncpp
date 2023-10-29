--[[=====================================
------------------IMGUI-----------------
=========================================]]--
project "Imgui"
	location "Imgui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/imconfig.h",
		"%{prj.name}/imgui.h",
		"%{prj.name}/imgui.cpp",
		"%{prj.name}/imgui_draw.cpp",
		"%{prj.name}/imgui_internal.h",
		"%{prj.name}/imgui_widgets.cpp",
		"%{prj.name}/imstb_rectpack.h",
		"%{prj.name}/imstb_textedit.h",
		"%{prj.name}/imstb_truetype.h",
		"%{prj.name}/imgui_demo.cpp"
	}

	filter "system:windows"
		systemversion "latest"
		
	filter "system:linux"
		pic "on"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
        runtime "Release"
		optimize "on"
--[[=====================================
------------------GLFW-----------------
=========================================]]--
project "GLFW"
    location "GLFW"
    kind "StaticLib"
    language "C"
	staticruntime "on"


    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    targetdir("bin-int/" .. outputdir .. "/%{prj.name}")

    files{
		"%{prj.name}/include/GLFW/glfw3.h",
		"%{prj.name}/include/GLFW/glfw3native.h",
		"%{prj.name}/src/glfw_config.h",
		"%{prj.name}/src/context.c",
		"%{prj.name}/src/init.c",
		"%{prj.name}/src/input.c",
		"%{prj.name}/src/monitor.c",
		"%{prj.name}/src/vulkan.c",
		"%{prj.name}/src/window.c",
		"%{prj.name}/src/platform.c",
		"%{prj.name}/src/null_init.c",
		"%{prj.name}/src/null_monitor.c",
		"%{prj.name}/src/null_window.c",
		"%{prj.name}/src/null_joystick.c"
    }
    filter "system:linux"
        pic "On"
        systemversion "latest"

        files
		{
			"%{prj.name}/src/x11_init.c",
			"%{prj.name}/src/x11_monitor.c",
			"%{prj.name}/src/x11_window.c",
			"%{prj.name}/src/xkb_unicode.c",
			"%{prj.name}/src/posix_time.c",
			"%{prj.name}/src/posix_thread.c",
			"%{prj.name}/src/glx_context.c",
			"%{prj.name}/src/egl_context.c",
			"%{prj.name}/src/osmesa_context.c",
			"%{prj.name}/src/linux_joystick.c"
		}
        
		defines
		{
			"_GLFW_X11"
		}

    filter "system:windows"
		systemversion "latest"

		files
		{
			"%{prj.name}/src/win32_init.c",
			"%{prj.name}/src/win32_joystick.c",
			"%{prj.name}/src/win32_monitor.c",
			"%{prj.name}/src/win32_time.c",
			"%{prj.name}/src/win32_thread.c",
			"%{prj.name}/src/win32_window.c",
			"%{prj.name}/src/wgl_context.c",
			"%{prj.name}/src/egl_context.c",
			"%{prj.name}/src/osmesa_context.c",
			"%{prj.name}/src/win32_module.c"
		}

		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

    filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
        runtime "Release"
		optimize "on"
--[[=====================================
------------------GLAD-----------------
=========================================]]--
project "Glad"
    location "Glad"
    kind "StaticLib"
    language "C"
	staticruntime "on"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    targetdir("bin-int/" .. outputdir .. "/%{prj.name}")

    files{
        "%{prj.name}/include/glad/glad.h",
        "%{prj.name}/include/KHR/khrplatform.h",
        "%{prj.name}/src/glad.c"
    }

    includedirs{
        "%{prj.name}/include"
    }
    filter "system:linux"
        pic "On"
        systemversion "latest"
        

    filter "system:windows"
        systemversion "latest"

		filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

    filter "configurations:Dist"
        runtime "Release"
		optimize "on"