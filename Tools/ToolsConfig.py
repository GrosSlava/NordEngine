
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
os.path.join("Source", "Engine", "Core", "Containers", "Public"), \
os.path.join("Source", "Engine", "Core", "Delegate", "Public"), \
os.path.join("Source", "Engine", "Core", "Files", "Public"), \
os.path.join("Source", "Engine", "Core", "GenericPlatform", "Public"), \
os.path.join("Source", "Engine", "Core", "Macros"), \
os.path.join("Source", "Engine", "Core", "Math", "Public"), \
os.path.join("Source", "Engine", "Core", "Memory", "Public"), \
os.path.join("Source", "Engine", "Core", "Misc"), \
os.path.join("Source", "Engine", "Core", "Path", "Public"), \
os.path.join("Source", "Engine", "Core", "Templates"), \
os.path.join("Source", "Engine", "Core", "Time", "Public"), \
os.path.join("Source", "Engine", "Engine", "CoreGame", "Public"), \
os.path.join("Source", "Engine", "Engine", "Platforms", "Public"), \
os.path.join("Source", "Engine", "Engine", "SubEngines", "Public"), \
os.path.join("Source", "Engine", "Engine", "UObject", "Public") \
]

'''
	Libs used by engine.
	Append with platform specific suffix like '.lib' or '.a', '.so' etc...
'''
ENGINE_USING_LIBS = [ \
"Engine", \
"openal32", \
"sfml-graphics", \
"sfml-window", \
"sfml-system", \
"sfml-audio", \
"sfml-network" \
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

ENGINE_MODULE_NAME = "Engine"

#.........................................................#