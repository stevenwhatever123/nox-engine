
project "fmod"
	filename "fmod"
	kind "StaticLib"
	language "C++"
	location "../project_files/"

	includedirs {"../libs/fmod/"}

	vpaths {

		['Header Files'] = {
			"../libs/fmod/**"
		},

		["Source Files"] = { }
	}

	files
	{
		-- "../libs/fmod/**",
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
