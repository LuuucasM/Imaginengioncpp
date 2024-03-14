include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "Imaginengion"
	architecture "x86_64"
	startproject "SangSang"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/premake"
	include "Imaginengion/vendor/GLFW"
	include "Imaginengion/vendor/Glad"
	include "Imaginengion/vendor/imgui"
	include "Imaginengion/vendor/yaml-cpp"
	
group ""
	include "Imaginengion"
	include "Sandbox"
	include "SangSang"