#ifndef __MODULEUIMANAGER_H__
#define __MODULEUIMANAGER_H__

#include "Module.h"
//#include "SDL\include\SDL_render.h"

#include "ModuleGameObjectManager.h"

class UI_Image;
class UI_Button;
class UI_Label;
class UI_InputBox;
class UI_HorizontalScrollBar;
class UI_Element;
struct _TTF_Font;

enum GUI_EVENTS
{
	MOUSE_ENTER,
	MOUSE_LEAVE,
	MOUSE_CLICK_LEFT,
	MOUSE_CLICK_RIGHT,
	MOUSE_LEAVE_LEFT,
	MOUSE_LEAVE_RIGHT,
	MOUSE_REPEAT_LEFT,
	GAIN_FOCUS,
	LOST_FOCUS,
	TEXT_CHANGED,
	DRAGGED
};

// ---------------------------------------------------
class ModuleUIManager : public Module
{
public:

	GameObject		*canvas = nullptr;

	ModuleUIManager(Application* app, bool start_enabled = true);
	~ModuleUIManager();

	bool Awake(JSONParser &config);

	bool Start();
	
	UPDATE_STATUS PreUpdate(float dt);
	UPDATE_STATUS PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	// On Gui for own behaviour
	void OnGui(GUI_EVENTS mouse_event, UI_Element *trigger);

	// Which element is under the mouse	
	const GameObject *whichUIelemOnMouse(const math::float2 &mouse_pos) const;

	// Gui creation functions
	/*UI_Label *CreateLabel(math::float2 p, const char *string, _TTF_Font *font = NULL, Module *mod = NULL, UI_Element *parent = NULL);
	UI_Image *CreateImage(math::float2 p, SDL_Texture *tex, SDL_Rect &section, Module *mod = NULL, UI_Element *parent = NULL);
	UI_Button *CreateButton(math::float2 p, SDL_Texture *tex_idle, SDL_Rect& section_idle, SDL_Texture *tex_hover,
									  SDL_Rect& section_hover, SDL_Texture *tex_clicked, SDL_Rect& section_clicked,
									  Module *mod = NULL, UI_Element *parent = NULL);
	UI_InputBox *CreateInputBox(math::float2 pos, math::float2 text_offset, SDL_Texture *frame_tex, SDL_Rect &frame_section, const char *initial_text,
									      _TTF_Font *font, Module *module = NULL, UI_Element *parent = NULL);
	UI_HorizontalScrollBar *CreateHorizontalScrollBar(math::float2 pos, int bar_offset, int thumb_vert_offset, SDL_Texture *bar_tex, SDL_Rect &section_bar,
								SDL_Texture *thumb_tex, SDL_Rect &section_thumb, Module *module = NULL, UI_Element *parent = NULL);*/

	// New factory functions with components
	void CreateImage(const math::float3 &pos = math::float3::zero, const math::float2 &size = math::float2(100.0f, 100.0f));
	void CreateLabel(const math::float3 &pos = math::float3::zero, const math::float2 &size = math::float2(100.0f, 100.0f));
	void CreateButton(const math::float3 &pos = math::float3::zero, const math::float2 &size = math::float2(119.0f, 35.0f));

	/*const SDL_Texture* getAtlas() const;
	const UI_Element* whichFocus() const;*/
	

private:	

	//UI_Element				*focus;
	const GameObject				*current_ui = nullptr;
	const GameObject				*previous_ui = nullptr;

	std::vector<GameObject*>	UI_list;
	//SDL_Texture*				atlas;
	//p2SString					atlas_file_name;

	void CreateCanvas();
	GameObject *CreateBaseElement(const char *name_ui, const math::float3 &pos, const math::float2 &size);
	
};

#endif __MODULEUIMANAGER_H__