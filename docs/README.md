

# Platypus Engine
The agent of the engines

## About the engine

Platypus Engine is a videogame engine made by Eric Segovia and Andreu Miró, students from CITM university, Terrasa, for the subject "Motors de Videojocs" in the 3th garde in 2023/24.

Also this engine, contains an implementation of a particle system, making posible to implement particles to your games.

##  About us
**Éric Segovia Baena**  (eric.segovia@estudiantat.upc.edu)

![FotoEric](/docs/assets/imagen_Eric.jpg)

- Resource manager
- Game objects
- Serialitzation of the scene
- Component transform
- Component mesh
- Comonent material
- Partycle system
- Heriarchy of game objects

**Andreu Miró Sabaté**  (andreu.miro@estudiantat.upc.edu)

![FotoAndreu](/docs/assets/imagen_andreu.jpg)

- Camera System
- Frustum Culling
- Web page
- Component material
- Component camera

## Core Subsystems

### Game Objects
- You can create game objects
- Every game object can have their own components (transform, mesh, material, camera and partycle)
- A modificable heriarchy for game objects
- Empty game objects exists
- Able to load your own objects droping them to the engine

### Resorce Manager
- Creates a folder that saves a copy for all the items uploaded to de project (if one item has already been uploaded it doesen't duplicate!!)
- Creates a .meta for every item uploaded (doesen't duplicete them too!!)
- Creates an unique id for every item

## Particle System
This system let you crate a component for a game object that enable you to create multiples particles emmiters.

Each emitter can have the next options:
- Base emitter: You can change the position and the life time of the particles.
- Spawn emitter: Decide the number of particles emited in a certain time, or how many particles are emitted in 1 second
- Position emitter: Decide the direction of the particles, it lets you to set it randomly between a certain range.
- Scale emitter: You can change the size of the particles. Also you can change the size on a progessive way and decide when this resize starts and ends.
- Color emitter: You can change the color of the particle and the opacity of it. Also enable you to make a progresive change of color and opacity.
  
## Trailer

[![A video to show the engine](/docs/assets/video.jpg)](https://www.youtube.com/watch?v=ZT09cbbbHG4)

## Link

> Link to github: [Platypus Engine](https://github.com/Icefenix7198/Platypus_Engine)  /
> Link of the release: [Platypus Engine Release](https://github.com/Icefenix7198/Platypus_Engine/releases/tag/v1.1)

## License

MIT License

Copyright (c) 2024 Éric Segovia Baena & Andreu Miró Sabaté

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
