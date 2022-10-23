
import os
import sys





'''
	Folders to remove from project.
'''
INTERMEDIATE_FOLDERS = [ \
"Intermediate", \
".vs", \
".vscode", \
"__pycache__" \
]

'''
	File extensions to remove from project.
'''
INTERMEDIATE_EXTENSIONS = [ \
".user", \
".generated", \
".gen", \
".obj", \
".o", \
".sln", \
".vcxproj", \
".code-workspace" \
]

'''
	Paths for c++ easy includs.
	Relative to engine root dir.
'''
ENGINE_INCLUDE_PATHS = [ \
os.path.join("Source", "ThirdParty", "SFML", "include"), \
os.path.join("Source", "SimpleEngine", "Core", "Containers", "Public"), \
os.path.join("Source", "SimpleEngine", "Core", "Delegate", "Public"), \
os.path.join("Source", "SimpleEngine", "Core", "Files", "Public"), \
os.path.join("Source", "SimpleEngine", "Core", "GenericPlatform", "Public"), \
os.path.join("Source", "SimpleEngine", "Core", "Macros"), \
os.path.join("Source", "SimpleEngine", "Core", "Math", "Public"), \
os.path.join("Source", "SimpleEngine", "Core", "Memory", "Public"), \
os.path.join("Source", "SimpleEngine", "Core", "Misc"), \
os.path.join("Source", "SimpleEngine", "Core", "Path", "Public"), \
os.path.join("Source", "SimpleEngine", "Core", "Templates"), \
os.path.join("Source", "SimpleEngine", "Core", "Time", "Public"), \
os.path.join("Source", "SimpleEngine", "Engine", "CoreGame", "Public"), \
os.path.join("Source", "SimpleEngine", "Engine", "Platforms", "Public"), \
os.path.join("Source", "SimpleEngine", "Engine", "SubEngines", "Public"), \
os.path.join("Source", "SimpleEngine", "Engine", "UObject", "Public") \
]




'''
	Libs used by engine.
	Append with platform specific suffix like '.lib'
'''
ENGINE_WINDOWS_USING_LIBS = [ \
"SimpleEngine", \
"openal32", \
"sfml-graphics", \
"sfml-window", \
"sfml-system", \
"sfml-audio", \
"sfml-network", \
"winmm" \
]

'''
	Libs used by engine.
	Append with platform specific suffix like '.a', '.so'.
'''
ENGINE_LINUX_USING_LIBS = [ \
"SimpleEngine" \
]




'''
	Reserved folders names.
'''
RESERVED_PROJECT_FOLDER_NAME = [ \
"Intermediate", \
"Build", \
"build", \
"Source", \
"source", \
"Src", \
"src", \
"SRC", \
"Content", \
"content", \
"Docs", \
"docs", \
"doc", \
"Tools", \
"tools", \
"Private", \
"private", \
"Public", \
"public", \
"Plugins", \
"plugins", \
"ThirdParty" \
]


#..................Reserved modules names.................#

ENGINE_MODULE_NAME = "SimpleEngine"

#.........................................................#