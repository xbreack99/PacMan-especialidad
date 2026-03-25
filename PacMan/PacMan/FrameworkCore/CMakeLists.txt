
add_library(
    FrameworkCore
    STATIC
    Node.cpp
    GraphicsComponent.cpp
    TransformComponent.cpp
    IComponent.h
    Node.h
    GraphicsComponent.h
    TransformComponent.h
    ISystem.h 
    InputSystem.cpp
    InputSystem.h 
    InputComponent.h
    InputComponent.cpp
    RenderSystem.h
    RenderSystem.cpp 
    "ColisionSystem.h"
    "ColisionSystem.cpp"
    "ColliderComponent.cpp"
    "ColliderComponent.h"
    "Singleton.h"
    "TileMapComponent.h"
    "Utilitys.h"
    "TileMapComponent.cpp"
    "TileCollisionSystem.h"
    "TileCollisionSystem.cpp"
    "MovementComponent.h"
    "MovementComponent.cpp"
    "SceneManager.h"
    "SceneManager.cpp"
    "IScene.h"   )

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

target_link_libraries(FrameworkCore PRIVATE SFML::Graphics)