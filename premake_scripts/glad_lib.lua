project "glad"
	filename "glad"
	kind "StaticLib"
	language "C"
	targetdir "../libs/compiled_libs/glad"
	location "../project_files/"

	includedirs {"../libs/glad/include"}

	vpaths {

		['Header Files'] = {
			"../libs/glad/include/glad/glad.h",
			"../libs/glad/include/KHR/khrplatform.h",
		},

		["Source Files"] = {

			"../libs/glad/src/glad.c",
		}
	}

	files
	{
		"../libs/glad/src/glad.c",
	}

	defines {
		"_GLAD_WIN32",
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "configurations:Debug"
			runtime "Debug"
			symbols "on"

