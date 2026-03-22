cmake_minimum_required(VERSION 3.21)
project(SpeedBoost CXX)
 
add_library(SpeedBoost SHARED
    SpeedBoostMod.cpp
    SpeedBoostMod.h
)
 
target_include_directories(SpeedBoost
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}          # SpeedBoost.h
    PRIVATE
        ${MODS_INCLUDE_DIR}                  # IMod.h, ModContext.h
        ${FRAMEWORK_INCLUDE_DIR}             # MovementComponent.h
)

find_package(ImGui-SFML CONFIG REQUIRED)
target_link_libraries(SpeedBoost PRIVATE ImGui-SFML::ImGui-SFML)

# Set Windows-specific properties
if(WIN32)
    target_compile_definitions(SpeedBoost PRIVATE MYLIBRARY_EXPORTS)
endif()