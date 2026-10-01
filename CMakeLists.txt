cmake_minimum_required(VERSION 3.28)

project(Pong LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)

FetchContent_MakeAvailable(SFML)

add_executable(Pong src/main.cpp)

add_custom_command(
        TARGET Pong POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:Pong>/assets
)

target_compile_features(Pong PRIVATE cxx_std_17)

target_link_libraries(Pong PRIVATE SFML::Graphics)