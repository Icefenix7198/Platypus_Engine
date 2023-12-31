#ifndef __IMPORTER_H__
#define __IMPORTER_H__

#include "Resource.h"

enum ImporterType //Each type of archive will inherit of this importer
{
	NONE,
	IMPORTER_FBX,
	IMPORTER_MESH,
	IMPORTER_MATERIAL,
	NUM_IMPORTERS //For iteration reasons, not an importer
};

class Importer
{
public:

	//Transform/copy values from asset file to our structure of info
	virtual void Import(const char* pathFile);
	//Save from our struct of info to library
	virtual __int64 Save(); //Todo: Preguntar al profe si eltipo de la funcion esta bien o que 
	
	//Load  info from library to our struct	
	virtual void Load(char* buffer, Resource* resource);
};

#endif __IMPORTER_H__
