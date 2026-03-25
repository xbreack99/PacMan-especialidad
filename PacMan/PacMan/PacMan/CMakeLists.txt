add_executable (PacMan "PacMan.h" "PacMan.cpp" "Main.cpp" "TileMap.h" "TileMap.cpp" "GameScene.h" "GhostAISystem.h" "GhostAIComponent.h" "GhostAISystem.cpp" "GhostAIComponent.cpp" "PacmanCollisionSystem.h" "GameScene.cpp" "GameOverScene.h" "GameOverScene.cpp" "PauseScene.h" "PauseScene.cpp" "MenuScene.h" "MenuScene.cpp" "PacmanCollisionSystem.cpp"    "Ghost.h" "Ghost.cpp" "MapLoader.h" "MapLoader.cpp" "QuickSave.h" "QuickSave.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET PacMan PROPERTY CXX_STANDARD 20)
endif()

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

target_link_libraries(PacMan PRIVATE SFML::Graphics)

target_link_libraries(PacMan PUBLIC FrameworkCore)
target_link_libraries(PacMan PRIVATE Mods)
target_include_directories(PacMan PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/FrameworkCore)

add_custom_command(
    TARGET PacMan
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)