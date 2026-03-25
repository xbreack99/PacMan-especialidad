cmake_minimum_required(VERSION 3.21)
project(SpeedBoost CXX)
 
add_library(SpeedBoost SHARED
    SpeedBoostMod.cpp
    SpeedBoostMod.h
)
 
target_include_directories(SpeedBoost
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${MODS_INCLUDE_DIR}                 
        ${FRAMEWORK_INCLUDE_DIR}             
)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

target_link_libraries(SpeedBoost PRIVATE SFML::Graphics)
# Set Windows-specific properties
if(WIN32)
    target_compile_definitions(SpeedBoost PRIVATE MYLIBRARY_EXPORTS)
endif()