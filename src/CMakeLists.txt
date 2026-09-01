add_executable(GEP
    "main.cpp"
    "Engine.h"
    "Engine.cpp"
    "InputManager.h"
    "InputManager.cpp"
    "Time.h"
    "Time.cpp"

    "Shape/Triangle.h" 
    "Shape/Triangle.cpp" 
    "Shape/Rectangle.h" 
    "Shape/Rectangle.cpp" 

    "Rendering/Shader.h" 
    "Rendering/Shader.cpp" 
    "Rendering/Texture.h" 
    "Rendering/Texture.cpp"
    "Rendering/Camera.h" 
    "Rendering/Camera.cpp"
    "Rendering/SpriteRenderer.h"
    "Rendering/SpriteRenderer.cpp"

    "Core/Transform.h"
    "Core/Transform.cpp" 
    "Core/ResourceManager.h" 
    "Core/ResourceManager.cpp" 
    "Core/Component.h" 
    "Core/GameObject.h" 
    "Core/GameObject.cpp" 
    "Core/Scene.h" 
    "Core/Scene.cpp" 
    "Core/SceneManager.h" 
    "Core/SceneManager.cpp" 
    "Core/ComponentRegistry.h" 

    "Core/ChaseBehavior.h" 
    "Core/ChaseBehavior.cpp" 
    "Core/PlayerMovement.h" 
    "Core/PlayerMovement.cpp" 
    "Core/PickupComponent.h" 
    "Core/PickupComponent.cpp" 
    "Core/PlayerWeapon.h" 
    "Core/PlayerWeapon.cpp" 
    "Core/EnemyHealthComponent.h" 
    "Core/EnemyHealthComponent.cpp" 
    "Core/PlayerHealthComponent.h" 
    "Core/PlayerHealthComponent.cpp" 
    "Core/EnemyWeaponComponent.h" 
    "Core/EnemyWeaponComponent.cpp" 
    "Core/AnimationComponent.h" 
    "Core/AnimationComponent.cpp" 

    "Core/UIButtonComponent.h" 
    "Core/UIButtonComponent.cpp"  
    "Core/RespawnComponent.h" 
    "Core/RespawnComponent.cpp" 
    
    "World/WorldGenerator.h" 
    "World/WorldGenerator.cpp")

set_property(TARGET GEP PROPERTY CXX_STANDARD 20)
set_property(TARGET GEP PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(GEP PRIVATE 
    SDL3::SDL3
    SDL3_image::SDL3_image
    glad
    glm::glm-header-only
    nlohmann_json::nlohmann_json
)

add_custom_target(AssetCopy
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${ASSETS_INPUT_DIRECTORY}" "$<TARGET_FILE_DIR:GEP>/assets"
    COMMENT "Copying assets to build directory: $<TARGET_FILE_DIR:GEP>/assets")

add_dependencies(GEP AssetCopy)

add_custom_target(ShaderCopy
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${SHADERS_INPUT_DIRECTORY}" "$<TARGET_FILE_DIR:GEP>/shaders"
    COMMENT "Copying shaders to build directory: $<TARGET_FILE_DIR:GEP>/shaders")

add_dependencies(GEP ShaderCopy)