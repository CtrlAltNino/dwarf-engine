# Simple 3D Engine

### Description

Simple 3D Engine, or in short S3DE, is a cross-platform engine written in C++ that is able to utilize different graphics API's to render 3D Objects and Scenes. It's main purpose is to be a simple, transparent and robust foundation to experiment with graphics features, rendering pipelines and shaders. Through its modular nature, you are able to create your own rendering pipelines or post processing effects in the form of plugins, alter the source code itself to implement your own graphics features, or just learn and experiment with shaders in different graphics API's.

### Project Goals

Game engine and graphics development is a very deep topic containing highly specialized tools and work flows. Especially when it comes to real time rendering, there are a lot of quirks and hacks to be used and learned. This creates a high entry barrier as a lot of things happening are considered to be _magic_. To battle this, I created this project to be a playground that makes the aforementioned _magical_ techniques directly accessible in a transparent way. S3DE has the following goals:

**<u>1. To be simple</u>**

Using popular game engines like Unreal Engine, Unity or even Godot is very convenient and essential. Through a lot of layers of an engine though, the fundamental processes of getting a 3D model to show on your screen, or adding the all so beloved motion blur effect, get lost.

These processes are very complicated at times, so its natural to abstract workflows in favor of efficiency and accessibility. But for people who crave a better understanding of these, S3DE provides all the mainstream graphical features of a realtime engine (e.g. shadow casting, anti-aliasing, post processing effects) in a simple and basic form.

**<u>2. To be flexible</u>**

All the functionality that S3DE harbors, is completely modular by nature. So you are free to bend (and also break) the rendering pipeline to your will. An extract of the things you are able to do:

- Enable/disable entire features
- Experiment with all their settings/variables
- Look at the source code and even alter it if you want to do so
- Rearrange steps of the rendering pipeline

To evaluate those experiments, S3DE contains a performance logger, showing you exactly how well the performance of the current scene is. It breaks down which steps take place and how long they take (measured in ms). These logs can be viewed at runtime or recorded and exported to a .csv file for later assessment.

**<u>3. To be extendable</u>**

When creativity hits, it hits hard. So if you want to experiment on any front, S3DE got you covered. Through plugins you can add your own visions to S3DE using the already present functionalities. If that is not enough, the documentation together with the project being open source encourages you to add your own code to S3DE. A couple examples:

- Add your own shader to render objects differently
- Write our own custom post processing effect
- Implement your own rendering Pipeline
- Create your own volumetric cloud system
- Develop your own module with an editor window to create noise textures 

**<u>4. To never be in your way</u>**

In S3DE, you will never be forced to use an abstract layer to access functionality. You have full access to all the things that are happening in the rendering process, every texture, every variable, every object, at all times. This quality opens up a lot of things for you:

- Create your own data structure to manage scene objects, for example as an octree based on your own conditions
- Implement your own texture loading and unloading algorithms
- Write your own class for Materials

**<u>What S3DE is NOT</u>**

S3DE is not providing tutorials for how to implement certain features or writing shaders, merely details on how the shipped features have been implemented (through documentation and the code itself). If you are for example just starting out with shader programming, you can use S3DE as a way to quickly try out and visualize these in any graphics API you want. Drop in your shaders, models and textures (or use existing ones) and directly see your creation. Changes in your shader will then be applied in real time.

### Planned Features

- <u>General Engine Features:</u>
  - Saving and loading of S3DE projects
  - GUI that reflects the state of the scene
  - Improved management of models
  - Runtime import of ressources (models, textures, shaders)
  - Animation System
  - Multi Threading
  - Framerate limiting
  - VSync

- <u>Basic Rendering Features:</u>
  - Full implementation of Forward, Deffered, and Forward+ Rendering
  - Transparency Sorting
  - Modularity that allows to quickly integrate new Post Processing Effects
  - Fog System
  - Motion Vectors
  - Depth Buffer

- <u>Advanced Graphical Features:</u>
  - Particle Effect System
  - Anti Aliasing Implementations (MSAA, FXAA, TAA)
  - Shadow Mapping
  - Light Mapping
  - Reflection Probes
  - Depth of Field
  - Motion Blur (Camera based and Object based)
  - Chromatic Aberration
  - Screen Space Reflections
  - Subsurface Scattering
  - More Advanced Lighting Models
