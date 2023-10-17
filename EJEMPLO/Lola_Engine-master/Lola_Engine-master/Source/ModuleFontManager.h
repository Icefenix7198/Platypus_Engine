#ifndef __MODULEFONTMANAGER_H__
#define __MODULEFONTMANAGER_H__

#include "Module.h"

//#include "FreeType\freetype2\freetype\ftglyph.h"
//#include "FreeType\freetype2\freetype\ftoutln.h"
//#include "FreeType\freetype2\freetype\fttrigon.h"

// FreeType Headers
//#include "FreeType2\include\ft2build.h"
//#include "FreeType2\include\freetype\freetype.h"
//#include "FreeType2\include\freetype\ftglyph.h"
//
//#pragma comment (lib, "Source/FreeType2/libx86/freetype.lib")

class ResourceTexture;

class ModuleFontManager : public Module
{
public:

	ModuleFontManager(Application* app, bool start_enabled = true);
	~ModuleFontManager();

	bool Awake(JSONParser &config);

	bool Start();

	UPDATE_STATUS PreUpdate(float dt);
	UPDATE_STATUS PostUpdate(float dt);

	bool CleanUp();

	void CreateTextureFromText(ResourceTexture *res);

private:
	
	//FT_Face face;			// The Object In Which FreeType Holds Information On A Given Font Is Called A "face".
	//FT_Library library;

};

#endif // !__MODULEFONTMANAGER_H__
