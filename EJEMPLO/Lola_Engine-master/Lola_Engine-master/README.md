# :dancer: LOLA Engine :dancer:

3D Engine created and developed by Carlos Randino (**crandino** on GitHub) in VideoGame Engines Subject on CITM (Terrassa).
This engine is hosted on this [GitHub repository](https://github.com/crandino/Lola_Engine).
LOLA Engine is under a [MIT License](https://github.com/crandino/Lola_Engine/blob/master/LICENSE.md).

Useful tips:

- Camera: Camera can be moved with A/D (left/right), W/S (up/down) and R/F (front/back). Holding right mouse button, the camera will orbit. The movement is a little bit strange and it has to be fixed.
- Windows: The engine has different windows that may be activated: 
  - Show Hierarchy: Press number 1.
  - Show Configuration: Press number 2.
  - Show Inspector: Selecting a GameObject on Hierarchy menu or clicking on mesh on the editor will launch the Inspector window that shows all the information related with this gameobject.
- Loading files: Now, in order to load any file to the engine, you must follow these steps:
  - First, any interested file must be dropped on its corresponding folder: FBX files on Assets/Models/ and textures on Assets/Textures. The engine will import these files to their own file-format.
  - Now, you can drag and drop a FBX format-like file previously placed on Assets/Models folder to instantly load them.
- Deleting gameobjects: Selecting a GameObject and pressing _Supr_ will delete it, including all its children.
- Saving and loading: The game editor allows to save and load the scene. Press F8 to save and F9 to load.
- Play mode: LOLA Engine presents a Play Mode, but it can not be observed because there aren't components that run their logic (animation, sound, physics, etc...) There are three available buttons:
  - PLAY: When it is red, engine enters on Play Mode. The buttons change to green to indicate that. Pressing PLAY again on Play mode, the engine stops play mode and return to the previous configuration before hitting PLAY when it was red.
  - PAUSE: The game stops, so it doesn't update the logic.
  - NEXT FRAME: The game goes forward only one frame.

