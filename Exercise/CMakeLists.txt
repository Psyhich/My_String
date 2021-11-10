cmake_minimum_required(VERSION 3.21)

project(my_strings)

# Command to create symlink for clangd to see all includes
if(CMAKE_BUILD_TYPE MATCHES Debug)
	set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E create_symlink
			${CMAKE_BINARY_DIR}/compile_commands.json
			${CMAKE_SOURCE_DIR}/compile_commands.json
	)
endif()
# Function to check if module exists, it sets bool value to HAS_MODULE_<moudle name>
function(CheckHasModule Module)
  find_package(${Module} QUIET)
  if(NOT DEFINED ${Module}_DIR)
    set(HAS_MODULE_${Module} TRUE PARENT_SCOPE)
  elseif(${Module}_DIR)
    set(HAS_MODULE_${Module} TRUE PARENT_SCOPE)
  else()
    set(HAS_MODULE_${Module} FALSE PARENT_SCOPE)
  endif()
endfunction()

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

set(SOURCES classes.hpp classes.cpp)
set(MAIN main.cpp)
set(TESTS tester.cpp)

set(SOURCES include/my_string.h src/my_string.cpp)
set(MAIN src/main.cpp)
set(TESTS src/tests.cpp)

find_package(Threads REQUIRED)

CheckHasModule(GTest)

if(NOT ${HAS_MODULE_GTest})
	# If GoogleTest not found, downloading it
	include(FetchContent)
	FetchContent_Declare(
	  GoogleTest
	  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
	)
	# For Windows: Prevent overriding the parent project's compiler/linker settings
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(GoogleTest)
endif()

enable_testing()

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include/)

add_executable(main ${MAIN})
target_link_libraries(main ${PROJECT_NAME})

add_executable(tests ${TESTS})
target_link_libraries( tests
	gtest # For basic library usage
	gtest_main # For tests to be runned(this rung RUN_ALL_TESTS())
	rt # Needed for gtest
	pthread # Needed for gtest
	${PROJECT_NAME} # My test files
)

include(GoogleTest)
gtest_discover_tests(tests)

# Automaticaly running all tests on build
add_custom_command(TARGET tests
                   POST_BUILD
                   COMMAND ctest -C $<CONFIGURATION> --output-on-failure)

