# CMakeList.txt : CMake project for UnitTests, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

include_directories(UnitTests "CUTE")

if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++latest")
    endif()
endif()

target_link_libraries(UnitTests ConfusableMatcher)

add_executable (UnitTests "UnitTests.cpp" "UnitTests.h")