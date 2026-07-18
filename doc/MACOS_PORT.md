# DOOM macOS Port Guide

## Overview

This is a port of the original DOOM engine to macOS using SDL2 (Simple DirectMedia Layer). The port uses SDL2 for cross-platform video and audio, making it compatible with macOS (both Intel and Apple Silicon), as well as other Unix-like systems.

## Features

- ✅ **Native macOS Support** - Runs on Apple Silicon (M1/M2/M3) and Intel Macs
- ✅ **SDL2 Backend** - Cross-platform graphics and audio
- ✅ **Modern Display** - Supports window scaling and full-screen modes
- ✅ **High-Quality Audio** - Stereo sound mixing via SDL2
- ✅ **Keyboard and Mouse Input** - Full input support with relative mouse mode
- ✅ **Network Play** - Supports multiplayer via TCP/IP

## Prerequisites

### System Requirements

- macOS 10.9 or later
- Xcode Command Line Tools
- SDL 2.0 library

### Installation

#### 1. Install Xcode Command Line Tools

```bash
xcode-select --install
```

#### 2. Install SDL2 via Homebrew

```bash
brew install sdl2
pkg-config --modversion sdl2  # Verify installation
```

If SDL2 is installed in a non-standard location, update the paths in `Makefile.macos`.

#### 3. Obtain DOOM Game Data

You need the original DOOM data files (WAD files):

- **Free Options:**
  - GOG.com: Download DOOM (DRM-free)
  - Steam: Purchase DOOM or DOOM II
  
- **File Structure:**
  ```
  DOOM.WAD      (for DOOM)
  DOOM2.WAD     (for DOOM II)
  ```

## Building

### Quick Start

```bash
# Clone/fork the repository
cd DOOM
git checkout feature/macos

# Build
make -f Makefile.macos

# Run
./macos/doom -iwad /path/to/DOOM.WAD
```

### Build Options

```bash
# View build information
make -f Makefile.macos info

# Clean build artifacts
make -f Makefile.macos clean

# Build with verbose output
make -f Makefile.macos VERBOSE=1
```

## Running DOOM

### Basic Usage

```bash
./macos/doom -iwad /path/to/DOOM.WAD
```

### Command Line Options

```bash
# Window scaling
./macos/doom -iwad DOOM.WAD -2x   # 640x400 window
./macos/doom -iwad DOOM.WAD -3x   # 960x600 window

# Fullscreen mode
./macos/doom -iwad DOOM.WAD -fullscreen

# Sound volume (0-15)
./macos/doom -iwad DOOM.WAD -sfxvolume 15
./macos/doom -iwad DOOM.WAD -musicvolume 15

# Network play (server)
./macos/doom -iwad DOOM.WAD -server

# Network play (client)
./macos/doom -iwad DOOM.WAD -connect hostname
```

## Controls

### Default Controls

| Action | Key |
|--------|-----|
| Move Forward | W / Up Arrow |
| Move Backward | S / Down Arrow |
| Strafe Left | A / Q |
| Strafe Right | D / E |
| Look Up | Page Up |
| Look Down | Page Down |
| Fire | Ctrl / Mouse Button 1 |
| Open/Use | Space |
| Run | Shift |
| Map | M |
| Inventory | I |
| Pause | P |
| Quit | Esc |

### Mouse Controls

- **Look Around:** Move mouse (relative mode)
- **Fire:** Left click
- **Alternative Fire:** Right click

## Project Structure

```
DOOM/
├── linuxdoom-1.10/           # Main source code
│   ├── i_video_sdl.c        # ✨ SDL2 video output (NEW)
│   ├── i_sound_sdl.c        # ✨ SDL2 audio output (NEW)
│   ├── i_system.c           # System interface
│   ├── d_main.c             # Main loop
│   ├── p_*.c                # Physics & gameplay
│   ├── r_*.c                # Rendering
│   └── ...
├── Makefile.macos           # ✨ macOS build configuration (NEW)
├── doc/
│   └── MACOS_PORT.md        # ✨ This file (NEW)
├── .git/
├── LICENSE.TXT
└── README.TXT
```

## Implementation Details

### Video System (i_video_sdl.c)

The SDL2 video implementation provides:

- **Display Management:** SDL_Window and SDL_Renderer
- **Texture Streaming:** Real-time framebuffer updates
- **Input Handling:** Keyboard and mouse events via SDL2
- **Scaling:** Hardware-accelerated window scaling
- **Fullscreen:** Desktop fullscreen mode support

Key Functions:
- `I_InitGraphics()` - Initialize display
- `I_UpdateGraphics()` - Render frame
- `I_GetEvent()` - Process input
- `I_ShutdownGraphics()` - Cleanup

### Audio System (i_sound_sdl.c)

The SDL2 audio implementation provides:

- **Audio Mixing:** 8-channel sound mixing
- **Volume Control:** Per-channel and master volume
- **Stereo Output:** Full stereo sound separation
- **Format Conversion:** 8-bit WAD data to 16-bit audio
- **Buffer Management:** Queue-based audio playback

Key Functions:
- `I_InitSound()` - Initialize audio device
- `I_StartSound()` - Start sound effect
- `I_UpdateSound()` - Mix and queue audio
- `I_SetSfxVolume()` - Control volume

### Build System (Makefile.macos)

Features:
- Automatic SDL2 flag detection via `pkg-config`
- Clang/GCC compatibility
- Organized object file output directory
- Clean build support

## Troubleshooting

### Build Issues

#### SDL2 Not Found

```bash
# Check SDL2 installation
pkg-config --modversion sdl2

# If not found, reinstall
brew uninstall sdl2
brew install sdl2
```

#### Compiler Errors

Ensure you have the latest Xcode Command Line Tools:

```bash
xcode-select --install
xcode-select --reset
```

### Runtime Issues

#### No Sound

1. Check volume levels:
   ```bash
   ./macos/doom -iwad DOOM.WAD -sfxvolume 15
   ```

2. Verify audio device:
   ```bash
   # Check system audio settings
   osascript -e "output volume of (get volume settings)"
   ```

#### Performance Issues

- Use `-2x` or `-3x` scaling to reduce pixel rendering
- Close other applications
- Try fullscreen mode with `-fullscreen`

## Contributing

### Git Workflow

```bash
# Update from upstream
git fetch upstream
git rebase upstream/master

# Create feature branch
git checkout -b feature/your-feature

# Commit and push
git add .
git commit -m "feat: Your feature description"
git push origin feature/your-feature

# Create Pull Request on GitHub
```

### Code Style

- Follow the existing code conventions
- Use 4-space indentation (tabs replaced with spaces)
- Include descriptive comments
- Test on both Intel and Apple Silicon Macs if possible

## Future Improvements

- [ ] Network multiplayer
- [ ] .app bundle packaging
- [ ] High-resolution texture support

## License

This source code is distributed under the **GNU General Public License v2**.

The original DOOM source code is copyright (C) 1993-1996 by id Software, Inc.

This macOS port modifications are subject to the same license.

## References

### Original Documentation
- [README.TXT](../README.TXT) - Original id Software README
- [LICENSE.TXT](../LICENSE.TXT) - GPL v2 License

### Additional Resources
- [SDL2 Documentation](https://wiki.libsdl.org/FrontPage)
- [DOOM Engine](https://en.wikipedia.org/wiki/Doom_engine)
- [DOOM Wiki](https://doom.fandom.com/)

### Similar Projects
- [Chocolate DOOM](https://www.chocolate-doom.org/) - Accurate DOOM port
- [PrBoom+](http://prboom-plus.sourceforge.net/) - Extended DOOM port
- [GZDoom](https://zdoom.org/) - Advanced DOOM engine

## Support

For issues and questions:

1. Check this documentation first
2. Review existing GitHub issues
3. Create a new issue with:
   - macOS version
   - Xcode version
   - SDL2 version
   - Error messages/logs
   - Steps to reproduce

## Acknowledgments

- id Software for releasing the DOOM source code
- SDL2 team for the excellent cross-platform library
- The DOOM community for documentation and tools
- All contributors to this macOS port
