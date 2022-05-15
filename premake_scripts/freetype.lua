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

	include "harfbazz.lua"

	dependson {
		"harfbazz"
	}

	if _ACTION ~= 'clean_up' and _ACTION ~= 'clean_up_full'  then

		current_dir = os.getcwd();
		os.chdir("../libs/build_harfbuzz")
		os.executef("cmake --build .")
		os.chdir(current_dir)

		harfbuzz_include = path.getabsolute("../libs/harfbazz/src/")
		harfbuzz_library = path.getabsolute("../libs/build_harfbuzz/Debug/harfbuzz.lib")

		os.executef('cmake --log-level NOTICE -G %q -B%s -S%s -D %s -D %s -D %s -D %s -D %s -D %s',
		"Visual Studio 17 2022",
		"../libs/build_freetype/",
		"../libs/freetype/",
		"FT_DISABLE_BROTLI:BOOL=ON",
		"FT_DISABLE_BZIP2:BOOL=ON",
		"FT_DISABLE_PNG:BOOL=ON",
		"FT_DISABLE_ZLIB:BOOL=ON",
		'HarfBuzz_INCLUDE_DIR:STRING="' .. harfbuzz_include .. '"',
		'HarfBuzz_LIBRARY:STRING="' .. harfbuzz_library .. '"'
		)
	end

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
