# Rollercoaster Revolution 99 — Project TODO

Full re-creation of the Digital Chocolate Flash/Java game using SDL2, targeting all platforms (Windows, macOS, Linux, Web via Emscripten, Android, iOS). Every feature from the original is included.

---

## Original Game Feature Checklist

All features from *Rollercoaster Revolution: 99 Tracks* that must be implemented:

- [ ] **99 unique tracks** across Career mode
- [ ] **6 track element types**: loops, drops, jumps, cave tunnels, water splashes (speed reduction), falling tracks
- [ ] **2-button controls**: accelerate (left arrow) / brake (right arrow)
- [ ] **3 train types**: light (gains speed fast, flies off tracks easily), medium (balanced), heavy (stays grounded, struggles with loops)
- [ ] **Train-per-level selection**: some levels require a specific train type to complete or collect all tokens
- [ ] **Smile Tokens**: primary collectibles used to unlock new tracks and achievements; placed at speed-dependent positions; require the right speed to reach (or magnet power-up to auto-collect)
- [ ] **5 power-ups**: nitro boost (burst of speed for loops/gaps), sticky wheels (stay on track at any speed), wings (glide through air on jumps), magnet (attracts nearby smile tokens), slow motion (slows game speed, controls remain responsive)
- [ ] **Career mode**: sequential level progression with instant restart on failure
- [ ] **Survival mode**: endless progression with increasing difficulty; no instant restart (loading screen between attempts)
- [ ] **Hot Seat multiplayer**: two players take turns on the same track, competing on performance/score
- [ ] **Ghost Race mode**: race against a ghost recording of a previous run or opponent
- [ ] **Passenger happiness system**: too fast = crash, too slow = miserable passengers; balance is key
- [ ] **Passenger loss mechanics**: hard bumps and near-miss derailments eject passengers; fewer passengers = lower score multiplier
- [ ] **Speed-dependent token collection**: tokens only collectible within a specific speed window
- [ ] **Crash detection**: derailment on jumps, loop overshoot, falling track failures
- [ ] **Colourful visuals**: smart, vibrant art style with animated track elements
- [ ] **Foot-tapping music**: upbeat soundtrack per level or theme
- [ ] **Sound effects**: crash, cheer, token collect, power-up activate, brake squeal, splash
- [ ] **Loading screens**: between levels and on Survival mode restart
- [ ] **Level select screen**: grid or list of 99 tracks with completion status
- [ ] **Progress saving**: unlocked levels, collected tokens, high scores persisted to disk
- [ ] **Gamepad support**: in addition to keyboard controls
- [ ] **Haptic feedback**: gamepad rumble on crashes, nitro boosts, lift hill clank-clank
- [ ] **SQLite persistence**: local database for scores, ghost runs (BLOB), settings, progress; FS.syncfs for Emscripten IndexedDB
- [ ] **Level editor**: hidden dev tool for click-and-drag track creation, token/power-up placement, play-testing
- [ ] **Weather effects**: rain, snow (visual polish per level theme)
- [ ] **Foreground parallax**: pillars, trees, structures passing in front of track for depth and speed sensation
- [ ] **Aspect ratio handling**: logical resolution, letterboxing/pillarboxing, anchor-based HUD for 16:9, 21:9, 4:3

---

## Project Structure

```
Rollercoaster-Revolution-99/
├── CMakeLists.txt
├── cmake/
│   ├── CompilerFlags.cmake
│   ├── FindSDL2.cmake
│   └── Emscripten.cmake
├── assets/
│   ├── audio/
│   │   ├── music/
│   │   └── sfx/
│   ├── graphics/
│   │   ├── trains/
│   │   ├── tracks/
│   │   ├── backgrounds/
│   │   ├── foreground/
│   │   ├── tokens/
│   │   ├── powerups/
│   │   ├── weather/
│   │   └── ui/
│   └── tracks/
├── src/
│   ├── main.cpp
│   ├── core/
│   │   ├── Game.h / Game.cpp
│   │   ├── Config.h / Config.cpp
│   │   ├── Random.h / Random.cpp
│   │   └── EventBus.h / EventBus.cpp
│   ├── db/
│   │   ├── DatabaseManager.h / DatabaseManager.cpp
│   │   └── Schema.sql
│   ├── physics/
│   │   ├── CoasterPhysics.h / CoasterPhysics.cpp
│   │   ├── TrackSegment.h / TrackSegment.cpp
│   │   └── Train.h / Train.cpp
│   ├── track/
│   │   ├── TrackBuilder.h / TrackBuilder.cpp
│   │   ├── TrackLayout.h / TrackLayout.cpp
│   │   ├── TrackElements.h / TrackElements.cpp
│   │   └── TrackValidator.h / TrackValidator.cpp
│   ├── game/
│   │   ├── GameState.h / GameState.cpp
│   │   ├── MenuState.h / MenuState.cpp
│   │   ├── PlayState.h / PlayState.cpp
│   │   ├── SurvivalState.h / SurvivalState.cpp
│   │   ├── CareerState.h / CareerState.cpp
│   │   ├── HotSeatState.h / HotSeatState.cpp
│   │   ├── GhostRaceState.h / GhostRaceState.cpp
│   │   ├── ScoreManager.h / ScoreManager.cpp
│   │   ├── PowerUpManager.h / PowerUpManager.cpp
│   │   ├── HappinessSystem.h / HappinessSystem.cpp
│   │   ├── PassengerSystem.h / PassengerSystem.cpp
│   │   ├── ProgressManager.h / ProgressManager.cpp
│   │   └── TrainSelector.h / TrainSelector.cpp
│   ├── input/
│   │   ├── InputManager.h / InputManager.cpp
│   │   └── InputState.h / InputState.cpp
│   ├── audio/
│   │   ├── AudioManager.h / AudioManager.cpp
│   │   └── SoundEffect.h / SoundEffect.cpp
│   ├── render/
│   │   ├── Renderer.h / Renderer.cpp
│   │   ├── Sprite.h / Sprite.cpp
│   │   ├── SpriteSheet.h / SpriteSheet.cpp
│   │   ├── Camera.h / Camera.cpp
│   │   ├── ParticleSystem.h / ParticleSystem.cpp
│   │   ├── WeatherSystem.h / WeatherSystem.cpp
│   │   └── ui/
│   │       ├── UIScreen.h / UIScreen.cpp
│   │       ├── HUD.h / HUD.cpp
│   │       ├── MenuScreen.h / MenuScreen.cpp
│   │       ├── LevelSelectScreen.h / LevelSelectScreen.cpp
│   │       └── LoadingScreen.h / LoadingScreen.cpp
│   ├── platform/
│   │   ├── FileSystem.h / FileSystem.cpp
│   │   ├── EmscriptenFS.h / EmscriptenFS.cpp
│   │   ├── AndroidFS.h / AndroidFS.cpp
│   │   └── iOSFS.h / iOSFS.cpp
│   └── tools/
│       └── LevelEditor.h / LevelEditor.cpp
├── test/
│   ├── test_physics.cpp
│   ├── test_track.cpp
│   ├── test_scoring.cpp
│   ├── test_happiness.cpp
│   ├── test_progress.cpp
│   └── test_database.cpp
└── resources/
    ├── icon.png
    └── version.h.in
```

---

## File Descriptions

### Build System

| File | Purpose |
|------|---------|
| `CMakeLists.txt` | Root build configuration. Defines project, finds SDL2/SDL_mixer/SDL_image/SDL_ttf, adds subdirectories, sets output targets for desktop, Emscripten, Android (via SDL2 Android CMake), and iOS (via SDL2 Xcode toolchain). |
| `cmake/CompilerFlags.cmake` | Compiler warning flags (`-Wall -Wextra -Wpedantic`), sanitizer configs (ASan, UBSan), C++20 standard enforcement, link-time optimization for release. |
| `cmake/FindSDL2.cmake` | Locates SDL2, SDL_mixer, SDL_image, SDL_ttf libraries and headers across platforms; handles Emscripten special cases. |
| `cmake/Emscripten.cmake` | Toolchain-specific settings for WebAssembly builds: memory limits, asyncify for file loading, shell file for canvas sizing. |
| `cmake/Android.cmake` | Android-specific settings: NDK toolchain, ABI filters (arm64-v8a, armeabi-v7a, x86_64), minSdkVersion, asset packaging via APK, JNI initialization for SDL2 Android activity lifecycle. |
| `cmake/iOS.cmake` | iOS-specific settings: Xcode toolchain, minimum deployment target, asset bundling, orientation lock (landscape), SDL2 iOS video driver configuration, touch input mapping to game actions. |

### Assets

| Directory | Purpose |
|-----------|---------|
| `assets/audio/music/` | Background music tracks — upbeat, foot-tapping tunes. One per theme or shared across levels. |
| `assets/audio/sfx/` | Sound effects: crash, passenger cheer, token collect, nitro boost roar, sticky wheels clank, wings whoosh, magnet hum, slow-motion warp, brake squeal, water splash, tunnel echo, falling track rumble. |
| `assets/graphics/trains/` | Sprite sheets for all 3 train types (light, medium, heavy) with animation frames for each orientation. |
| `assets/graphics/tracks/` | Track segment sprites: straight, curve, loop, drop, jump, tunnel entrance/exit, water splash, falling track (intact/broken). |
| `assets/graphics/backgrounds/` | Parallax background layers per level theme (skyline, mountains, clouds, trees). |
| `assets/graphics/foreground/` | Foreground parallax elements: pillars, trees, rock formations, support beams that pass in front of the track for depth and speed sensation. |
| `assets/graphics/tokens/` | Smile token sprite with collection animation (spin, glow, absorb). |
| `assets/graphics/powerups/` | Sprites for all 5 power-up types: nitro boost (flame/lightning icon), sticky wheels (magnetized wheel icon), wings (winged icon), magnet (horseshoe magnet icon), slow motion (hourglass/clock icon). Each with pickup and active-state animations. |
| `assets/graphics/weather/` | Weather effect sprites: rain drops, snowflakes, lightning flashes. Used by `WeatherSystem` for per-level atmospheric polish. |
| `assets/graphics/powerups/` | Sprites for all 5 power-up types: nitro boost (flame/lightning icon), sticky wheels (magnetized wheel icon), wings (winged icon), magnet (horseshoe magnet icon), slow motion (hourglass/clock icon). Each with pickup and active-state animations. |
| `assets/graphics/ui/` | UI elements: buttons, panels, progress bars, score displays, level select grid tiles. |
| `assets/tracks/` | Track layout definitions for all 99 levels (JSON format). Each file defines segment sequence, train requirements, token positions, power-up placements (nitro/sticky/wings/magnet/slow-mo), hazards, weather settings, and passenger count. |

### Database

| File | Purpose |
|------|---------|
| `db/DatabaseManager.h/.cpp` | SQLite wrapper with prepared statement caching, migration system, and connection pooling. Handles all persistent data: level progress, high scores, ghost run BLOBs (serialized input timestamps), settings, achievements. On Emscripten, calls `FS.syncfs()` after writes to persist to IndexedDB. |
| `db/Schema.sql` | Database schema definition. Tables: `levels` (id, unlocked, best_score, tokens_collected, stars), `scores` (level_id, score, date, train_type), `ghost_runs` (level_id, mode, input_data BLOB, timestamp), `settings` (key, value), `achievements` (id, unlocked, date). Migrations tracked via `schema_version` table. |

### Core Engine

| File | Purpose |
|------|---------|
| `main.cpp` | Entry point. Initializes SDL subsystems (video, audio, mixer, image, ttf), creates `Game` instance, runs the main loop, handles graceful shutdown and cleanup. |
| `core/Game.h/.cpp` | Central game loop with fixed-timestep `Tick()` (physics/logic at 60Hz, adjustable for slow-motion mode which halves the tick rate) and variable `Draw()`. Manages the `GameState` stack (push/pop for menus, pause overlay, level editor). Handles pause, quit, window focus, and resolution change events. |
| `core/Config.h/.cpp` | Loads/saves player settings: resolution, fullscreen, logical resolution mode (letterbox/pillarbox/stretch), volume (music/SFX), keybindings (keyboard + gamepad), haptic intensity, language preference. Serializes to/from a platform-appropriate config file. |
| `core/Random.h/.cpp` | Deterministic PRNG (PCG or Xoshiro256). Seeded per-level for reproducible physics outcomes, ghost race recordings, and any procedural content. Enables replay consistency. |
| `core/EventBus.h/.cpp` | Lightweight event system for decoupled communication between subsystems. Events: `TrainCrashed`, `TokenCollected`, `PowerUpActivated`, `PowerUpExpired`, `LevelCompleted`, `PassengerMoodChanged`, `PassengerEjected`, `SlowMotionToggled`, `WeatherChanged`. Subscribers react without direct coupling. |

### Physics

| File | Purpose |
|------|---------|
| `physics/CoasterPhysics.h/.cpp` | Core simulation engine: velocity integration, gravity, acceleration/deceleration curves, G-force calculation per car, crash detection thresholds. Handles derailment (too fast on jumps/curves), loop overshoot (too slow), falling track failures, nitro boost impulse application, and passenger ejection triggers (excessive G-force, near-miss derailment, hard bump on water splash). |
| `physics/TrackSegment.h/.cpp` | Defines all 6 segment types with physical properties: height delta, length, friction coefficient, minimum/maximum safe speed, crash type on violation, G-force multiplier (for passenger ejection calculation). Types: straight, curve, loop, drop, jump, tunnel, water splash (speed reduction + bump force), falling track. |
| `physics/Train.h/.cpp` | Multi-car train model: linked list of 3-4 cars with distance constraints (Car N follows Car N-1's exact path with fixed spacing). "Worm" physics ensure realistic loop traversal where front cars may clear while rear cars are still climbing. Partial derailment possible (rear cars fly off while front stays on). Three train variants: Light (high acceleration, low stability, crashes easily on loops/jumps), Medium (balanced), Heavy (low acceleration, high stability, fails loops if too slow, stays grounded on drops). Each has mass, drag, max speed, grip, and car count values. |

### Track System

| File | Purpose |
|------|---------|
| `track/TrackBuilder.h/.cpp` | Assembles `TrackSegment` sequences from JSON definitions into complete `TrackLayout` instances. Supports both hand-crafted (99 career levels) and procedural generation (Survival mode endless tracks). |
| `track/TrackLayout.h/.cpp` | Data structure representing a full track as an ordered sequence of segments with metadata (level number, theme, par time, required train type). Provides forward/backward iteration, segment lookup by index, and total length calculation. |
| `track/TrackElements.h/.cpp` | Manages interactive elements placed along the track: smile tokens (with speed window for collection), power-up pickups (5 types: nitro boost, sticky wheels, wings, magnet, slow motion), hazards (falling track trigger points). Each element has a position, activation condition, and visual state. |
| `track/TrackValidator.h/.cpp` | Validates track layouts for completeness: no gaps between segments, reachable finish line, at least one valid train type can complete the track, all tokens are reachable at some speed, no impossible loops. Run at load time to catch bad track files. |

### Gameplay

| File | Purpose |
|------|---------|
| `game/GameState.h/.cpp` | Abstract base class for all game states. Defines `Enter()`, `Exit()`, `Tick(float dt)`, `Draw(Renderer&)`, `HandleInput(const InputState&)` interface. Manages state lifecycle and transitions. |
| `game/MenuState.h/.cpp` | Main menu state: title screen with animated logo, buttons for Career/Survival/Hot Seat/Ghost Race, options entry (resolution, volume, keybindings, haptic intensity, logical resolution mode), credits. Background shows a looping demo track with weather effects. Hidden level editor access via config flag or secret key combination. |
| `game/PlayState.h/.cpp` | Core active gameplay state. Coordinates physics tick, input processing (accelerate/brake), token collection, power-up activation (nitro/sticky/wings/magnet/slow-mo), crash detection, passenger ejection on hard bumps, and level completion/failure. Shared base for all play modes. |
| `game/SurvivalState.h/.cpp` | Survival mode: endless track generation with increasing difficulty (steeper drops, tighter loops, more falling sections, fewer power-ups, adverse weather). No instant restart — shows loading screen between attempts. Score accumulates across consecutive completed tracks. |
| `game/CareerState.h/.cpp` | Career mode: sequential progression through all 99 hand-crafted levels. Instant restart on crash (no loading screen). Tracks which levels are unlocked, best scores, and token collection per level. |
| `game/HotSeatState.h/.cpp` | Hot Seat multiplayer: two players take turns on the same track. Player 1 runs, then Player 2 runs. Scores compared at end. Tracks current player, scores, and turn transitions. |
| `game/GhostRaceState.h/.cpp` | Ghost Race mode: player races against a ghost recording of a previous run (own best time or another player's). Ghost train rendered semi-transparently. Records input timestamps for replay. |
| `game/ScoreManager.h/.cpp` | Calculates score from multiple factors: speed balance bonus (staying in the "sweet spot"), smile tokens collected (manual + magnet-assisted), time to complete, power-up usage efficiency (strategic nitro/wings timing), passenger count multiplier (fewer passengers = lower multiplier). Produces star rating (1-3) per level. |
| `game/PowerUpManager.h/.cpp` | Manages all 5 power-ups: nitro boost (instant speed burst for loops/gaps), sticky wheels (ignore derailment for duration), wings (glide through airborne segments), magnet (auto-attract nearby smile tokens), slow motion (reduced game speed, responsive controls). Handles pickup, active duration, stacking rules (only one active at a time), visual/sfx feedback, and expiration. |
| `game/HappinessSystem.h/.cpp` | Passenger happiness model: maps current train speed to a happiness curve. Too fast → fear/crash; too slow → boredom/miserable; sweet spot → thrilled/smiling. Happiness affects end-of-level score and visual passenger reactions (arms up, screaming, bored faces). |
| `game/PassengerSystem.h/.cpp` | Manages individual passenger count and ejection logic. Hard bumps, near-miss derailments, and excessive G-forces can eject passengers mid-ride. Ejected passengers fly off with particle effects. Remaining passenger count acts as a score multiplier (fewer passengers = lower multiplier). Initial passenger count varies by train type (light = 6, medium = 8, heavy = 10). |
| `game/ProgressManager.h/.cpp` | Persists player progress via `DatabaseManager`: unlocked levels (complete level N to unlock N+1), best scores per level, total smile tokens collected, achievements. Saves to SQLite on level completion and loads on startup. On Emscripten, triggers `FS.syncfs()` after each save to persist to IndexedDB. |
| `game/TrainSelector.h/.cpp` | UI and logic for selecting train type before a level. Shows stats comparison (acceleration, stability, grip). Highlights required train for levels that mandate one. Remembers last-used selection per level. |

### Input

| File | Purpose |
|------|---------|
| `input/InputManager.h/.cpp` | SDL event processing for keyboard and gamepad. Maps raw events to game actions (accelerate, brake, pause, menu navigate). Supports full key rebinding and gamepad button/axis mapping via `Config`. Handles input buffering for responsive controls. Manages gamepad haptic feedback (rumble): crash impact (strong burst), nitro boost (continuous vibration), lift hill clank-clank (rhythmic pulse), near-miss derailment (short jolt). |
| `input/InputState.h/.cpp` | Frame-level snapshot of input: `accelerating` (bool), `braking` (bool), `pause_pressed` (bool), `menu_up/down/confirm` (bool). Decoupled from raw SDL events so tick logic is deterministic and testable. Input remains responsive during slow-motion mode. |

### Audio

| File | Purpose |
|------|---------|
| `audio/AudioManager.h/.cpp` | SDL_mixer wrapper. Manages music playback (streaming, fade in/out, track switching per level theme), SFX channels (positioned or global), master volume, music volume, SFX volume. Handles audio device initialization and error recovery. On Emscripten, implements audio context resume on first user interaction (browser autoplay policy blocking). Provides `Resume()` method called on first click/key press. |
| `audio/SoundEffect.h/.cpp` | Individual sound definitions with metadata: crash (loud impact + debris), cheer (passenger excitement), token collect (chime), nitro boost (engine roar), sticky wheels (clank), wings (whoosh), magnet (hum/ping), slow motion (warp-down), brake squeal, water splash, tunnel echo reverb, falling track rumble. Each loads its sample from `assets/audio/sfx/`. |

### Rendering

| File | Purpose |
|------|---------|
| `render/Renderer.h/.cpp` | SDL2 render context. Manages texture loading, sprite batching for performance, draw ordering (background → track → train → foreground → elements → UI → weather), coordinate transforms (world to screen), and render target management. Uses `SDL_RenderSetLogicalSize` for consistent rendering across resolutions with letterboxing/pillarboxing. Supports hardware acceleration fallback. |
| `render/Sprite.h/.cpp` | Single sprite abstraction: loads from PNG, provides draw method with position, rotation, scale, color tint, and alpha. Used for UI elements, tokens, power-ups. |
| `render/SpriteSheet.h/.cpp` | Sprite sheet manager: loads a large texture, defines frame rectangles, provides animation playback (frame index, speed, loop/ping-pong). Used for train animations, token spin, power-up glow, background parallax layers. |
| `render/Camera.h/.cpp` | Follows the train along the track with smooth interpolation. Dynamic zoom based on speed and track context: zooms out during massive drops and high-speed sections to show scale, zooms in during slow technical sections and cave tunnels for detail. Look-ahead offset toward direction of travel so player can see upcoming track elements. Handles horizontal/vertical scrolling, parallax background layers (multiple depth levels), and foreground parallax elements. Keeps train centered with configurable lead distance. |
| `render/ParticleSystem.h/.cpp` | GPU-friendly particle system for visual effects: water splash droplets, crash sparks/debris, token collection sparkles, nitro flame trail, sticky wheel sparks, wing glide aura, magnet pulse ring, slow-motion screen warp, train exhaust/smoke, passenger ejection trajectories. Manages particle lifetime, velocity, gravity, fade, and pooling for zero-allocation during gameplay. |
| `render/WeatherSystem.h/.cpp` | Renders weather effects per level: rain (angled drops with splash particles), snow (drifting flakes with accumulation), lightning (random flashes with thunder SFX delay). Configured per track layout file. Uses particle pooling for performance. Adds atmospheric polish without affecting gameplay physics. |
| `render/ui/UIScreen.h/.cpp` | Base class for UI screens. Provides layout system (anchors, padding), widget management (buttons, labels, progress bars), and transition animations (fade in/out, slide). |
| `render/ui/HUD.h/.cpp` | In-game HUD overlay with anchor-based layout for variable aspect ratios (16:9, 21:9, 4:3). Elements anchored to corners: speedometer (bottom-left, visual gauge + numeric), smile token counter (top-left, with collection animation), passenger happiness meter (top-center, face icon + bar), passenger count (top-right, icon + number, decreases on ejection), active power-up indicator with icon and countdown timer (bottom-center), level number (top-right), pause button (top-right corner). Uses `SDL_RenderSetLogicalSize` for consistent scaling across resolutions. |
| `render/ui/MenuScreen.h/.cpp` | Menu UI components: animated title, styled buttons with hover/press states, mode selection cards (Career/Survival/Hot Seat/Ghost Race), settings panel with sliders and keybinding UI, credits scroll. |
| `render/ui/LevelSelectScreen.h/.cpp` | Level select grid showing all 99 tracks. Each tile displays level number, completion status (stars), best score, token count. Locked levels shown with a padlock. Scrollable grid with page navigation. |
| `render/ui/LoadingScreen.h/.cpp` | Loading screen displayed between levels (always in Survival mode, optionally in Career). Shows progress bar, level preview or tip text, animated coaster graphic. Hides asset loading stutter. |

### Platform

| File | Purpose |
|------|---------|
| `platform/FileSystem.h/.cpp` | Cross-platform file path resolution. Determines correct directories per OS: `AppData` (Windows), `~/Library/Application Support` (macOS), `~/.config` (Linux), internal storage (Android), `Documents` (iOS), asset bundle paths (Emscripten virtual filesystem). Provides `OpenFile`, `ReadAllText`, `WriteAllText`, `DirectoryExists`, `EnsureDirectory`. |
| `platform/EmscriptenFS.h/.cpp` | Emscripten-specific file system management. Wraps `FS.syncfs()` for IndexedDB persistence — called after every database write and settings save to ensure progress survives page refresh. Handles virtual filesystem mounting (`MEMFS` → `IDBFS`) for save directory. Provides `SyncToIndexedDB()` async method with callback for save confirmation. |
| `platform/AndroidFS.h/.cpp` | Android-specific file system handling. Manages asset extraction from APK (reads via `AAssetManager`), save path resolution to internal storage (`getFilesDir()`), and external storage permissions (if needed for screenshots). Handles SDL2 Android activity lifecycle events (pause/resume) for audio context and renderer restoration. |
| `platform/iOSFS.h/.cpp` | iOS-specific file system handling. Resolves save paths to `Documents` directory (iCloud-excluded), reads assets from app bundle. Handles iOS lifecycle events (background/foreground) for renderer context loss/recovery, audio session interruption, and metal/OpenGL state restoration. |

### Tools

| File | Purpose |
|------|---------|
| `tools/LevelEditor.h/.cpp` | Hidden dev tool (enabled via command-line flag or config) for creating and editing track layouts. Block-based drag-and-drop on a 2D grid: segment palette (straight, curve, drop, jump, loop, tunnel, water splash, falling track) on left sidebar, main canvas with grid snapping, connection-point validation (green = valid, red = mismatch). Element palette for placing smile tokens, 5 power-ups, and hazards onto specific segments. Properties panel on click: edit segment height/friction/speed limits, token speed windows, hazard thresholds. Live play-test button runs the track with selected train type without leaving editor. Train type dropdown for preview with light/medium/heavy. Auto-validation via `TrackValidator` before save. Exports to JSON format for `assets/tracks/`. Dramatically reduces the effort of authoring 99 hand-crafted levels compared to manual JSON editing. |

### Tests

| File | Purpose |
|------|---------|
| `test/test_physics.cpp` | Unit tests for `CoasterPhysics`: velocity under gravity, acceleration/braking curves, nitro boost impulse, crash thresholds (derailment speed, loop minimum speed), G-force calculations, train mass effects on momentum. |
| `test/test_track.cpp` | Unit tests for `TrackLayout` and `TrackBuilder`: segment connectivity validation, token placement reachability, level completeness checks, JSON parsing correctness, `TrackValidator` rule enforcement. |
| `test/test_scoring.cpp` | Unit tests for `ScoreManager`: speed bonus calculation, token scoring (manual vs magnet-assisted), time bonus, star rating thresholds, power-up efficiency scoring, edge cases (zero tokens, instant crash, perfect run, all 5 power-ups used). |
| `test/test_happiness.cpp` | Unit tests for `HappinessSystem`: happiness curve mapping, speed-to-mood transitions, thresholds for thrilled/neutral/miserable, score impact from happiness level. |
| `test/test_progress.cpp` | Unit tests for `ProgressManager`: save/load cycle, level unlock progression, score persistence, token count aggregation, file corruption handling, missing file defaults. |
| `test/test_database.cpp` | Unit tests for `DatabaseManager`: schema migration (v1 → v2 → v3), CRUD operations on all tables, ghost run BLOB serialization/deserialization, concurrent write safety, Emscripten FS.syncfs mock, corrupted database recovery, empty database initialization. |

### Resources

| File | Purpose |
|------|---------|
| `resources/icon.png` | Application icon for window manager, desktop shortcut, and executable embedding (Windows `.rc`, macOS `.icns`, Linux `.desktop`). |
| `resources/version.h.in` | CMake template for auto-generated version header. Populated with `RR99_VERSION_MAJOR`, `RR99_VERSION_MINOR`, `RR99_VERSION_PATCH`, and git commit hash for display in credits and crash reports. |

---

## Implementation Phases

### Phase 1 — Foundation
- [ ] CMake build system with SDL2, SDL_mixer, SDL_image, SDL_ttf, SQLite3
- [ ] Emscripten toolchain configuration with IDBFS support
- [ ] Android NDK toolchain configuration with APK asset packaging
- [ ] iOS Xcode toolchain configuration with app bundle asset inclusion
- [ ] Compiler flags, ASan, UBSan enabled
- [ ] `core/Game` — fixed-timestep game loop with state stack, slow-motion tick adjustment
- [ ] `core/Config` — settings load/save with keybinding support
- [ ] `core/Random` — deterministic PRNG
- [ ] `core/EventBus` — decoupled event system
- [ ] `db/DatabaseManager` — SQLite wrapper with migrations, prepared statement caching
- [ ] `db/Schema.sql` — initial schema (levels, scores, ghost_runs, settings, achievements)
- [ ] `platform/EmscriptenFS` — FS.syncfs for IndexedDB persistence, audio context resume hook
- [ ] `platform/AndroidFS` — APK asset extraction, internal storage save paths, activity lifecycle handling
- [ ] `platform/iOSFS` — bundle asset reading, Documents directory saves, lifecycle event handling
- [ ] `render/Renderer` — SDL2 window, `SDL_RenderSetLogicalSize`, texture management, sprite batching
- [ ] `render/Sprite` / `render/SpriteSheet` — sprite loading and animation
- [ ] `input/InputManager` — keyboard + gamepad input with rebinding, haptic feedback (rumble), touch input mapping for mobile
- [ ] `input/InputState` — frame-level input snapshot
- [ ] `platform/FileSystem` — cross-platform asset and save paths
- [ ] `resources/version.h.in` — auto-generated version header

### Phase 2 — Physics & Track
- [ ] `physics/TrackSegment` — all 6 segment types with physical properties
- [ ] `physics/Train` — light/medium/heavy train models, multi-car linked-list with distance constraints ("worm" physics), partial derailment support
- [ ] `physics/CoasterPhysics` — velocity, gravity, nitro boost impulse, crash detection (derailment, loop overshoot, falling track), G-force per car
- [ ] `track/TrackLayout` — track data structure with iteration and metadata
- [ ] `track/TrackBuilder` — JSON loading for hand-crafted tracks + procedural generation for Survival
- [ ] `track/TrackElements` — smile tokens (speed-window collection), 5 power-ups (nitro, sticky wheels, wings, magnet, slow motion), hazards (falling track triggers)
- [ ] `track/TrackValidator` — load-time validation (connectivity, reachability, completable)
- [ ] `tools/LevelEditor` — 2D grid canvas, block-based drag-and-drop segment palette, connection-point validation, element placement, properties panel, live play-test, train preview dropdown, JSON export

### Phase 3 — Core Gameplay Loop
- [ ] `game/GameState` — state machine with Enter/Exit/Tick/Draw/HandleInput
- [ ] `game/PlayState` — active gameplay: physics, input, tokens, power-ups, crash/complete detection
- [ ] `game/TrainSelector` — pre-level train selection UI with stat comparison
- [ ] `game/HappinessSystem` — passenger happiness curve (speed → mood)
- [ ] `game/PassengerSystem` — passenger count, ejection on hard bumps/near-misses, score multiplier
- [ ] `game/ScoreManager` — multi-factor scoring (speed, tokens, time, happiness, passenger count multiplier)
- [ ] `game/PowerUpManager` — 5 power-ups: nitro boost, sticky wheels, wings, magnet, slow motion; single-active rule, duration timers, visual/SFX feedback
- [ ] `render/Camera` — follow train, dynamic zoom (out on drops, in on tunnels), look-ahead, parallax backgrounds + foreground
- [ ] `render/ParticleSystem` — splash, sparks, sparkles, smoke, passenger ejection trajectories
- [ ] `render/WeatherSystem` — rain, snow, lightning per-level weather effects
- [ ] `render/ui/HUD` — anchor-based layout, speedometer, token counter, happiness meter, passenger count, power-up timer, pause button

### Phase 4 — Game Modes
- [ ] `game/MenuState` — title screen, mode selection, options, credits, demo track background
- [ ] `game/CareerState` — 99 sequential levels, instant restart, level unlock tracking
- [ ] `game/SurvivalState` — endless procedural tracks, increasing difficulty, loading screen on restart
- [ ] `game/HotSeatState` — two-player turn-based competition on same track
- [ ] `game/GhostRaceState` — ghost recording playback, semi-transparent ghost train, input timestamp recording, ghost BLOB storage in SQLite
- [ ] `game/ProgressManager` — persist unlocks, scores, tokens, achievements via `DatabaseManager`; FS.syncfs on Emscripten
- [ ] `render/ui/LevelSelectScreen` — 99-level grid with stars, scores, lock states
- [ ] `render/ui/LoadingScreen` — progress bar, level preview, animated graphic

### Phase 5 — Audio & Polish
- [ ] `audio/AudioManager` — music streaming, SFX channels, volume controls, fade transitions, Emscripten audio context resume
- [ ] `audio/SoundEffect` — all SFX definitions (crash, cheer, token, nitro, sticky wheels, wings, magnet, slow-mo, brake, splash, tunnel, rumble, thunder)
- [ ] Train sprite animations for all 3 types and orientations
- [ ] Track element animations (falling track collapse, water splash, tunnel lighting)
- [ ] Token collection animation (spin, glow, absorb; magnet pull trajectory)
- [ ] Power-up visual effects (nitro flame trail, sticky wheel sparks, wing glide aura, magnet pulse ring, slow-motion screen warp)
- [ ] Passenger visual reactions (arms up when thrilled, scared faces on drops, bored when slow, ejection physics)
- [ ] Slow-motion screen effect (desaturated colors, motion blur, warped time visual)
- [ ] Weather effects (rain drops, snowflakes, lightning flashes with thunder delay)
- [ ] Foreground parallax elements (pillars, trees, support beams passing in front of track)
- [ ] Haptic feedback (gamepad rumble: crash burst, nitro vibration, lift hill clank, near-miss jolt)
- [ ] Menu transitions (fade, slide), screen shake on crash and high G-force
- [ ] Foot-tapping background music tracks

### Phase 6 — Testing & Release
- [ ] `test/test_physics.cpp` — physics unit tests (gravity, acceleration, nitro impulse, crash thresholds, G-force, multi-car constraints)
- [ ] `test/test_track.cpp` — track validation tests
- [ ] `test/test_scoring.cpp` — scoring logic tests (including passenger count multiplier)
- [ ] `test/test_happiness.cpp` — happiness curve tests
- [ ] `test/test_progress.cpp` — save/load persistence tests
- [ ] `test/test_database.cpp` — SQLite migration, CRUD, ghost BLOB, Emscripten sync, corruption recovery
- [ ] All 99 track layout files authored and validated (using LevelEditor tool)
- [ ] Emscripten WebAssembly build with HTML5 shell, IndexedDB persistence, audio resume
- [ ] Packaging: Windows installer, macOS `.app`/`.dmg`, Linux `.deb`/Flatpak, Android APK/AAB, iOS `.ipa`
- [ ] Performance profiling and optimization (sprite batching, object pooling, cache-friendly layouts, particle pooling)
- [ ] Aspect ratio testing (16:9, 21:9, 4:3, mobile portrait/landscape) — HUD anchoring, letterboxing, logical resolution
- [ ] Touch input testing on Android and iOS devices (virtual buttons for accelerate/brake)
- [ ] Final playtesting pass across all modes and difficulty curves
