#include "UIButtonComponent.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "../InputManager.h"
#include "../Rendering/SpriteRenderer.h"
#include "ComponentRegistry.h"
#include "../Rendering/Camera.h"
#include <glm/vec2.hpp>

REGISTER_COMPONENT(UIButtonComponent);

UIButtonComponent::UIButtonComponent(GameObject* owner)
    : IComponent(owner)
{
}

void UIButtonComponent::Deserialize(const nlohmann::json& data)
{
    if (data.contains("targetScene"))
        targetScene = data["targetScene"].get<std::string>();

}

void UIButtonComponent::Update(float)
{
    if (targetScene.empty())
        return;

    bool mouseDown = InputManager::Instance().IsMouseDown(InputManager::MouseButton::Left);

    if (IsMouseOver() && mouseDown && !wasPressed)
    {
        SceneManager::Instance().LoadSceneDeferred(targetScene);
        wasPressed = true;
    }

    if (!mouseDown)
        wasPressed = false;
}

bool UIButtonComponent::IsMouseOver() const
{
    auto* sprite = owner->GetComponent<SpriteRenderer>();
    if (!sprite) return false;

    float mouseX = InputManager::Instance().GetMouseX();
    float mouseY = InputManager::Instance().GetMouseY();

    int winW = 0, winH = 0;
    SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &winW, &winH);

    glm::vec2 buttonScreen;
    buttonScreen.x = winW * 0.5f + owner->GetTransform().Position.x;
    buttonScreen.y = winH * 0.5f - owner->GetTransform().Position.y;

    glm::vec2 halfSize = sprite->GetSize() * 0.5f;

    bool over =
        mouseX >= buttonScreen.x - halfSize.x &&
        mouseX <= buttonScreen.x + halfSize.x &&
        mouseY >= buttonScreen.y - halfSize.y &&
        mouseY <= buttonScreen.y + halfSize.y;

    return over;
}
