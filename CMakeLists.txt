cmake_minimum_required(VERSION 3.10)

project(NordEngine)



# Compiler config
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)
set(CMAKE_SUPPRESS_REGENERATION ON)

# Set CMake property
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/Install" CACHE PATH "default install path" FORCE)
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Bin/Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Bin/Release")

# Platform config
if(MSVC)
	add_definitions(/MP)
	add_compile_definitions(WIN32)
	add_compile_definitions(WIN64)
	add_compile_definitions(WINDOWS_PLATFORM)
elseif(UNIX AND NOT APPLE)
	add_compile_definitions(LINUX)
	add_compile_definitions(__linux)
	add_compile_definitions(LINUX_PLATFORM)
else()
	message(FATAL_ERROR "Unsupported platform!")
endif()

# Build config
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(EngineBuildOutputDir "${CMAKE_BINARY_DIR}/Bin/Debug")
	add_compile_definitions(_DEBUG)
	add_compile_definitions(DEBUG_BUILD=1)
	add_compile_definitions(RELEASE_BUILD=0)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	set(EngineBuildOutputDir "${CMAKE_BINARY_DIR}/Bin/Release")
	add_compile_definitions(_RELEASE)
	add_compile_definitions(NDEBUG)
	add_compile_definitions(DEBUG_BUILD=0)
	add_compile_definitions(RELEASE_BUILD=1)
else()
	message(FATAL_ERROR "Build type is not selected!")
endif()
add_compile_definitions(ENGINE)



# Engine
add_subdirectory(Source/NordEngine)

# Tools
add_subdirectory(Source/Tools/ReflectionGen)



# Group all source files
file(GLOB_RECURSE AllSources "${PROJECT_SOURCE_DIR}/Source/*.h" "${PROJECT_SOURCE_DIR}/Source/*.hpp" "${PROJECT_SOURCE_DIR}/Source/*.cpp" "${PROJECT_SOURCE_DIR}/Source/*.c")
foreach(LSource IN LISTS AllSources)
	#convert source file to absolute
	get_filename_component(SourceAbsPath "${LSource}" ABSOLUTE)
	# Get the directory of the absolute source file
	get_filename_component(GroupDir "${SourceAbsPath}" DIRECTORY)
	# Remove common directory prefix to make the group
	string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${GroupDir}")
	# Make sure we are using windows slashes
	string(REPLACE "/" "\\" GROUP2 "${GROUP}")
	source_group("${GROUP2}" FILES "${LSource}")
endforeach()

# Hide targets
set_target_properties(CppReflectionGen NordEngineCore NordEngineLib PROPERTIES FOLDER HiddenTargets)

# All target
add_custom_target(${PROJECT_NAME} SOURCES ${AllSources} DEPENDS 
	# Tools
	CppReflectionGen
	# Engine
	NordEngineCore
	NordEngineLib
)

# Install ThirdParty
if(MSVC)
	file(GLOB_RECURSE SharedLibs "${PROJECT_SOURCE_DIR}/Source/ThirdParty/*.dll")
elseif(UNIX AND NOT APPLE)
	#TODO
endif()
install(FILES ${SharedLibs} DESTINATION NordEngine)
# Install Tools
install(TARGETS 
			CppReflectionGen 
		DESTINATION NordEngine)
# Install Engine
install(TARGETS 
			NordEngineCore 
			NordEngineLib 
		DESTINATION NordEngine)