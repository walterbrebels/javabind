cmake_minimum_required(VERSION 3.24)

project(javabind LANGUAGES CXX VERSION 0.1)

option(JAVABIND_INTEGER_WIDENING_CONVERSION "Enable support for integer widening conversions (unsigned ints)" OFF)

# Java integration
find_package(JNI REQUIRED)

# compiler configuration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# sources
file(GLOB JAVABIND_LIBRARY_HEADERS
    ${CMAKE_SOURCE_DIR}/include/javabind/*.hpp
)

set(header_files ${JAVABIND_LIBRARY_HEADERS})

# header-only target for Java integration
add_library(javabind INTERFACE)
target_sources(javabind INTERFACE "$<BUILD_INTERFACE:${header_files}>")
target_include_directories(javabind INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_link_libraries(javabind INTERFACE JNI::JNI)

if(JAVABIND_INTEGER_WIDENING_CONVERSION)
    target_compile_definitions(javabind INTERFACE JAVABIND_INTEGER_WIDENING_CONVERSION)
endif()

if(MSVC)
    target_compile_definitions(javabind INTERFACE _CRT_SECURE_NO_WARNINGS)
    # set warning level 4 and treat all warnings as errors
    target_compile_options(javabind INTERFACE ${SIMDPARSE_AVX2_COMPILE} /permissive- /wd5272 /W4 /WX /Zc:__cplusplus,enumTypes,lambda,referenceBinding,rvalueCast,strictStrings,ternary)
else()
    # enable lots of warnings and treat all warnings as errors
    target_compile_options(javabind INTERFACE ${SIMDPARSE_AVX2_COMPILE} -Wall -Wextra -pedantic -Werror -Wfatal-errors)
endif()

# shared library for unit tests
add_library(javabind_native SHARED test/javabind.cpp)
target_link_libraries(javabind_native PRIVATE javabind)

# code generator
add_executable(javabind_codegen codegen/main.cpp)
target_link_libraries(javabind_codegen PRIVATE javabind_native javabind)

# installer
install(DIRECTORY include/javabind DESTINATION include)
