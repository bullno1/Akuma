solution "Akuma"
	configurations {"Develop", "Release"}
	location "build"

	project "Akuma"
		kind "ConsoleApp"
		language "C++"
		targetdir "bin"
		files {
			"src/**.h",
			"src/main.cpp",
			"src/Simulator.cpp",
			"src/Input.cpp",
			"src/FileWatcher/FileWatcher.cpp"
		}
		includedirs {
			"src"
		}
		debugargs {
			"iPhone_portrait",
			"../samples/basic/main.lua"
		}
		--[[vpaths {
			["src/**.h"] = "Header Files/*",
			["src/**.cpp"] = "Source Files/*"
		}]]
		links {
			"moaicore",
			"moaiext-luaext",
			"moaiext-untz",
			"sfml-window",
			"sfml-system",
			"sfml-graphics",
			"AntTweakBar"
		}
		defines {
			"BOOST_FILESYSTEM_VERSION=3"
		}

		configuration "windows"
			defines {
				"WIN32",
				"_CONSOLE"
			}
			links {
				"opengl32",
				"advapi32",
				"ws2_32",
				"iphlpapi",
				"user32",
				"gdi32"
			}
			files {
				"src/FileWatcher/FileWatcherWin32.cpp"
			}
			excludes {
				"src/FileWatcher/FileWatcherLinux.h",
				"src/FileWatcher/FileWatcherOSX.h"
			}

		configuration "Develop"
			defines {
				"NDEBUG"
			}
			flags {
				"Optimize",
				"Symbols",
				"NoEditAndContinue"
			}
			targetsuffix "_d"

		configuration "Release"
			defines {
				"NDEBUG"
			}
			flags {
				"Optimize",
				"OptimizeSpeed"
			}
