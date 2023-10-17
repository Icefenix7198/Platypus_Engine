#ifndef __MODULE_H__
#define __MODULE_H__

#include "Globals.h"

#include "JSONParser.h"

class Application;
class GameObject;

enum GUI_EVENTS;

class Module
{
private :

	bool enabled;
	
protected:
	
	char name[SHORT_STRING];

public:

	Application* App;

	Module(Application* parent, bool start_enabled = true) : App(parent), enabled(start_enabled)
	{}

	virtual ~Module()
	{}

	virtual bool Awake(JSONParser &config) 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual UPDATE_STATUS PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual UPDATE_STATUS Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual UPDATE_STATUS PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void OnGui(GUI_EVENTS ui_event, GameObject *ui_element)
	{ }

	virtual bool Save(JSONParser &module)
	{
		return true;
	}

	virtual bool Load(JSONParser &module)
	{
		return true;
	}

	void Enable()
	{
		if (!enabled)
			Start();
	}

	void Disable()
	{
		if (enabled)
			CleanUp();
	}

	bool IsEnabled() const
	{
		return enabled;
	}

	const char *GetModuleName() const
	{
		return name;
	}

};

#endif //__MODULE_H__