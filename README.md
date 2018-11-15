# [Tacocat-engine](https://github.com/Hugo-Bo-Diaz/Tacocat-engine)
3D game engine for the game engines subject

Made by
* [Hugo Bó Diaz](https://github.com/Hugo-Bo-Diaz)
* [Daniel Garcia Ortega](https://github.com/viriato22)

## Controls

### Camera

* Use the WASD keys to move around
* Press the right mouse button to rotate
* Press the Left Alt key along with the left mouse button to orbit around the origin
* Press the shift key to move faster

### Scene

* Drop the files directly to the window to load them
* Files accepted
  * 3d models --> .fbx
  * Textures --> .png
* With the primitives window you can display one primitive primitive at a time changing the parameters and clicking the button of the primitive you want

### UI

* Thanks to ImGui we are able to give ample information on all the things happening on the screen:
  * Model data: number of vertices, number of indices, transformations, number of meshes...
  * Texture data: width, height, buffer id...
  * Application: memory usage, fps, vsync... 
  * Window: width, height...

### Contribution

* Hugo
  * All resources are game objects
  * Transformations
  * Frustum culling
  * Mouse picking
  * FOV
  * Event system
  
* Dani
  * Quadtree
  * Time management
  