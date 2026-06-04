# Rollercoaster Revolution 99

### An open-source re-implementation of the classic Digital Chocolate mobile game Rollercoaster Revolution: 99 Tracks.

---

## About

**Rollercoaster Revolution 99** is an open-source re-implementation of the 2009 Digital Chocolate mobile game. The original was a Java/Flash game where players navigated a rollercoaster train through 99 increasingly difficult tracks using just two buttons — accelerate and brake.

This project rebuilds the full game in C++20 with SDL2, targeting all major platforms: Windows, macOS, Linux, Web (via Emscripten), Android, and iOS. Every feature from the original is included: 6 track element types, 3 train variants, 5 power-ups, smile tokens, passenger happiness, crash physics, multiple game modes (Career, Survival, Hot Seat, Ghost Race), and a hidden level editor.

---

## Features

- **99 unique tracks** across Career mode with sequential progression
- **6 track element types**: loops, drops, jumps, cave tunnels, water splashes, falling tracks
- **2-button controls**: accelerate / brake with keyboard, gamepad, or touch
- **3 train types**: Light (fast but unstable), Medium (balanced), Heavy (stays grounded)
- **Smile Tokens**: speed-dependent collectibles with magnet auto-collect
- **5 power-ups**: nitro boost, sticky wheels, wings, magnet, slow motion
- **4 game modes**: Career, Survival (endless), Hot Seat (2-player turn-based), Ghost Race
- **Passenger happiness system**: balance speed for thrilled passengers; avoid ejections
- **Crash physics**: derailment, loop overshoot, falling track failures
- **Weather effects**: rain, snow, lightning per-level
- **Foreground parallax**: pillars and structures for depth and speed sensation
- **Rewritten from scratch**: C++20, SDL2, cross-platform
- **Hidden level editor**: dev tool for click-and-drag track creation

---

## Building

### Dependencies

- CMake 3.20+
- C++20 compiler (GCC 11+, Clang 14+, MSVC 2022+)
- SDL2, SDL2_image, SDL2_mixer, SDL2_ttf
- SQLite3

### Installing Dependencies

**Ubuntu / Debian:**
```bash
sudo apt install build-essential cmake libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev libsqlite3-dev
```

**Fedora:**
```bash
sudo dnf install gcc-c++ cmake SDL2-devel SDL2_image-devel SDL2_mixer-devel SDL2_ttf-devel sqlite-devel
```

**Arch Linux:**
```bash
sudo pacman -S base-devel cmake sdl2 sdl2_image sdl2_mixer sdl2_ttf sqlite
```

**macOS (Homebrew):**
```bash
brew install cmake sdl2 sdl2_image sdl2_mixer sdl2_ttf sqlite3
```

**Windows (vcpkg):**
```bash
vcpkg install sdl2 sdl2-image sdl2-mixer sdl2-ttf sqlite3
```

### Linux / macOS

```bash
git clone https://github.com/yourusername/Rollercoaster-Revolution-99
cd Rollercoaster-Revolution-99
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Windows

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Web (Emscripten)

```bash
emcmake cmake .. -DCMAKE_BUILD_TYPE=Release
emmake make
```

---

## Controls

| Action | Keyboard | Gamepad |
|--------|----------|---------|
| Accelerate | Left Arrow | D-Pad Left |
| Brake | Right Arrow | D-Pad Right |
| Pause | Escape / P | Start |
| Menu Navigate | Arrow Keys | D-Pad |
| Confirm | Enter / Space | A Button |
| Menu Back | Escape | B Button |

Controls are fully rebindable in the options menu. Touch controls with virtual buttons are available on mobile platforms.

---

## Project Structure

```
Rollercoaster-Revolution-99/
├── CMakeLists.txt              # Root build configuration
├── cmake/                      # CMake modules and toolchains
│   ├── CompilerFlags.cmake     # Warning flags, sanitizers, LTO
│   ├── FindSDL2.cmake          # SDL2 dependency locator
│   ├── Emscripten.cmake        # WebAssembly toolchain
│   ├── Android.cmake           # Android NDK toolchain
│   └── iOS.cmake               # iOS Xcode toolchain
├── src/
│   ├── main.cpp                # Entry point
│   ├── core/                   # Engine core: Game, Config, Random, EventBus
│   ├── db/                     # SQLite database manager
│   ├── physics/                # Coaster physics, track segments, train model
│   ├── track/                  # Track layout, builder, elements, validator
│   ├── game/                   # Game states, scoring, power-ups, progress
│   ├── input/                  # Keyboard, gamepad, touch input
│   ├── audio/                  # Music and SFX management
│   ├── render/                 # SDL2 rendering, sprites, camera, UI
│   ├── platform/               # Cross-platform file system abstraction
│   └── tools/                  # Level editor
├── assets/                     # Game assets (graphics, audio, track data)
├── resources/                  # App icon, version header template
└── test/                       # Unit tests
```

---

## Code Formatting

This project uses clang-format. Format all source files with:

```bash
find src/ -name '*.cpp' -o -name '*.h' | xargs clang-format -i
```

The `.clang-format` config at the project root defines the style (based on Google style with project-specific overrides).

## Contributing

Contributions are welcome! Please open an issue or pull request on GitHub.

This project is in active development. See [TODO.md](TODO.md) for the current implementation status.

---

## Licence

This project is licensed under the GNU General Public License v3.0. See [LICENCE](LICENCE) for details.

*Rollercoaster Revolution: 99 Tracks is the property of Digital Chocolate. This is a fan recreation and is not affiliated with or endorsed by the original copyright holders.*
