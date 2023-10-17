#ifndef __MODULERESOURCEMANAGER_H__
#define __MODULERESOURCEMANAGER_H__

#include "Module.h"

#include "Timer.h"

#include "MathGeoLib\MathGeoLib.h"

#include <map>

typedef long unsigned int ID;

class Resource;

class ModuleResourceManager : public Module
{

public:

	ModuleResourceManager(Application* app, bool start_enabled = true);
	~ModuleResourceManager();

	bool Awake(JSONParser &config);
	bool Start();

	UPDATE_STATUS PreUpdate(float dt);
	UPDATE_STATUS PostUpdate(float dt);

	bool CleanUp();

	Resource* CreateNewResource(RESOURCE_TYPE type, ID id, int timestamp);
	ID ImportFile(std::string &new_asset_file, ID force_id = 0);
	ID DeleteImportedFile(std::string &asset_to_delete);
	ID GenerateID();

	Resource* Get(ID uid);
	ID Find(const std::string &asset_to_find) const;

	bool LoadFile(const char *file_to_load);

private:

	Timer check_timer;
	float check_interval;

	math::LCG id_generator;

	std::map<ID, Resource*> resources;
	std::vector<Resource*> resources_to_delete;

	void DeleteEntry(ID id);
	bool IsUpdated(ID id) const;

	void FreeInactiveResources();
	
	RESOURCE_TYPE GetTypeOfFile(const std::string &file) const;
	
	// JSON related...
	ID FindFileIdJSON(const JSONParser &json, const char *scene_name, const char *file) const;
	void CreateJSONResourceInfo();

	bool Save(JSONParser &module);
	bool Load(JSONParser &module);
};

#endif // !__MODULERESOURCEMANAGER_H__
