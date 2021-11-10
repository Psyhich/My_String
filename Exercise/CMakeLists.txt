cmake_minimum_required(VERSION 3.21)

project(my_strings)

# Commend to create symlink for clangd to see all includes
if(CMAKE_BUILD_TYPE EQUAL "Debug")
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E create_symlink
			${CMAKE_BINARY_DIR}/compile_commands.json
			${CMAKE_SOURCE_DIR}/compile_commands.json
	)
	set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
endif()

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

set(SOURCES classes.hpp classes.cpp)
set(MAIN main.cpp)
set(TESTS tester.cpp)

set(SOURCES include/my_string.h src/my_string.cpp)
set(MAIN src/main.cpp)
set(TESTS src/tests.cpp)

find_package(Threads REQUIRED)

find_package(GTest REQUIRED)

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

