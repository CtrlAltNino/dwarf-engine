![dwarf-engine-banner](https://github.com/CtrlAltNino/dwarf-engine/blob/main/assets/images/Dwarf%20Engine%20Banner.png)

# Dwarf Engine

**An open-source and cross-platform scene editor built in C++**<br>
Version `0.1` — _First public release_

Dwarf Engine is a native, real-time 3D editor designed to help you visualize and edit scenes with minimal overhead.<br>
Easily use your own native shaders, or customize the source code to create your own graphics features.<br>
It supports hot-reloaded assets, custom materials, a forward renderer, and a growing set of editor tools.

This project is developed and maintained by [@CtrlAltNino](https://github.com/CtrlAltNino), and documented in depth on [the development blog](https://blog.ctrlaltnino.com).

---

## Features

### Editor

- Project launcher (create & manage projects)
- Scene creation, saving, loading
- Asset hot-reloading (textures, models, shaders)
- Material inspector & live preview
- Hierarchical scene management
- Performance stats (frametime, VRAM usage, draw calls)
- Free-floating camera & adjustable rendering settings
- Gizmos for entity transforms
- Tone mapping options (Reinhard, AGX, ACES)

### Rendering

- Custom forward renderer (OpenGL)
- Batching by material and shader
- Dynamic draw call generation
- HDR + tone mapping
- Client-side OpenGL state caching

### Assets

- Supported textures: `bmp`, `jpeg`, `png`, `tga`, `tiff`
- Supported models: `obj`, `fbx`, `gltf`
- Shaders organized by file extension
- Material serialization

---

## Build Instructions

### Dependencies

This project uses [`CMake`](https://cmake.org/) and [`vcpkg`](https://github.com/microsoft/vcpkg) for dependency management.

Tested on:

- Windows 11
- Ubuntu 24.10 (x11 and Wayland)

## Screenshots

The Crytek Sponza template project
![screenshot-sponza](https://github.com/CtrlAltNino/dwarf-engine/blob/main/assets/screenshots/screenshot%20crytek%20sponza.png)

Olaf saying hi!
![screenshot-olaf](https://github.com/CtrlAltNino/dwarf-engine/blob/main/assets/screenshots/screenshot%20olaf.png)

## Roadmap

For a detailed outlook on upcoming features and development goals, check out [the 0.1 Release Blog Post](https://blog.ctrlaltnino.com/p/introducing-dwarf-engine-0.1-a-cross-platform-editor-for-graphics-nerds/).

Some upcoming features for 0.2:

- Drag and drop into asset browser
- Scene-level settings
- Transparency sorting & IBL
- Skybox support

## Contributing

This project is in early development, and contributions are very welcome!<br>
Feel free to:

- Open issues
- Suggest features
- Submit PRs
- Share ideas in the [Discord server](https://discord.gg/EcpPTAbJcz)

## Credits

A full list of libraries, models, and SDKs used can be found in [`CREDITS.md`](https://github.com/CtrlAltNino/dwarf-engine/main/CREDITS.md).

## Support

If you like this project, you can support development:

- [Ko-fi](https://ko-fi.com/CtrlAltNino)
- [Patreon](https://patreon.com/CtrlAltNino)

Or simply star the repo and share it with others!
