cmake_minimum_required(VERSION 3.24.1)

project(Basic++
        VERSION 0.1
        LANGUAGES CXX)

# Making the output directory unified for all build systems.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)

set(INCLUDE_DIR include/Basic++)

set(HEADERS 
    ${INCLUDE_DIR}/Common.hxx
    ${INCLUDE_DIR}/Defer.hxx
    ${INCLUDE_DIR}/NumericAliases.hxx
    ${INCLUDE_DIR}/Expectations.hxx
    ${INCLUDE_DIR}/Formatting.hxx
    ${INCLUDE_DIR}/Printing.hxx
    ${INCLUDE_DIR}/Mathematics.hxx
    ${INCLUDE_DIR}/Message.hxx
    ${INCLUDE_DIR}/PCH.hxx
    ${INCLUDE_DIR}/Sys.hpp
    ${INCLUDE_DIR}/TypeName.hxx
    ${INCLUDE_DIR}/win32/Win32.hpp
    ${INCLUDE_DIR}/IO.hxx
	${INCLUDE_DIR}/Expected.hxx
        )

set(SOURCES
    src/Sys.cpp
    src/Win32.cpp
    )

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})

set (PCH_FILE include/Basic++/PCH.hxx)
target_compile_definitions(${PROJECT_NAME} PUBLIC BASIC_PCH)
target_precompile_headers(${PROJECT_NAME} PUBLIC ${PCH_FILE})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)

set(CXX_STANDARD_REQUIRED ON)

target_include_directories(${PROJECT_NAME} PUBLIC include/)
target_link_libraries(${PROJECT_NAME} PUBLIC dbghelp)
target_include_directories(${PROJECT_NAME} PUBLIC win32/include/)