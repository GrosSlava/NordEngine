cmake_minimum_required(VERSION 3.10)

project(Tests)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	message(FATAL_ERROR "${PROJECT_NAME} can't be selected as main project!")
endif()



# Core
file(GLOB _CoreTestsCpp "${PROJECT_SOURCE_DIR}/Core/*.cpp")
set(CoreTestsCpp )
foreach(test ${_CoreTestsCpp})
	file(RELATIVE_PATH TName "${PROJECT_SOURCE_DIR}" ${test})
	list(APPEND CoreTestsCpp ${TName})
endforeach()

create_test_sourcelist(CoreTests 
	CoreTestsMain.cpp
	${CoreTestsCpp}
)
add_executable(CoreTestsMain ${CoreTests} Helpers/TestHelpers.h)
target_include_directories(CoreTestsMain PRIVATE "${PROJECT_SOURCE_DIR}/Helpers")
target_link_libraries(CoreTestsMain PRIVATE NordEngineCore)

foreach(test ${CoreTestsCpp})
	get_filename_component(TName ${test} NAME_WE)
	add_test(NAME Core_${TName} COMMAND CoreTestsMain Core/${TName})
endforeach()
set_target_properties(CoreTestsMain PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD_DEBUG True)

# Engine
file(GLOB _EngineTestsCpp "${PROJECT_SOURCE_DIR}/Engine/*.cpp")
set(EngineTestsCpp )
foreach(test ${_EngineTestsCpp})
	file(RELATIVE_PATH TName "${PROJECT_SOURCE_DIR}" ${test})
	list(APPEND EngineTestsCpp ${TName})
endforeach()

create_test_sourcelist(EngineTests 
	EngineTestsMain.cpp
	${EngineTestsCpp}
)
add_executable(EngineTestsMain ${EngineTests} Helpers/TestHelpers.h)
target_include_directories(EngineTestsMain PRIVATE "${PROJECT_SOURCE_DIR}/Helpers")
target_link_libraries(EngineTestsMain PRIVATE NordEngineLib)

foreach(test ${EngineTestsCpp})
	get_filename_component(TName ${test} NAME_WE)
	add_test(NAME Engine_${TName} COMMAND EngineTestsMain Engine/${TName})
endforeach()
set_target_properties(EngineTestsMain PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD_DEBUG True)
