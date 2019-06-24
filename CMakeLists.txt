# CMakeList.txt : CMake project for ConfusableMatcher, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project(ConfusableMatcher)

enable_testing()

if (MSVC_VERSION GREATER_EQUAL "1900")
	include(CheckCXXCompilerFlag)
	CHECK_CXX_COMPILER_FLAG("/std:c++17" _cpp_latest_flag_supported)
	if (_cpp_latest_flag_supported)
		add_compile_options("/std:c++17")
	endif()
endif()

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

include_directories(ConfusableMatcher "sparsehash/src")
if (WIN32)
	include_directories(ConfusableMatcher "sparsehash/src/windows")
endif()
include_directories(RunUnitTests "UnitTests/CUTE" ".")
add_test(RunUnitTests RunUnitTests)

add_library(ConfusableMatcher STATIC "ConfusableMatcher.cpp" "ConfusableMatcher.h" "Export.cpp" "Export.h")
add_executable(RunUnitTests "UnitTests/UnitTests.cpp" "UnitTests/UnitTests.h")

set_property(TARGET ConfusableMatcher PROPERTY CXX_STANDARD 17)
set_property(TARGET ConfusableMatcher PROPERTY CXX_STANDARD_REQUIRED ON)

if (NOT WIN32)
	add_custom_target(
		SparsehashConfig
		COMMAND ./configure
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/sparsehash
	)

	add_custom_target(
		SparsehashBuild
		COMMAND make
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/sparsehash
	)

	add_dependencies(ConfusableMatcher SparsehashConfig)
	add_dependencies(ConfusableMatcher SparsehashBuild)
endif()

target_link_libraries(RunUnitTests ConfusableMatcher)
