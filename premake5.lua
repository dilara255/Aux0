--should change AS_ prefix on defines later
workspace "FoF_AS"
	startproject "TestApp"

	configurations {"Debug", "Release"}
	
	platforms { "x86_64" }

	filter "platforms:x86_64"
		architecture "x86_64"
		defines "SYS_ARCH=x86_64"

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	filter {}
	
	defines 'COMPILE_CFG="%{cfg.buildcfg}"'
	defines 'CURR_ARCHTECTURE="%{cfg.architecture}"'
	defines 'CURR_SYSTEM="%{cfg.system}"'	
	
	IncludeDir = {}
	IncludeDir["SPDLOG"]  = "%{wks.location}/Dependendencies/spdlog/include"
	IncludeDir["AUXAPIS"] = "%{wks.location}/Aux0/APIs"

	LibDir = {}
	LibDir["AUX0"]   = ("%{wks.location}/Aux0/lib/" .. outputdir)

	binDir = "bin/" .. outputdir .. "/%{prj.name}"
	binIntDir = "bin-int/" .. outputdir .. "/%{prj.name}"
	

project "Aux0"
	location "Aux0"
	kind "Staticlib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	pchheader "miscStdHeaders.h"
	pchsource "%{prj.name}/src/miscStdHeaders.cpp"

	defines "AUX0"

	flags
	{
		"MultiProcessorCompile"
	}

	targetdir (binDir)
	objdir (binIntDir)

	files
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/include/**.hpp",
		"%{prj.name}/include/**.h",
		"%{prj.name}/APIs/**.h",		
		"%{prj.name}/APIs/**.hpp"
	}

	includedirs {
		"%{IncludeDir.SPDLOG}",
		"%{prj.name}/include",
		"%{IncludeDir.AUXAPIS}"
	}

	filter "architecture:x86_64"

		defines "X64"

	filter "system:windows"
		systemversion "latest"
		--buildoptions "/MT" --may cause override, should do inside filter

		defines{ 
			"AS_PLATFORM_WINDOWS",
			"AS_BUILD_LIB"
		}

	filter "configurations:Debug"
		defines "AS_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines	"AS_RELEASE"
		optimize "on" 		

	filter {}
	
	postbuildcommands{
		("{COPY} %{cfg.buildtarget.relpath} %{LibDir.AUX0}")
	}
	
project "TestApp"
	location "TestApp"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	dependson {"Aux0"}

	defines {"AS_TESTAPP"}

	flags
	{
		"MultiProcessorCompile"
	}

	links ("%{LibDir.AUX0}/Aux0.lib")

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.h",
		"%{prj.name}/include/**.h",
		"%{prj.name}/include/**.hpp"
	}

	includedirs
	{
		"%{prj.name}/include",
		"%{IncludeDir.SPDLOG}", --is this really needed?
		"%{IncludeDir.AUXAPIS}"
	}

	filter "architecture:x86_64"
		defines "X64"

	filter "system:windows"
		systemversion "latest"
		--buildoptions "/MT" --may cause override, should do inside filter

		defines{
			"AS_PLATFORM_WINDOWS",
			"AS_BUILD_APP"
		}

	filter "configurations:Debug"
		defines "AS_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines	"AS_RELEASE"
		optimize "on" 		