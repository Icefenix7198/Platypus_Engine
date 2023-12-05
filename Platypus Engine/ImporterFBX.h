#ifndef __IMPORTER_FBX_H__
#define __IMPORTER_FBX_H__

#include "Importer.h"

class ImporterFBX : public Importer
{
public:

	//Transform/copy values from asset file to our structure of info
	void Import(const char* filePath);
	//Save from our struct of info to library
	__int64 Save(); //Todo: Preguntar al profe si eltipo de la funcion esta bien o que 
	//Load  info from library to our struct	
	void Load(char* buffer);

};

#endif __IMPORTER_FBX_H__
