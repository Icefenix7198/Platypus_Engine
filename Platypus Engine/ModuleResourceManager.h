#ifndef MODULE_RESOURCES
#define MODULE_RESOURCES

#include "Module.h"
#include "Globals.h"

#include "Resource.h"

#include "GameObject.h"

#include <vector>
#include <string>
#include <map>

class ModuleResources : public Module
{
public:
	ModuleResources(Application* app, bool start_enabled);
	~ModuleResources();

	bool Init();
	update_status PreUpdate(float dt);
	bool CleanUp();

	uint32_t Find(const char* file_in_assets) const;
	uint32_t ImportFile(const char* new_file_in_assets);
	uint32_t GenerateNewUID();
	const Resource* RequestResource(uint32_t uid) const;
	Resource* RequestResource(uint32_t uid);
	void ReleaseResource(uint32_t uid);

	
private:
	Resource* CreateNewResource(const char* assetsFile, ResourceType type);
private:
	std::map<uint32_t, Resource*> resources;
};

#endif //MODULE_RESOURCES