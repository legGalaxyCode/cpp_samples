cmake_minimum_required(VERSION 3.20)

# Add submodules

add_subdirectory(./third_party/googletest)

set(PROJECT_NAME cpp_samples)
project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Werror -Wpedantic")
# Temporary solution, should be migrated to C++23
set(CMAKE_CXX_STANDARD 20)

if (ASAN)
	message("ASAN is enabled!")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=leak")
endif(ASAN)

if (UBSAN)
	message("Build with UBsan!")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
endif(UBSAN)

if (THREAD)
	message("Build with Thread sanitizer!")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
endif(THREAD)

#
# if (USE_CLANG_TIDY)
# 	message("Build with clang-tidy!")
# 	find_program(CLANG_TIDY_EXE NAMES clang-tidy)
# 	if (CLANG_TIDY_EXE)
# 		message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
# 		set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-header-filter=.*" "--config-file=${PROJECT_SOURCE_DIR}/.clang-tidy")
# 	else()
# 		message(STATUS "clang-tidy not found!")
# 	endif()
# endif(USE_CLANG_TIDY)

add_subdirectory(std)
add_subdirectory(test)

# enable_testing()

# include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
# file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)
# add_executable(runUnitTests ${TEST_SRC_FILES})
# target_link_libraries(runUnitTests gtest gtest_main)
# target_link_libraries(runUnitTests project1_lib)
# add_test(UnitTests runUnitTests)
