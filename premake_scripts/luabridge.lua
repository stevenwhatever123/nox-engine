
project "luabridge"
	filename "luabridge"
	kind "StaticLib"
	language "C++"
	targetdir "../libs/compiled_libs/luabridge"
	location "../project_files/"

	includedirs {"../libs/luabridge/Source/"}
	

	vpaths {

		['Header Files'] = {
			"../libs/luabridge/Source/LuaBridge/*.h"
		},

		["Source Files"] = {
			"../libs/luabridge/Source/LuaBridge/*.h"
		}
	}

	files
	 {
		"../libs/luabridge/Source/LuaBridge/*.h"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
