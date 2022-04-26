
project "imguizmo"
	filename "imguizmo"
	kind "StaticLib"
	language "C++"
	targetdir "../libs/compiled_libs/imguizmo"
	location "../project_files/"

	includedirs {"../libs/imguizmo/", "../libs/imgui/"}
	

	vpaths {

		['Header Files'] = {
			"../libs/imguizmo/*.h"
		},

		["Source Files"] = {
			"../libs/imguizmo/*.cpp"
		}
	}

	files
	 {
		"../libs/imguizmo/*.h",
		"../libs/imguizmo/*.cpp"
	}

	defines {
		"WIN32",
		"_WINDOWS"
	}
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
