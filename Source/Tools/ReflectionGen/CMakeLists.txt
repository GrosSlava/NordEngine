cmake_minimum_required(VERSION 3.10)

project(CppReflectionGen)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	message(FATAL_ERROR "${PROJECT_NAME} can't be selected as main project!")
endif()



file(GLOB_RECURSE Sources "${PROJECT_SOURCE_DIR}/*.h" "${PROJECT_SOURCE_DIR}/*.cpp")
foreach(LSource IN LISTS Sources)
	get_filename_component(SourceAbsPath "${LSource}" ABSOLUTE)
	get_filename_component(GroupDir "${SourceAbsPath}" DIRECTORY)
	string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${GroupDir}")
	string(REPLACE "/" "\\" GROUP2 "${GROUP}")
	source_group("${GROUP2}" FILES "${LSource}")
endforeach()

add_executable(CppReflectionGen ${Sources})

if(MSVC)
	target_include_directories(CppReflectionGen 
		PRIVATE "${PROJECT_SOURCE_DIR}"
		PRIVATE "${CMAKE_SOURCE_DIR}/Source/ThirdParty/llvm/include"
	)
	target_link_libraries(CppReflectionGen 
		PRIVATE "${CMAKE_SOURCE_DIR}/Source/ThirdParty/llvm/lib/Windows/libclang.lib"
	)
	add_custom_command(TARGET CppReflectionGen POST_BUILD 
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/Source/ThirdParty/llvm/bin/Windows/libclang.dll" "${EngineBuildOutputDir}/libclang.dll"
	)
elseif(UNIX AND NOT APPLE)
	#TODO
endif()