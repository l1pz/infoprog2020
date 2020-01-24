workspace "Infoprog2020"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release",
		"Distribution",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Infoprog2020"
	location "Infoprog2020"
	kind "ConsoleApp"
	language "C++"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")
	
	files {
		"src/**.h",
		"src/**.cpp",
		"src/**.hpp",
		"src/**.c",
	}
	
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "on"
		systemversion "latest"
		
		defines {
			"IP_PLATFORM_WINDOWS",
		}
		
	filter "configurations:Debug"
		defines "IP_DEBUG"
		symbols "on"
	
	filter "configurations:Release"
		defines "IP_RELEASE"
		optimize "on"
		
	filter "configurations:Distribution"
		defines "IP_DISTRIBUTION"
		optimize "on"