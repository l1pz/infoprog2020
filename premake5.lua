workspace "Infoprog2020"
	architecture "x86_64"
	
	configurations
	{
		"Debug",
		"Release",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Infoprog2020"
	location "Infoprog2020"
	kind "ConsoleApp"
	language "C++"
	
	targetdir ("bin/" .. outputdir)
	objdir ("bin-obj/" .. outputdir)
	
	files {
		"src/**.h",
		"src/**.cpp",
		"src/**.hpp",
		"src/**.c",
	}

	pchheader "src/pch.h"
	
	cppdialect "C++17"
	staticruntime "on"

	filter "system:Windows"
      systemversion "latest" -- To use the latest version of the SDK available
		
	filter "configurations:Debug"
		symbols "on"
	
	filter "configurations:Release"
		optimize "on"