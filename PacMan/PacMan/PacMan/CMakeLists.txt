
find_package (SFML 3 REQUIRED COMPONENTS Graphics)
find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
add_executable (PacMan "PacMan.h" "PacMan.cpp" "Main.cpp" "TileMap.h" "TileMap.cpp" "GameScene.h" "GhostAISystem.h" "GhostAIComponent.h" "GhostAISystem.cpp" "GhostAIComponent.cpp" "PacmanCollisionSystem.h" "GameScene.cpp" "GameOverScene.h" "GameOverScene.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET PacMan PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries (PacMan PRIVATE SFML::Graphics)
target_link_libraries(PacMan PUBLIC FrameworkCore)
target_link_libraries(PacMan PRIVATE imgui::imgui)
target_link_libraries(PacMan PRIVATE ImGui-SFML::ImGui-SFML)

target_include_directories(PacMan PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/FrameworkCore)