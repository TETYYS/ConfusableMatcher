# CMakeList.txt : CMake project for ConfusableMatcher, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++latest")
    endif()
endif()

include_directories(ConfusableMatcher "sparsehash/src" "sparsehash/src/windows")

add_library(SHARED ConfusableMatcher "ConfusableMatcher.cpp" "ConfusableMatcher.h" "Export.cpp" "Export.h")