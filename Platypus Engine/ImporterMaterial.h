#ifndef __IMPORTER_MATERIAL_H__
#define __IMPORTER_MATERIAL_H__

#include "Importer.h"
// include resource material

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/mesh.h"
#include "Glew/include/glew.h"

#include "MathGeoLib/include/MathGeoLib.h"

#include "GameObject.h"
#include "ResourceMaterial.h"

class ImporterMaterial : public Importer
{
public:

	//Transform/copy values from asset file to our structure of info
	void Import(const char* filePath);
	//Save from our struct of info to library
	uint64_t Save(ResourceMaterial* rMaterial, char** buffer); //Todo: Preguntar al profe si eltipo de la funcion esta bien o que 
	//Load  info from library to our struct	
	void Load(/*ResourceMesh* rMesh,*/ char* buffer);
	
	//JSON Things
	void CreateMetaTexture(const char* filePath, std::vector<Resource*> meshes);
};

#endif __IMPORTER_MATERIAL_H__
