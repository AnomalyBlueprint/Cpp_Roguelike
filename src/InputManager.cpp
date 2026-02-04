#include "InputManager.h"

InputManager::InputManager() {}
InputManager::~InputManager() {}

void InputManager::Prepare()
{
    // Copy current state to last frame state before processing new events
    // This is a simple way to track "Just Pressed" vs "Held"
    lastFrameKeys = keyStates;
}

void InputManager::HandleEvent(const SDL_Event &event)
{
    if (event.type == SDL_KEYDOWN)
    {
        keyStates[event.key.keysym.sym] = true;
    }
    else if (event.type == SDL_KEYUP)
    {
        keyStates[event.key.keysym.sym] = false;
    }
}

bool InputManager::GetKeyDown(SDL_Keycode key)
{
    // Default to false if key not found
    if (keyStates.find(key) == keyStates.end())
        return false;
    return keyStates[key];
}

bool InputManager::GetKeyPressed(SDL_Keycode key)
{
    // It is pressed NOW, but was NOT pressed LAST FRAME
    return GetKeyDown(key) && !lastFrameKeys[key];
}

bool InputManager::GetKeyUp(SDL_Keycode key)
{
    // It is NOT pressed NOW, but WAS pressed LAST FRAME
    return !GetKeyDown(key) && lastFrameKeys[key];
}