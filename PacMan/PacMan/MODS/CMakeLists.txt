cmake_minimum_required (VERSION 3.8)

set(MODS_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/MODS   # IMod.h, ModContext.h
    CACHE INTERNAL ""
)
 
set(FRAMEWORK_INCLUDE_DIR
    ${CMAKE_SOURCE_DIR}/Framework
    ${CMAKE_CURRENT_SOURCE_DIR}/
    CACHE INTERNAL ""
)

add_library(Mods STATIC
    ModManager.cpp
    ModManager.h
    IMod.h
)
add_subdirectory(SpeedBoost)