externalproject "harfbazz"
	filename "harfbuzz"
	kind "StaticLib"
	language "C++"
	targetdir "../libs/build_harfbuzz/harfbuzz"
	location "../libs/build_harfbuzz/"

	includedirs {"../libs/harfbazz/src/"}

	if _ACTION ~= 'clean_up' and _ACTION ~= 'clean_up_full'  then
		os.executef('cmake --log-level NOTICE -G %q -B%s -S%s',
		"Visual Studio 17 2022",
		"../libs/build_harfbuzz/",
		"../libs/harfbazz"
		)
	end


	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
