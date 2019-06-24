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

include_directories(ConfusableMatcher "sparsehash/src" "sparsehash/src/windows")
include_directories(UnitTests "UnitTests/CUTE" ".")
add_test(UnitTests UnitTests)

add_library(ConfusableMatcher STATIC "ConfusableMatcher.cpp" "ConfusableMatcher.h" "Export.cpp" "Export.h")
add_executable(UnitTests "UnitTests/UnitTests.cpp" "UnitTests/UnitTests.h")

set_property(TARGET ConfusableMatcher PROPERTY CXX_STANDARD 17)
set_property(TARGET ConfusableMatcher PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(UnitTests ConfusableMatcher)
