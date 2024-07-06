cmake_minimum_required(VERSION 3.16)
project(ChessEngine LANGUAGES CXX)

option(ENABLE_PROFILING "Enable profiling with gprof" OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)
file(GLOB EXEC_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp
                   ${PROJECT_SOURCE_DIR}/src/gui/*.cpp
                   ${PROJECT_SOURCE_DIR}/engine/*.cpp
                   ${PROJECT_SOURCE_DIR}/engine/movegen/*.cpp
                   ${PROJECT_SOURCE_DIR}/utils.cpp
                   ${PROJECT_SOURCE_DIR}/src/drivers/*.cpp
                   ${PROJECT_SOURCE_DIR}/src/tests/*.cpp
                   
                   )

add_executable(main ${EXEC_SRC})

target_link_libraries(main PRIVATE sfml-graphics)
target_compile_features(main PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET main
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main>
        VERBATIM)
endif()

if(ENABLE_PROFILING)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
endif()