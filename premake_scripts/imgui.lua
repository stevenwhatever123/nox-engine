project "imgui"
	filename "imgui"
	kind "StaticLib"
	language "C++"
	targetdir "../libs/compiled_libs/imgui"
	location "../project_files/"

	includedirs {"../libs/imgui/"}

	vpaths {

		['Header Files'] = {
			"../libs/imgui/*.h"
		},

		["Source Files"] = {
			"../libs/imgui/*.cpp"
		}
	}

	files
	{
		"../libs/imgui/*.cpp",
		"../libs/imgui/*.h"
	}

	defines {
		"WIN32",
		"_WINDOWS"
	}
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
