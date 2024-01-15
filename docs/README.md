

# Platypus Engine
The agent of the engines

## About the engine

Platypus Engine is a videogame engine made by Eric Segovia and Andreu Miró, students from CITM university, Terrasa, for the subject "Motors de Videojocs" in the 3th garde in 2023/24.

Also this engine, contains an implementation of a particle system, making posible to implement particles to your games.

##  About us
**Éric Segovia Baena**  (eric.segovia@estudiantat.upc.edu)
(imagen y cosas que has hecho Eric)

**Andreu Miró Sabaté**  (andreu.miro@estudiantat.upc.edu)

<img src="/andreu_imagen.jpg" />

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

## Link

> Link to github: [Platypus Engine](https://github.com/Icefenix7198/Platypus_Engine)

## License
