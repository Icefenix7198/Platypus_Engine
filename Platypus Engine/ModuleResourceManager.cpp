#include "Application.h"
#include "ModuleResourceManager.h"

#include "MathGeoLib/include/Algorithm/Random/LCG.h"
#include "Component.h"
#include "ComponentTransform.h" //Para updatear la transform cuando el reparent, no se si haya que hacerlo de otra forma y no tener este include

ModuleResources::ModuleResources(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleResources::~ModuleResources()
{
}

bool ModuleResources::Init()
{  
	return true;
}

update_status ModuleResources::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleResources::CleanUp()
{
	return true;
}

uint32_t ModuleResources::ImportFile(const char* assetsFile)
{
	//Resource* resource = CreateNewResource(assetsFile, type); //Save ID, assetsFile path, libraryFile path
	uint32_t ret = 0;
	////char* fileBuffer = Engine->fileSystem->Load(assetsFile); //<-- pseudocode, load from File System
	//switch (resource->Type) {
	//case Resource::texture: App->tex->Import(fileBuffer, resource); break;
	//case Resource::scene: App->scene->Import(fileBuffer, resource); break;
	//case Resource::mesh: App->meshes > Import(fileBuffer, resource); break;
	//}
	//SaveResource(resource);
	//ret = resource->GetID();
	//RELEASE_ARRAY(buffer);
	//UnloadResource(resource); //<-- unload the resource after importing, we should only use the ID
	
	return ret;
}

uint32_t ModuleResources::GenerateNewUID()
{
	LCG lcg;
	//lcg.Seed();

	return uint32_t();
}





