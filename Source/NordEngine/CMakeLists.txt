cmake_minimum_required(VERSION 3.10)

project(NordEngineLib)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	message(FATAL_ERROR "${PROJECT_NAME} can't be selected as main project!")
endif()



# Core
file(GLOB_RECURSE CoreHeades "${PROJECT_SOURCE_DIR}/Core/*.h")
file(GLOB_RECURSE CoreCpp "${PROJECT_SOURCE_DIR}/Core/*.cpp")
foreach(LSource IN LISTS CoreHeades CoreCpp)
	get_filename_component(SourceAbsPath "${LSource}" ABSOLUTE)
	get_filename_component(GroupDir "${SourceAbsPath}" DIRECTORY)
	string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${GroupDir}")
	string(REPLACE "/" "\\" GROUP2 "${GROUP}")
	source_group("${GROUP2}" FILES "${LSource}")
endforeach()

add_library(NordEngineCore STATIC ${CoreHeades} ${CoreCpp})
target_include_directories(NordEngineCore 
	PUBLIC "${PROJECT_SOURCE_DIR}/Core/Containers/Public"
	PUBLIC "${PROJECT_SOURCE_DIR}/Core/Files/Public"
	PUBLIC "${PROJECT_SOURCE_DIR}/Core/GenericPlatform/Public"
	PUBLIC "${PROJECT_SOURCE_DIR}/Core/Macros/Public"
	PUBLIC "${PROJECT_SOURCE_DIR}/Core/Math/Public"
	PUBLIC "${PROJECT_SOURCE_DIR}/Core/Memory/Public"
	PUBLIC "${PROJECT_SOURCE_DIR}/Core/Misc"
	PUBLIC "${PROJECT_SOURCE_DIR}/Core/Templates"
	PUBLIC "${PROJECT_SOURCE_DIR}/Core/Time/Public"
)


# Engine
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/Gen")
file(GLOB_RECURSE EngineHeades "${PROJECT_SOURCE_DIR}/Engine/*.h")
file(GLOB_RECURSE EngineCpp "${PROJECT_SOURCE_DIR}/Engine/*.h")
foreach(LSource IN LISTS EngineHeades EngineCpp)
	get_filename_component(SourceAbsPath "${LSource}" ABSOLUTE)
	get_filename_component(GroupDir "${SourceAbsPath}" DIRECTORY)
	string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${GroupDir}")
	string(REPLACE "/" "\\" GROUP2 "${GROUP}")
	source_group("${GROUP2}" FILES "${LSource}")
endforeach()

set(EngineGenH )
set(EngineGenCpp )
foreach(LSourceH ${EngineHeades})
	get_filename_component(__LFileName ${LSourceH} NAME_WE)
	list(APPEND EngineGenH "${PROJECT_BINARY_DIR}/Gen/${__LFileName}.generated.h")
	list(APPEND EngineGenCpp "${PROJECT_BINARY_DIR}/Gen/${__LFileName}.generated.cpp")
endforeach()
foreach(LSource IN LISTS EngineGenH EngineGenCpp)
	source_group("Generated" FILES "${LSource}")
endforeach()

add_library(NordEngineLib STATIC ${EngineHeades} ${EngineCpp} ${EngineGenH} ${EngineGenCpp})

target_include_directories(NordEngineLib 
	PUBLIC "${PROJECT_BINARY_DIR}/Gen"
	PUBLIC "${CMAKE_SOURCE_DIR}/Source/Tools/ReflectionGen/Framework"
	PUBLIC "${PROJECT_SOURCE_DIR}/Engine/CoreGame/Public"
	PUBLIC "${PROJECT_SOURCE_DIR}/Engine/Platforms/Public"
	PUBLIC "${PROJECT_SOURCE_DIR}/Engine/SubEngines/Public"
	PUBLIC "${PROJECT_SOURCE_DIR}/Engine/UObject/Public"
)

if(MSVC)
	target_include_directories(NordEngineLib 
		PUBLIC "${CMAKE_SOURCE_DIR}/Source/ThirdParty/DirectX12"
		PUBLIC "${CMAKE_SOURCE_DIR}/Source/ThirdParty/SFML/include"
	)
	file(GLOB SFML_LIBS "${CMAKE_SOURCE_DIR}/Source/ThirdParty/SFML/lib/Windows/*.lib")
elseif(UNIX AND NOT APPLE)
	#TODO
endif()

target_link_libraries(NordEngineLib 
	PUBLIC NordEngineCore
	${SFML_LIBS}
)

# Engine reflection generation
set(ReflectionGenIncludes )
get_target_property(NordEngineCore_INCLUDES NordEngineCore INCLUDE_DIRECTORIES)
foreach(LIncludeDir ${NordEngineCore_INCLUDES})
	string(APPEND ReflectionGenIncludes "-I ${LIncludeDir} ")
endforeach()
get_target_property(NordEngineLib_INCLUDES NordEngineLib INCLUDE_DIRECTORIES)
foreach(LIncludeDir ${NordEngineLib_INCLUDES})
	string(APPEND ReflectionGenIncludes "-I ${LIncludeDir} ")
endforeach()

add_custom_command(OUTPUT ${EngineGenH} ${EngineGenCpp} DEPENDS ${EngineHeades}
	COMMAND CppReflectionGen "${PROJECT_BINARY_DIR}/Gen" ${EngineHeades} ! ${ReflectionGenIncludes}
)
