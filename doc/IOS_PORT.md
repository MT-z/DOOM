# DOOM iOS Port Guide

## Overview

This is a port of the original DOOM engine to iOS, running on the iOS Simulator (arm64) using a statically linked SDL2. It shares the SDL2 video/audio backend (`i_video_sdl.c`, `i_sound_sdl.c`) with the macOS port and adds an iOS-specific entry point.

- Target: iOS Simulator, arm64, iOS 13.0+
- Bundle: `ios/build/Doom.app` (ad-hoc signed, WAD bundled as `doom.wad`)
- Bundle ID: `org.example.doom`
- Landscape only, fullscreen, game controller supported

## Prerequisites

- macOS with Xcode (full install, not just Command Line Tools — the iOS Simulator SDK is required)
- CMake (`brew install cmake`)
- An iOS Simulator device (e.g. iPhone) available in Xcode
- `DOOM.WAD` placed at the repository root (commercial asset — never commit it)

## Building

### 1. Fetch the SDL2 source (git submodule)

SDL is vendored as a submodule at `ios/deps/SDL`, pinned to tag `release-2.32.8`:

```bash
git submodule update --init ios/deps/SDL
```

### 2. Build SDL2 static libraries (one-time)

```bash
cd ios/deps
cmake -S SDL -B build-sim -G Xcode -DCMAKE_SYSTEM_NAME=iOS \
      -DCMAKE_OSX_SYSROOT=iphonesimulator -DCMAKE_OSX_ARCHITECTURES=arm64 \
      -DCMAKE_OSX_DEPLOYMENT_TARGET=13.0 -DSDL_STATIC=ON -DSDL_SHARED=OFF -DSDL_TEST=OFF
cmake --build build-sim --config Release --target SDL2-static -- -sdk iphonesimulator
cmake --build build-sim --config Release --target SDL2main   -- -sdk iphonesimulator
cd ../..
```

This produces:

```
ios/deps/build-sim/Release-iphonesimulator/libSDL2.a
ios/deps/build-sim/Release-iphonesimulator/libSDL2main.a
ios/deps/build-sim/include*/          # generated SDL headers
```

### 3. Build the app

```bash
make -f Makefile.ios
```

Output: `ios/build/Doom.app` (binary + `Info.plist` + `doom.wad`, ad-hoc signed).

### Build options

```bash
# Clean build artifacts
make -f Makefile.ios clean

# Build with AddressSanitizer
make -f Makefile.ios ASAN=1
```

## Running on the Simulator

```bash
# Boot a simulator (once), e.g.:
xcrun simctl boot "iPhone 16"
open -a Simulator

# Build, install, and launch in one step
make -f Makefile.ios install
```

Or manually:

```bash
xcrun simctl install booted ios/build/Doom.app
xcrun simctl launch booted org.example.doom
```

## Project Structure

```
DOOM/
├── Makefile.ios              # ✨ iOS build configuration
├── ios/
│   ├── i_main_ios.c          # ✨ iOS entry point (SDL_main)
│   ├── Info.plist            # ✨ App bundle metadata
│   ├── build/                # Build output (gitignored)
│   │   └── Doom.app
│   └── deps/
│       ├── SDL/              # SDL2 source (git submodule, release-2.32.8)
│       └── build-sim/        # SDL2 static libs for simulator (gitignored)
└── linuxdoom-1.10/           # Main source code (shared with macOS port)
    ├── i_video_sdl.c         # SDL2 video backend
    └── i_sound_sdl.c         # SDL2 audio backend
```

## Implementation Notes

- Compiled with `-DNORMALUNIX -DMACOS -DIOS`; `linuxdoom-1.10` sources are shared with the macOS port.
- `ios/i_main_ios.c` provides the `SDL_main` entry point required on iOS.
- The WAD is loaded from inside the app bundle (`Doom.app/doom.wad`), copied there at build time from `DOOM.WAD` at the repository root.
- Statically linked against SDL2; the link pulls in the iOS system frameworks SDL2 requires (UIKit, Metal, GameController, CoreAudio, etc.).
- Landscape-only, status bar hidden, fullscreen (`Info.plist`).

## Troubleshooting

### `xcrun: error: SDK "iphonesimulator" cannot be located`

Full Xcode is required. Point `xcode-select` at it:

```bash
sudo xcode-select -s /Applications/Xcode.app
```

### Missing `libSDL2.a`

Re-run step 2 (SDL2 static library build). Verify with:

```bash
ls ios/deps/build-sim/Release-iphonesimulator/
```

### `Unable to install` / launch failures

- Make sure a simulator is booted: `xcrun simctl list devices | grep Booted`
- Re-sign if needed: `codesign --force --sign - ios/build/Doom.app`
- Check simulator logs: `xcrun simctl spawn booted log stream --predicate 'process == "doom"'`

## Known Limitations

- Simulator only — building for a physical device requires the `iphoneos` SDK, a real signing identity, and touch controls.
- Touch input is not implemented; use a keyboard or a game controller paired with the simulator/host.
- Music plays via OPL2 FM synthesis (Sound Blaster style), mixed into the SFX stream at 11025 Hz — iOS SDL allows only one open audio device. The General MIDI backend (`-gmmusic`) is macOS-only.

## References

- [MACOS_PORT.md](MACOS_PORT.md) — macOS port guide (shared backend details)
- [SDL2 Documentation](https://wiki.libsdl.org/FrontPage)
- [Apple simctl reference](https://developer.apple.com/documentation/xcode/running-your-app-in-simulator-or-on-a-device)
