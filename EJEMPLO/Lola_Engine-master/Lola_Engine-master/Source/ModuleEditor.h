#ifndef __MODULEEDITOR_H__
#define __MODULEEDITOR_H__

#include "Module.h"
#include "Globals.h"

class GameObject;

class ModuleEditor : public Module
{
	public:

		GameObject *go_selected = nullptr;

		ModuleEditor(Application* app, bool start_enabled = true);
		~ModuleEditor();

		bool Awake(JSONParser &config);
		UPDATE_STATUS PreUpdate(float dt);
		UPDATE_STATUS Update(float dt);
		UPDATE_STATUS PostUpdate(float dt);
		bool CleanUp();	

		void ChangeSelectedGameObject(GameObject *new_go);

	private:

		int node_flags, leaf_flags;
		long unsigned int UUID_selected = 0;
		
		//Windows variables
		bool about_menu = false;
		bool conf_menu = false;
		bool console_menu = false;
		bool hierarchy_menu = false;
		bool component_menu = false;
		bool warning_alert = false;

		bool load_menu = false;

		// Windows methods
		void ShowMenuBar();
		void ShowMenuFile();
		void ShowCreateMenu();
		void ShowAboutMenu();
		void ShowConfMenu();
		void ShowConsole();
		void ShowHierarchy();
		void ShowComponentInfo();
		void ShowWarning();
		void ShowLoadMenu(); // Incompleted!
		void ShowTimeControl();

		void ExpandTree(const GameObject* go_to_expand);

		bool Save(JSONParser &module);
		bool Load(JSONParser &module);
};

#endif // !__MODULEIMGUI_H__

