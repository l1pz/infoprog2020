workspace "infoprog2020"
architecture "x86_64"

configurations {
  "debug",
  "release"
}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "infoprog2020"
location "infoprog2020"
kind "ConsoleApp"
language "C++"

targetdir("bin/" .. outputdir)
objdir("bin-obj/" .. outputdir)
includedirs {"includes/"}
libdirs {"libs/"}
links {"nana"}

files {
  "src/**.h",
  "src/**.cpp",
  "src/**.hpp",
  "src/**.c"
}

pchheader "src/pch.h"

cppdialect "C++17"
staticruntime "on"

filter "system:Windows"
systemversion "latest"

filter "system:Linux"
linkoptions "-pthread"
links {"X11", "pthread", "rt", "Xft", "png", "asound", "fontconfig"}

filter "configurations:Debug"
symbols "on"

filter "configurations:Release"
optimize "on"

newaction {
  trigger = "clean",
  description = "clean the software",
  execute = function()
    print("clean the build...")
    os.rmdir("./bin-obj")
    os.rmdir("./bin")
    print("done.")
  end
}
