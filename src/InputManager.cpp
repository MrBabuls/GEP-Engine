#include "InputManager.h"

InputManager& InputManager::Instance() 
{
    static InputManager instance;
    return instance;
}

void InputManager::Update() 
{
    if (m_inputDisabled)
    {
        m_keysDown.clear();
        m_keysUp.clear();
    }
    else
    {
        m_keysDown.clear();
        m_keysUp.clear();
    }

    m_mouseDown.clear();
    m_mouseUp.clear();
}

void InputManager::ProcessEvent(const SDL_Event& event) 
{
    if (event.type == SDL_EVENT_MOUSE_MOTION)
    {
        m_mouseX = static_cast<float>(event.motion.x);
        m_mouseY = static_cast<float>(event.motion.y);
        return;
    }

    bool blockGameplayKeys = m_gameplayInputDisabled;

    switch (event.type)
    {
    case SDL_EVENT_KEY_DOWN:
        if (!blockGameplayKeys)
        {
            if (!event.key.repeat)
            {
                m_keysDown.insert(event.key.key);
                m_keysPressed.insert(event.key.key);
            }
        }
        break;

    case SDL_EVENT_KEY_UP:
        if (!blockGameplayKeys)
        {
            m_keysUp.insert(event.key.key);
            m_keysPressed.erase(event.key.key);
        }
        break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        m_mouseDown.insert(event.button.button);
        m_mousePressed.insert(event.button.button);
        break;

    case SDL_EVENT_MOUSE_BUTTON_UP:
        m_mouseUp.insert(event.button.button);
        m_mousePressed.erase(event.button.button);
        break;
    }
}

// Keyboard
bool InputManager::IsKeyDown(Key key) const 
{
    return m_keysDown.find(static_cast<SDL_Keycode>(key)) != m_keysDown.end();
}

bool InputManager::IsKeyUp(Key key) const 
{
    return m_keysUp.find(static_cast<SDL_Keycode>(key)) != m_keysUp.end();
}

bool InputManager::IsKeyPressed(Key key) const 
{
    return m_keysPressed.find(static_cast<SDL_Keycode>(key)) != m_keysPressed.end();
}

// Mouse
bool InputManager::IsMouseDown(MouseButton btn) const 
{
    return m_mouseDown.find(static_cast<unsigned int>(btn)) != m_mouseDown.end();
}

bool InputManager::IsMouseUp(MouseButton btn) const 
{
    return m_mouseUp.find(static_cast<unsigned int>(btn)) != m_mouseUp.end();
}

bool InputManager::IsMousePressed(MouseButton btn) const 
{
    return m_mousePressed.find(static_cast<unsigned int>(btn)) != m_mousePressed.end();
}
