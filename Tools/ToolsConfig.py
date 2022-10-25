
import os
import sys




# Name of engine to use in paths and other names.
ENGINE_NAME = "NordEngine"



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
os.path.join("Source", ENGINE_NAME, "Core", "Containers", "Public"), \
os.path.join("Source", ENGINE_NAME, "Core", "Delegate", "Public"), \
os.path.join("Source", ENGINE_NAME, "Core", "Files", "Public"), \
os.path.join("Source", ENGINE_NAME, "Core", "GenericPlatform", "Public"), \
os.path.join("Source", ENGINE_NAME, "Core", "Macros"), \
os.path.join("Source", ENGINE_NAME, "Core", "Math", "Public"), \
os.path.join("Source", ENGINE_NAME, "Core", "Memory", "Public"), \
os.path.join("Source", ENGINE_NAME, "Core", "Misc"), \
os.path.join("Source", ENGINE_NAME, "Core", "Path", "Public"), \
os.path.join("Source", ENGINE_NAME, "Core", "Templates"), \
os.path.join("Source", ENGINE_NAME, "Core", "Time", "Public"), \
os.path.join("Source", ENGINE_NAME, "Engine", "CoreGame", "Public"), \
os.path.join("Source", ENGINE_NAME, "Engine", "Platforms", "Public"), \
os.path.join("Source", ENGINE_NAME, "Engine", "SubEngines", "Public"), \
os.path.join("Source", ENGINE_NAME, "Engine", "UObject", "Public") \
]




'''
	Libs used by engine.
	Append with platform specific suffix like '.lib'
'''
ENGINE_WINDOWS_USING_LIBS = [ \
ENGINE_NAME, \
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
ENGINE_NAME \
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

ENGINE_MODULE_NAME = ENGINE_NAME

#.........................................................#