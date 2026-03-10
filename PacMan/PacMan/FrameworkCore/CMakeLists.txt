
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
    RenderSystem.cpp)

find_package(ImGui-SFML CONFIG REQUIRED)
target_link_libraries(FrameworkCore PRIVATE ImGui-SFML::ImGui-SFML)