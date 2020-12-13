# Camel Engine

**Camel Engine** is a 3D game engine developed by three students for the subject of Video game Engines in UPC, CITM: Pol Camacho, Marc Rosell and Alexandru Cercel. 

## Brief Project Description
The engine is programmed using **C/C++**, **OpenGL**, **ImGui**, **SDL**, **GLEW** and other libraries TBD, and it is currently work in progress.

## Repository
https://github.com/polcamacho/Camel-engine

## Team
* [**Pol Camacho**](https://github.com/polcamacho)
* [**Marc Rosell**](https://github.com/MarcRosellH)
* [**Alexandru Cercel**](https://github.com/AlexandruC5)

## Engine controls
* WSAD: Move camera forward/backward/left/right.
* Mouse wheel: Zoom in/out.
* Mouse Botton Center (drag mouse): Move left and right.
* Alt + Left click: Move around an object.
* F: Focus the camera in a selected object.
* SHIFT (Holded): Duplicate WSAD movement.

## Functionality
### Assignment 2

Now the engine has a camera component which is used for the engine scene view as well.
There is also a main camera, which is able to do culling with the other game objects inside the scene, and now mouse picking is possible.
Now the engine has time management to seperate the engine timer and the game timer.
Finally, the engine uses a custom file format and importers for meshes and materials.

Due to an issue with a MathGeoLib include, the development of the engine has been paused and no more work has been done since its detection, and other issues and errors haven't been solved.

### Assignment 1

Camel Engine supports 3D model and texture loading. Also, it is possible to drag and drop, with left mouse click hovered, models and textures.
This models will appear with the textures they were exported with and the user can change textures dragging them from the texture folder.

This engine has 3 differenciated parts: The hierarchy, where are displayed all game objects that are present at sceen, with root object that corresponds to 
scene, and all child objects that a game object can have. Second, the scene itself, that shows models and textures and 3D scene. And finally, the inspector, where
are shown the game object components: Transform (object position, scale and rotation), mesh and material (texture). Also the user is able to hide an object or only hide its texture.

And last but not least there is the main menu bar where appear few other functionalities:

 * The user can create 4 different primitives in determined positions (Cube, Sphere, Cylinder and Pyramid).
 * Open status, console, hierarchy and inspector windows.
 * About info of the project
 * Close the application

## License

MIT License

Copyright (c) [2019] [Pol Camacho Banal, Marc Rosell Hernandez and Alexandru Cercel]

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
