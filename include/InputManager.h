#pragma once
#include <SDL.h>
#include <unordered_map>
#include "GameMath.h"

class InputManager
{
public:
    InputManager();
    ~InputManager();

    // Call this at the start of every frame to reset "Just Pressed" states
    void Prepare();

    // Call this for every SDL event in the loop
    void HandleEvent(const SDL_Event &event);

    // Checks
    bool GetKeyDown(SDL_Keycode key);    // Returns true if held down
    bool GetKeyPressed(SDL_Keycode key); // Returns true only on the frame it was pressed
    bool GetKeyUp(SDL_Keycode key);      // Returns true only on the frame it was released

private:
    // We use two maps: one for current state, one for "previous frame" state if needed
    // or just track "down" and separate "pressed" logic.
    std::unordered_map<SDL_Keycode, bool> keyStates;
    std::unordered_map<SDL_Keycode, bool> lastFrameKeys;
};