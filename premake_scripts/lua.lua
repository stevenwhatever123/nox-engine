
project "lua"
	filename "lua"
	kind "StaticLib"
	language "C++"
	targetdir "../libs/compiled_libs/lua"
	location "../project_files/"

	includedirs {"../libs/" }
	

	vpaths {

		['Header Files'] = {
			"../libs/lua/*.h"
		},

		["Source Files"] = {
			"../libs/lua/*.c"
		}
	}

	files
	 {
		 "../libs/lua/onelua.c",
	}

	defines {
		"MAKE_LIB",
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
