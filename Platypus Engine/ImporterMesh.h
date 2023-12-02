#ifndef __IMPORTER_MESH_H__
#define __IMPORTER_MESH_H__

#include "Importer.h"
#include "ResourceMesh.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/mesh.h"
#include "Glew/include/glew.h"

#include "MathGeoLib/include/MathGeoLib.h"

class ImporterMesh : public Importer
{
public:

	//Transform/copy values from asset file to our structure of info
	virtual void Import(aiScene* scene, ResourceMesh& reMesh);
	//Save from our struct of info to library
	uint64_t Save(ResourceMesh rMesh, char** buffer); //Todo: Preguntar al profe si eltipo de la funcion esta bien o que 
	//Load  info from library to our struct	
	virtual void Load(ResourceMesh &rMesh, char* buffer);

};

#endif __IMPORTER_MESH_H__
