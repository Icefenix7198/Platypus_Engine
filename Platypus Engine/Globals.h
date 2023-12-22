#pragma once

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 )

#include <windows.h>
#include <stdio.h>
#include<string>


#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

//File System things (ft. ThorEngine)
#define LIBRARY_PATH "Library/"
#define FOLDERS_PATH "Library/Folders/"
#define MESHES_PATH "Library/Meshes/"
#define MATERIALS_PATH "Library/Materials/"
#define TEXTURES_PATH "Library/Textures/"
#define MODELS_PATH "Library/Models/"
#define ANIMATIONS_PATH "Library/Animations/"
#define BONES_PATH "Library/Bones/"
#define PARTICLES_PATH "Library/ParticleSystems/"
#define SHADERS_PATH "Library/Shaders/"
#define MAPS_PATH "Library/Maps/"
//Assets paths
#define ASSETS_MODELS "Assets/Models/"
#define ASSETS_TEXTURES "Assets/Textures/"
#define ASSETS_GAMEOBJECTS "Assets/Game_Objects/";
//CFF
#define CFF ".pry"

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define PI 3.14159265358979323846264338327950288
#define CHECKERS_HEIGHT 64
#define CHECKERS_WIDTH 64

typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

extern std::string consoleLog;
