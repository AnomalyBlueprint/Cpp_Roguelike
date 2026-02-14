# Cpp_Roguelike
A custom C++ game engine and roguelike built from scratch using SDL2.

## Credits

**Art Assets**
* **Micro Roguelike** by [Kenney.nl](https://kenney.nl/assets/micro-roguelike)
    * License: CC0 1.0 Universal (Public Domain)
* **Fantasy UI Borders** by [Kenney.nl](https://kenney.nl/assets/fantasy-ui-borders)
    * License: CC0 1.0 Universal (Public Domain)
* **UI Pack: Pixel Adventure** by [Kenney.nl](https://kenney.nl/assets/ui-pack-pixel-adventure)
    * License: CC0 1.0 Universal (Public Domain)
* **Kenney Fonts** by [Kenney.nl](https://kenney.nl/assets/kenney-fonts)
    * License: CC0 1.0 Universal (Public Domain)

## Tech Stack & Versions

* **Language**: C++17
* **Compiler**: Clang++ (via Makefile)
* **Library**: SDL2 (Minimum v2.0.18 required for `SDL_GetTicks64`)

## SDL2 Learning Tracker
A checklist of commonly used public SDL2 functions to track engine progress.

<details>
<summary><strong>1. Initialization & Core (2/6)</strong></summary>

- [x] SDL_Init
- [x] SDL_Quit
- [ ] SDL_InitSubSystem
- [ ] SDL_QuitSubSystem
- [ ] SDL_WasInit
- [ ] SDL_GetError

</details>

<details>
<summary><strong>2. Window Management (2/18)</strong></summary>

- [x] SDL_CreateWindow
- [x] SDL_DestroyWindow
- [ ] SDL_SetWindowTitle
- [ ] SDL_SetWindowIcon
- [ ] SDL_SetWindowPosition
- [ ] SDL_SetWindowSize
- [ ] SDL_SetWindowFullscreen
- [ ] SDL_GetWindowSurface
- [ ] SDL_UpdateWindowSurface
- [ ] SDL_GetWindowSize
- [ ] SDL_GetWindowPosition
- [ ] SDL_GetWindowFlags
- [ ] SDL_HideWindow
- [ ] SDL_ShowWindow
- [ ] SDL_MaximizeWindow
- [ ] SDL_MinimizeWindow
- [ ] SDL_RestoreWindow
- [ ] SDL_SetWindowBordered

</details>

<details>
<summary><strong>3. Rendering (2D Accelerated) (8/25)</strong></summary>

- [x] SDL_CreateRenderer
- [x] SDL_DestroyRenderer
- [x] SDL_RenderClear
- [x] SDL_RenderPresent
- [x] SDL_SetRenderDrawColor
- [x] SDL_RenderFillRect
- [x] SDL_RenderDrawRect
- [x] SDL_RenderCopy
- [ ] SDL_RenderCopyEx (Rotation/Flipping)
- [ ] SDL_SetRenderDrawBlendMode
- [ ] SDL_RenderDrawPoint
- [ ] SDL_RenderDrawLine
- [ ] SDL_RenderDrawLines
- [ ] SDL_RenderDrawPoints
- [ ] SDL_RenderFillRects
- [ ] SDL_RenderDrawRects
- [ ] SDL_GetRendererOutputSize
- [ ] SDL_CreateTexture
- [ ] SDL_SetRenderTarget (Render to Texture)
- [ ] SDL_GetRenderTarget
- [ ] SDL_RenderSetScale
- [ ] SDL_RenderSetViewport
- [ ] SDL_RenderSetClipRect
- [ ] SDL_RenderGetScale
- [ ] SDL_RenderGetViewport

</details>

<details>
<summary><strong>4. Textures (5/12)</strong></summary>

- [x] SDL_CreateTextureFromSurface
- [x] SDL_DestroyTexture
- [x] SDL_SetTextureColorMod
- [x] SDL_SetTextureAlphaMod
- [x] SDL_SetTextureBlendMode
- [ ] SDL_QueryTexture
- [ ] SDL_UpdateTexture
- [ ] SDL_LockTexture
- [ ] SDL_UnlockTexture
- [ ] SDL_GetTextureColorMod
- [ ] SDL_GetTextureAlphaMod
- [ ] SDL_GetTextureBlendMode

</details>

<details>
<summary><strong>5. Surface (CPU Rendering) (2/15)</strong></summary>

*Used primarily for loading images before converting to Textures.*
- [x] SDL_LoadBMP
- [x] SDL_FreeSurface
- [ ] SDL_CreateRGBSurface
- [ ] SDL_CreateRGBSurfaceWithFormat
- [ ] SDL_BlitSurface
- [ ] SDL_BlitScaled
- [ ] SDL_FillRect
- [ ] SDL_LockSurface
- [ ] SDL_UnlockSurface
- [ ] SDL_SetColorKey
- [ ] SDL_SetSurfaceColorMod
- [ ] SDL_SetSurfaceAlphaMod
- [ ] SDL_SetSurfaceBlendMode
- [ ] SDL_SaveBMP
- [ ] SDL_ConvertSurface

</details>

<details>
<summary><strong>6. Input & Events (1/20)</strong></summary>

- [x] SDL_PollEvent
- [ ] SDL_WaitEvent
- [ ] SDL_PumpEvents
- [ ] SDL_PushEvent
- [ ] SDL_GetKeyboardState
- [ ] SDL_GetModState
- [ ] SDL_GetMouseState
- [ ] SDL_GetRelativeMouseState
- [ ] SDL_GetMouseFocus
- [ ] SDL_ShowCursor
- [ ] SDL_HideCursor
- [ ] SDL_CreateCursor
- [ ] SDL_SetCursor
- [ ] SDL_FreeCursor
- [ ] SDL_GameControllerOpen
- [ ] SDL_GameControllerClose
- [ ] SDL_GameControllerGetAxis
- [ ] SDL_GameControllerGetButton
- [ ] SDL_JoystickOpen
- [ ] SDL_JoystickClose

</details>

<details>
<summary><strong>7. Timer & Time (1/5)</strong></summary>

- [x] SDL_GetTicks64
- [ ] SDL_GetTicks (32-bit legacy)
- [ ] SDL_Delay
- [ ] SDL_AddTimer
- [ ] SDL_RemoveTimer

</details>

<details>
<summary><strong>8. Audio (0/10)</strong></summary>

- [ ] SDL_OpenAudioDevice
- [ ] SDL_CloseAudioDevice
- [ ] SDL_PauseAudioDevice
- [ ] SDL_QueueAudio
- [ ] SDL_DequeueAudio
- [ ] SDL_GetQueuedAudioSize
- [ ] SDL_ClearQueuedAudio
- [ ] SDL_LoadWAV
- [ ] SDL_FreeWAV
- [ ] SDL_MixAudioFormat

</details>

<details>
<summary><strong>9. Filesystem & Platform (0/5)</strong></summary>

- [ ] SDL_GetBasePath
- [ ] SDL_GetPrefPath
- [ ] SDL_GetPlatform
- [ ] SDL_GetCPUCount
- [ ] SDL_GetSystemRAM

</details>