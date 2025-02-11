![dwarf-engine-banner](https://github.com/CtrlAltNino/dwarf-engine/blob/main/doc/images/Dwarf%20Engine%20Banner.png)

# Dwarf Engine

## Description

Dwarf Engine, or just Dwarf, is a cross-platform engine written in C++ that is able to utilize different graphics API's to render 3D Objects and Scenes. It's main purpose is to be a simple, transparent and robust foundation to experiment with graphics features, rendering pipelines and shaders. Through its modular nature, you are able to create your own rendering pipelines or post processing effects in the form of plugins, alter the source code itself to implement your own graphics features, or just learn and experiment with shaders in different graphics API's.

![dwarf-engine-banner](https://github.com/CtrlAltNino/dwarf-engine/blob/main/doc/images/Dwarf%20Engine%20Screenshot%203.png)

## Project Goals

Game engine and graphics development is a very deep topic containing highly specialized tools and work flows. Especially when it comes to real time rendering, there are a lot of quirks and hacks to be used and learned. This creates a high entry barrier as a lot of things happening are considered to be _magic_. To battle this, I created this project to be a playground that makes the aforementioned _magical_ techniques directly accessible in a transparent way. The development of Dwarf holds the following goals:

### 1. To be simple

Using popular game engines like Unreal Engine, Unity or even Godot is very convenient and essential. Through a lot of layers of an engine though, the fundamental processes of getting a 3D model to show on your screen, or adding the all so beloved motion blur effect, get lost.

These processes are very complicated at times, so its natural to abstract workflows in favor of efficiency and accessibility. But for people who crave a better understanding of these, Dwarf provides all the mainstream graphical features of a realtime engine (e.g. shadow casting, anti-aliasing, post processing effects) in a simple and basic form.

---

### 2. To be flexible

All the functionality that Dwarf harbors, is completely modular by nature. So you are free to bend (and also break) the rendering pipeline to your will. An extract of the things you are able to do:

- Enable/disable entire features
- Experiment with all their settings/variables
- Look at the source code and even alter it if you want to do so
- Rearrange steps of the rendering pipeline

To evaluate those experiments, Dwarf contains a performance logger, showing you exactly how well the performance of the current scene is. It breaks down which steps take place and how long they take (measured in ms). These logs can be viewed at runtime or recorded and exported to a .csv file for later assessment.

---

### 3. To be extendable

When creativity hits, it hits hard. So if you want to experiment on any front, Dwarf got you covered. Through plugins you can add your own visions to Dwarf using the already present functionalities. If that is not enough, the documentation together with the project being open source encourages you to add your own code to Dwarf. A couple examples:

- Add your own shader to render objects differently
- Write our own custom post processing effect
- Implement your own rendering Pipeline
- Create your own volumetric cloud system
- Develop your own module with an editor window to create noise textures

---

### 4. To never be in your way

In Dwarf, you will never be forced to use an abstract layer to access functionality. You have full access to all the things that are happening in the rendering process, every texture, every variable, every object, at all times. This quality opens up a lot of things for you:

- Create your own data structure to manage scene objects, for example as an octree based on your own conditions
- Implement your own texture loading and unloading algorithms
- Write your own class for Materials

---

### What Dwarf is NOT

Dwarf is not providing tutorials on how to implement certain features or how to write shaders, merely details on how and why the shipped features have been implemented (through documentation and the source code itself). If you are, for example, just starting out with shader programming, you can use Dwarf as a way to quickly try out and visualize these in any graphics API you want. Drop in your shaders, models and textures (or use existing ones) and directly see your creation. Changes in your shader will then be applied in real time.

## Features

- **Workflow:**
  - Dwarf relies on a project based workflow, which you can manage with the project launcher
  - Per project you manage various assets (textures, models, shaders, etc...), plugins and scenes
  - The possibility to use scenes enables the reuse of assets for different purposes as you can set a lot of settings on a per scene basis
- **General Engine Features:**
  - Saving and loading of Dwarf projects
  - GUI that reflects the state of the scene
  - Runtime import of resources (models, textures, shaders)
  - Support for various model files
  - Animation support
  - Multi threading
  - Framerate limiting
  - Window settings (windowed, fullscreen, borderless)
  - VSync
- **Basic Rendering Features:**
  - Full implementation of forward, deffered, and forward+ rendering
  - Transparency sorting
  - Modularity that allows to quickly integrate new post processing effects
  - Basic depth based fog system
  - Motion vectors
  - Depth buffer
- **Advanced Graphical Features:**
  - Particle effect system
  - Anti aliasing implementations (MSAA, FXAA, TAA)
  - Shadow mapping
  - Light mapping
  - Reflection probes
  - Depth of field
  - Motion blur (Camera based and object based)
  - Chromatic aberration
  - Screen space reflections
  - Subsurface scattering
  - More advanced lighting models
