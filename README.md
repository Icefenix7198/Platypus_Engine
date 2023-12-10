# Motores3D
A videogame motor for third grade of the career at CITM Terrasa, made by Andreu Miró and Eric Segovia.


# Controls

## Camera

- Alt + Left: Click Orbital camera
- Alt + Right: click Free camera
  - (In free camera) A: move Left
  - (In free camera) D: move Right
  - (In free camera) W: move Forward
  - (In free camera) S: move Backwards
  - (In free camera) Q: move Down
  - (In free camera) R: move Up
- Alt + Mouse wheel movement: Zoom
- Press Mouse wheel and move mouse: Free screen move
- F to look the game object (if you change some of the values of translate, it will look to that position, although the mesh won't move)

# Features

- You can create a cube, shpere and a cylinder.
- You can drag an drop fbx files from your device to the program.
- You can show the vertex and face normals of a game object.
- Every game object has three components: transform, mesh and material.
- The project starts with a house charged. 
- There are two fbx that you can charge to the engine (to see Mike the mouse you will have to zoom out!!!!).
- You can se the UV checkers of a gameobject, but they can't have a texture.
- Component camera with frustum culling, custom settings and raycast.
- In the window Inspector, you can modify the position, rotation and scale of an object.
- In the window Inspector, you can delete an object.
- In the Heriarchy window, you can reparent the game objects.
- You can import meshes and textures.
- All meshses use a bounding volume.

## Windows

- Heriarchy window were you can select a created or lodaded game object.
- Inspector window were you can acces to de components of the game object and crate some components.
- Configuration window were you can configurate the window, see the mouse information, the fps, and hardware information.
- Console window that prints libraries initialitzation and information of game objects loading.
- Assets window, where you can se the assets that you uploaded to your project.

## Resource Manager

- Creates a .meta for every model droped on the scene (doesn't duplicate).
- Creates a copy of the dragged and droped outerfiles.
- Creates a folder called Library. In this folder there are stored our custom format meshes (.pry).
- If a model is already loaded, doesen't import used stored meshes.
- If a model has cff and .meta, doesn't import the model, instead, loads the already saved meshes.
- The components use less memory as they reuse the same resorces loaded in the project.
