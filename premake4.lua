solution "Akuma"
	configurations {"Develop"}
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
			"src",
			os.getenv("MOAI_INCLUDE")
		}
		libdirs {
			os.getenv("MOAI_LIB")
		}
		debugargs {
			"iPhone_portrait",
			"../samples/basic/main.lua"
		}
		links {
			"moaicore",
			"moaiext-luaext",
			"moaiext-untz",
			"SDL"
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
				"gdi32",
				"psapi",
				"dsound",
				"strmiids",
				"rpcrt4"
			}
			files {
				"src/FileWatcher/FileWatcherWin32.cpp",
				"src/Akuma.ico",
				"src/Akuma.rc"
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
				"OptimizeSpeed",
				"Symbols",
				"NoEditAndContinue"
			}
