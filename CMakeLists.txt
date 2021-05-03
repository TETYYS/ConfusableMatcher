cmake_minimum_required(VERSION 3.8)
project(ConfusableMatcher)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

find_package(Threads)

set(INSTALL_HAYAI FALSE CACHE BOOL "")
set(BUILD_HAYAI_TESTS FALSE CACHE BOOL "")
set(BUILD_HAYAI_SAMPLES FALSE CACHE BOOL "")
add_subdirectory("UnitTests/hayai")

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

include(CheckIncludeFileCXX)
check_include_file_cxx(experimental/string_view HAVE_EXPERIMENTAL_STRING_VIEW)

configure_file(${PROJECT_SOURCE_DIR}/Config.h.in ${PROJECT_SOURCE_DIR}/Config.h)

include_directories(ConfusableMatcherStatic "plf_stack")
include_directories(ConfusableMatcher "plf_stack")
include_directories(ConfusableMatcher "sparsehash/build/src")
include_directories(ConfusableMatcher "sparsehash/src")
if (WIN32)
	include_directories(ConfusableMatcher "sparsehash/src/windows")
endif()
if(MSVC)
	string(APPEND CMAKE_CXX_FLAGS " /Zc:__cplusplus")
endif()

add_library(ConfusableMatcherStatic STATIC "ConfusableMatcher.cpp" "ConfusableMatcher.h" "Export.cpp" "Export.h" "Config.h" "Shared.h" "ConfusableMatcherData.cpp")
set_target_properties(ConfusableMatcherStatic PROPERTIES OUTPUT_NAME confusablematcher)

add_library(ConfusableMatcher SHARED "ConfusableMatcher.cpp" "ConfusableMatcher.h" "Export.cpp" "Export.h" "Config.h" "Shared.h" "ConfusableMatcherData.cpp")
set_target_properties(ConfusableMatcher PROPERTIES PREFIX "")

target_link_libraries(ConfusableMatcherStatic ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(ConfusableMatcher ${CMAKE_THREAD_LIBS_INIT})

include_directories(RunUnitTests "UnitTests/CUTE" ".")
add_test(RunUnitTests RunUnitTests)
add_executable(RunUnitTests "UnitTests/UnitTests.cpp" "UnitTests/UnitTest.h")
target_link_libraries(RunUnitTests ConfusableMatcherStatic)

include_directories(RunBenchmark "UnitTests/hayai/src" ".")
add_executable(RunBenchmark "UnitTests/Benchmark.cpp")
target_link_libraries(RunBenchmark hayai_main ConfusableMatcherStatic)

add_executable(RunFuzz "Fuzzing/Main.cpp")
target_link_libraries(RunFuzz ConfusableMatcherStatic)

target_compile_features(ConfusableMatcher PRIVATE cxx_std_17)
target_compile_features(ConfusableMatcherStatic PRIVATE cxx_std_17)
target_compile_features(RunUnitTests PRIVATE cxx_std_17)
target_compile_features(RunBenchmark PRIVATE cxx_std_17)
target_compile_features(RunFuzz PRIVATE cxx_std_17)

if (NOT WIN32)
	file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/sparsehash/build)

	add_custom_target(
		SparsehashConfig
		COMMAND ../configure CXXFLAGS="-Ofast"
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/sparsehash/build
	)

	add_custom_target(
		SparsehashBuild
		COMMAND make
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/sparsehash/build
	)

	add_dependencies(SparsehashBuild SparsehashConfig)
	add_dependencies(ConfusableMatcher SparsehashConfig)
	add_dependencies(ConfusableMatcher SparsehashBuild)
endif()

add_dependencies(ConfusableMatcherStatic ConfusableMatcher)

export(PACKAGE ConfusableMatcher)