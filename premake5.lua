newaction {
	trigger = "clean_up",
	description = "Clean up files gen",
	execute = function()
		os.rmdir('build')
		os.rmdir('obj')
		os.rmdir('project_files')
		os.rmdir('x64')
		os.rmdir('.vs')
		os.remove('*.sln')
		os.remove('*.vcxproj*')
	end
	
}


newaction {
	trigger = 'clean_up_full',
	description = "Clean up all files and libs",
	execute = function()
		os.rmdir('build')
		os.rmdir('libs/compiled_libs')
		os.rmdir('libs/build_assimp')
		os.rmdir('libs/build_freetype')
		os.rmdir('libs/build_harfbuzz')
		os.rmdir('obj')
		os.rmdir('project_files')
		os.rmdir('x64')
		os.rmdir('.vs')
		os.remove('*.sln')
		os.remove('*.vcxproj*')
	end
}

workspace "NoxEngine"
	configurations {"Debug", "Release"}
	architecture "x64"
	
	startproject "NoxEngine"
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	include "premake_scripts/glfw.lua"
	include "premake_scripts/glm_lib.lua"
	include "premake_scripts/glad_lib.lua"
	include "premake_scripts/assimp.lua"
	include "premake_scripts/imgui.lua"
	include "premake_scripts/fmod.lua"
	include "premake_scripts/freetype.lua"
	include "premake_scripts/imguizmo.lua"
	include "premake_scripts/luabridge.lua"
	include "premake_scripts/lua.lua"

	project "NoxEngine"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++20"
		targetdir "build"
		justmycode "Off"

		includedirs {
			"headers/",
			"libs/",
			"libs/imgui",
			"libs/glfw/include/",
			"libs/glm/",
			"libs/glad/include/",
			"libs/assimp/include/",
			"libs/fmod/include/inc/",
			"libs/freetype/include/",
			"libs/imguizmo/",
			"libs/luabridge/Source/",
			"libs/lua/",
			"libs/3rdParty/",
		}

		dependson {
			"glfw",
			"glm",
			"glad",
			"Assimp",
			"imgui",
			"fmod",
			"freetype",
			"imguizmo",
			"luabridge",
			"lua",
			"zlib"
		}

		links {
			"user32",
			"gdi32",
			"opengl32",
			"glfw",
			"glm",
			"glad",
			"Assimp",
			"imgui",
			"libs/fmod/lib/fmod_vc",
			"libs/fmod/lib/fmodL_vc",
			"freetype",
			"imguizmo",
			"lua"
		}

		vpaths {
			["Headers"]            = { "headers/*.h", },
			["Headers/imgui"]      = { "libs/imgui/*.h" },
			["Headers/Components"] = { "headers/Components/*.h" },
			["Headers/Core"]       = { "headers/Core/*.h" },
			["Headers/EngineGUI"]  = { "headers/EngineGUI/*.h" },
			["Headers/Managers"]   = { "headers/Managers/*.h" },
			["Headers/Utils"]      = { "headers/Utils/*.h" },

			["Source Files"]            = { "src/*.cpp" },
			["Source Files/Components"] = { "src/Components/*.cpp" },
			["Source Files/EngineGUI"]  = { "src/EngineGUI/*.cpp" },
			["Source Files/Managers"]   = { "src/Managers/*.cpp" },
			["Source Files/Utils"]      = { "src/Utils/*.cpp" },
			["Source Files/Core"]       = { "src/Core/*.cpp" },

			["Source Files/imgui"] = {
				"libs/imgui/*.cpp",
				"libs/imgui/backends/imgui_impl_opengl3.cpp",
				"libs/imgui/backends/imgui_impl_glfw.cpp"

			}

		}

		files {
			"headers/**.h",
			"src/**.cpp",
			"libs/imgui/*.h",
			"libs/imgui/*.cpp",
			"libs/imgui/backends/imgui_impl_opengl3.cpp",
			"libs/imgui/backends/imgui_impl_glfw.cpp"
		}


		postbuildcommands { '{COPYFILE} "%{wks.location}/libs/fmod/lib/fmod.dll" %{cfg.targetdir}'  }
		postbuildcommands { '{COPYFILE} "%{wks.location}/libs/compiled_libs/assimp/Debug/assimp-vc143-mtd.dll" %{cfg.targetdir}'  }
