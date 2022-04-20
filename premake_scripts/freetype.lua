externalproject "freetype"
	filename "freetype"
	kind "StaticLib"
	language "C++"
	targetdir "../libs/build_freetype/freetype"
	location "../libs/build_freetype/"

	includedirs {"../libs/freetype/include/"}

	vpaths {

		['Header Files'] = {
			"../libs/freetype/include/**.h"
		},

		["Source Files"] = {
			"../libs/freetype/src/**.c"
		}
	}


	if _ACTION ~= 'clean_up' then
		os.executef('cmake --log-level NOTICE -G %q -B%s -S%s -D %s -D %s -D %s -D %s',
		"Visual Studio 17 2022",
		"../libs/build_freetype/",
		"../libs/freetype/",
		"FT_DISABLE_BROTLI:BOOL=ON",
		"FT_DISABLE_BZIP2:BOOL=ON",
		"FT_DISABLE_PNG:BOOL=ON",
		"FT_DISABLE_ZLIB:BOOL=ON"
		)
	end

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
