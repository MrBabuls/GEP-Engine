#pragma once
#include "SDL3/SDL.h"
#include <unordered_set>

class InputManager {
public:
    enum class Key {
        W = SDLK_W,
        A = SDLK_A,
        S = SDLK_S,
        D = SDLK_D
    };

    enum class MouseButton {
        Left = SDL_BUTTON_LEFT,
        Right = SDL_BUTTON_RIGHT,
        Middle = SDL_BUTTON_MIDDLE
    };

    static InputManager& Instance();

    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    void ProcessEvent(const SDL_Event& event);
    void Update();

    // Keyboard
    bool IsKeyDown(Key key) const;
    bool IsKeyUp(Key key) const;
    bool IsKeyPressed(Key key) const;

    // Mouse
    bool IsMouseDown(MouseButton btn) const;
    bool IsMouseUp(MouseButton btn) const;
    bool IsMousePressed(MouseButton btn) const;

    int GetMouseX() const { return m_mouseX; }
    int GetMouseY() const { return m_mouseY; }

    void SetInputDisabled(bool disabled) { m_inputDisabled = disabled; }
    bool IsInputDisabled() const { return m_inputDisabled; }

    void DisableAllInput() {
        m_keysDown.clear();
        m_keysUp.clear();
        m_keysPressed.clear();
        m_mouseDown.clear();
        m_mouseUp.clear();
        m_mousePressed.clear();
        m_inputDisabled = true;
    }

private:
    InputManager() = default;
    ~InputManager() = default;

    std::unordered_set<SDL_Keycode> m_keysDown;
    std::unordered_set<SDL_Keycode> m_keysUp;
    std::unordered_set<SDL_Keycode> m_keysPressed;

    std::unordered_set<unsigned int> m_mouseDown;
    std::unordered_set<unsigned int> m_mouseUp;
    std::unordered_set<unsigned int> m_mousePressed;

    int m_mouseX = 0;
    int m_mouseY = 0;

    bool m_inputDisabled = false;
    bool m_gameplayInputDisabled = false;
};