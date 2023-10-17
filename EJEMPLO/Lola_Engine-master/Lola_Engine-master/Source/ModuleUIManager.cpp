#include "ModuleUIManager.h"

#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"

#include "Component.h"
#include "ComponentTransform2D.h"
#include "ComponentImageUI.h"
#include "ComponentLabelUI.h"
#include "ComponentButtonUI.h"
#include "ComponentPropertiesUI.h"

#include "ResourceMesh.h"
#include "ResourceTexture.h"

#include "Mesh.h"

ModuleUIManager::ModuleUIManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	sprintf_s(name, SHORT_STRING, "UI_Manager");
}

// Destructor
ModuleUIManager::~ModuleUIManager()
{}

// Called before render is available
bool ModuleUIManager::Awake(JSONParser &config)
{
	DEBUG("Loading GUI atlas");
	bool ret = true;

	//atlas_file_name = conf.child("atlas").attribute("file").as_string("");*/

	return ret;
}

// Called before the first frame
bool ModuleUIManager::Start()
{
	//atlas = app->tex->loadTexture(atlas_file_name.GetString());

	current_ui = previous_ui = canvas;
	/*screen = new UI_Element();
	screen->interactable = false;
	focus = screen;
	previous_UIelement = screen;*/

	return true;
}

// Update all guis
UPDATE_STATUS ModuleUIManager::PreUpdate(float dt)
{
	if (UI_list.empty())
		return UPDATE_CONTINUE;

	math::float2 p = math::float2(App->input->GetMouseX(), App->input->GetMouseY());
	current_ui = whichUIelemOnMouse(p);

	if (current_ui != nullptr && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
		current_ui->transform_2d->Move({ (float)App->input->GetMouseXMotion(), (float)App->input->GetMouseYMotion(), 0.0f });


	std::list<Module*>::const_iterator it = current_ui->ui_properties->GetListeners().begin();
	while (it != current_ui->ui_properties->GetListeners().end())
	{
		if (current_ui != previous_ui)
		{
			(*it)->OnGui(MOUSE_ENTER, (GameObject*)current_ui);
			(*it)->OnGui(MOUSE_LEAVE, (GameObject*)previous_ui);
		}	

		++it;
	}

	//{
	//	if (current_UIelement != previous_UIelement)
	//		(*it)->OnGui(MOUSE_LEAVE, previous_UIelement);

	//	if (current_UIelement->interactable)
	//	{
	//		if (!current_UIelement->is_inside && current_UIelement->isMouseIn(p))
	//			(*it)->OnGui(MOUSE_ENTER, current_UIelement);

	//		if (App->input->GetMouseButton(0) == KEY_REPEAT)
	//			(*it)->OnGui(MOUSE_REPEAT_LEFT, current_UIelement);

	//		// Behaviour for left mouse button
	//		if (App->input->GetMouseButton(0) == KEY_DOWN)
	//		{
	//			(*it)->OnGui(MOUSE_CLICK_LEFT, current_UIelement);
	//			if (focus != current_UIelement && current_UIelement->can_be_focused)
	//			{
	//				(*it)->OnGui(LOST_FOCUS, focus);
	//				(*it)->OnGui(GAIN_FOCUS, current_UIelement);
	//			}
	//		}
	//		else if (App->input->GetMouseButton(0) == KEY_UP)
	//			(*it)->OnGui(MOUSE_LEAVE_LEFT, current_UIelement);

	//		// Behaviour for right mouse button
	//		if (App->input->GetMouseButton(1) == KEY_DOWN)
	//			(*it)->OnGui(MOUSE_CLICK_RIGHT, current_UIelement);
	//		else if (App->input->GetMouseButton(1) == KEY_UP)
	//			(*it)->OnGui(MOUSE_LEAVE_RIGHT, current_UIelement);
	//	}

	//	--it;  // Previous listener
	//}

	/*for (std::vector<UI_Element*>::iterator it = UIelement_list.begin(); it != UIelement_list.end(); ++it)
		(*it)->preUpdate();*/
	
	//previous_UIelement = current_UIelement;
	return UPDATE_CONTINUE;
}

// Called after all Updates
UPDATE_STATUS ModuleUIManager::PostUpdate(float dt)
{
	if (UI_list.empty())
		return UPDATE_CONTINUE;

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisable(GL_LIGHTING); // Panel mesh is not afected by lights!

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, App->window->GetScreenWidth(), App->window->GetScreenHeight(), 0, -1, 1);	//
	glMatrixMode(GL_MODELVIEW);             // Select Modelview Matrix
	glPushMatrix();							// Push The Matrix
	glLoadIdentity();

	for (std::vector<GameObject*>::iterator it = UI_list.begin(); it != UI_list.end(); ++it)
	{
		GameObject *curr_ui = (*it);
		const Mesh* panel = curr_ui->GetPanel();

		// Vertices
		glBindBuffer(GL_ARRAY_BUFFER, panel->id_vertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		// Texture coordinates
		glBindBuffer(GL_ARRAY_BUFFER, panel->id_tex_coord);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		ComponentImageUI *ui_image = (ComponentImageUI*)curr_ui->GetComponentByType(COMPONENT_TYPE::COMP_UI_IMAGE);
		if (ui_image != nullptr)
		{
			// Texture
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindTexture(GL_TEXTURE_2D, ui_image->resource->tex_buffer);
		}	

		ComponentLabelUI *ui_label = (ComponentLabelUI*)curr_ui->GetComponentByType(COMPONENT_TYPE::COMP_UI_LABEL);
		if (ui_label != nullptr)
		{
			// Texture
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindTexture(GL_TEXTURE_2D, ui_label->resource->tex_buffer);
		}

		ComponentButtonUI *ui_button = (ComponentButtonUI*)curr_ui->GetComponentByType(COMPONENT_TYPE::COMP_UI_BUTTON);
		if (ui_button != nullptr)
		{
			// Texture
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindTexture(GL_TEXTURE_2D, ui_button->current_state->tex_buffer);
		}
		
		// Indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, panel->id_indices);
		glDrawElements(GL_TRIANGLES, panel->num_indices, GL_UNSIGNED_INT, NULL);
	}	

	glMatrixMode(GL_PROJECTION);              // Select Projection
	glPopMatrix();							  // Pop The Matrix
	glMatrixMode(GL_MODELVIEW);               // Select Modelview
	glPopMatrix();							  // Pop The Matrix

	glEnable(GL_LIGHTING);

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleUIManager::CleanUp()
{
	DEBUG("Freeing GUI");

	/*for (std::vector<UI_Element*>::iterator it = UIelement_list.begin(); it != UIelement_list.end(); ++it)
		delete (*it);
	*/
	//UIelement_list.clear();

	// Deleting screen root for tree
	//delete screen;

	return true;
}

// const getter for atlas
//const SDL_Texture* ModuleUIManager::getAtlas() const
//{
//	return atlas;
//}
//
//const UI_Element* ModuleUIManager::whichFocus() const
//{
//	return focus;
//}

//// Factories for class Gui ---------------------------------------------------
//UI_Label* ModuleUIManager::CreateLabel(math::float2 p, const char *string, _TTF_Font *font, Module *mod, UI_Element *parent)
//{
//	UI_Label *l = new UI_Label();
//	//l->init(p, string, font, mod, parent);
//	//UIelement_list.push_back(l);
//	return l;
//}
//
//UI_Image* ModuleUIManager::CreateImage(math::float2 p, SDL_Texture *tex, SDL_Rect &section, Module *mod, UI_Element *parent)
//{
//	UI_Image *i = new UI_Image();
//	//i->init(p, tex, section, mod, parent);
//	//UIelement_list.push_back(i);
//	return i;
//}
//
//UI_Button *ModuleUIManager::CreateButton(math::float2 p, SDL_Texture *tex_idle, SDL_Rect& section_idle, SDL_Texture *tex_hover,
//								  SDL_Rect& section_hover, SDL_Texture *tex_clicked, SDL_Rect& section_clicked,
//								  Module *mod, UI_Element *parent)
//{
//	UI_Button *b = new UI_Button();
//	//b->init(p, tex_idle, section_idle, tex_hover, section_hover, tex_clicked, section_clicked, mod, parent);
//	//UIelement_list.push_back(b);
//
//	return b;	
//}

void ModuleUIManager::CreateImage(const math::float3 &pos, const math::float2 &size)
{
	GameObject *ui_image = CreateBaseElement("UI_Image", pos, size);
	ComponentImageUI *image = (ComponentImageUI*)ui_image->AddComponent(COMPONENT_TYPE::COMP_UI_IMAGE);

	image->AddResource(App->resource_manager->Get(App->resource_manager->Find("Tigger.png")));
	image->InitTexture("Tigger.png");

	UI_list.push_back(ui_image);
}

void ModuleUIManager::CreateLabel(const math::float3 &pos, const math::float2 &size)
{
	GameObject *ui_label = CreateBaseElement("UI_Label", pos, size);
	ComponentLabelUI *label = (ComponentLabelUI*)ui_label->AddComponent(COMPONENT_TYPE::COMP_UI_LABEL);

	label->AddResource(App->resource_manager->CreateNewResource(RESOURCE_TYPE::RES_TEXTURES, App->resource_manager->GenerateID(), 1));
	label->SetText();

	UI_list.push_back(ui_label);
}

void ModuleUIManager::CreateButton(const math::float3 &pos, const math::float2 &size)
{
	GameObject *ui_button = CreateBaseElement("UI_Button", pos, size);
	ComponentButtonUI *button = (ComponentButtonUI*)ui_button->AddComponent(COMPONENT_TYPE::COMP_UI_BUTTON);

	button->InitIdleTexture("idle_button.png");
	button->InitHoverTexture("hover_button.png");
	button->InitPushedTexture("pushed_button.png");

	UI_list.push_back(ui_button);
}

void ModuleUIManager::CreateCanvas()
{
	canvas = App->gameobject_manager->CreateGameObject("Canvas", nullptr);
	canvas->AddComponent(COMPONENT_TYPE::COMP_TRANSFORM_2D);
}

GameObject *ModuleUIManager::CreateBaseElement(const char *name_ui, const math::float3 &pos, const math::float2 &size)
{
	if (canvas == nullptr)
		CreateCanvas();

	GameObject *ui = App->gameobject_manager->CreateGameObject(name_ui, canvas);

	ui->AddComponent(COMPONENT_TYPE::COMP_TRANSFORM_2D);
	ui->transform_2d->SetLocalPos(pos);
	ui->transform_2d->SetSize(size);

	ui->AddComponent(COMPONENT_TYPE::COMP_UI_PROPERTIES);
	ui->ui_properties->AddListener(this);

	return ui;
}

//UIinputBox *Gui::createInputBox(iPoint pos, iPoint text_offset, SDL_Texture *frame_tex, SDL_Rect &frame_section, const char *initial_text,
//									  _TTF_Font *font, Module *module, UIelement *parent)
//{
//	UIinputBox *i = new UIinputBox();
//	i->init(pos, text_offset, frame_tex, frame_section, initial_text, font, module, parent);
//	UIelement_list.add(i);
//	return i;
//}
//
//UIHorizontalScrollBar *Gui::createHorizontalScrollBar(iPoint pos, int bar_offset, int thumb_vert_offset, SDL_Texture *bar_tex, SDL_Rect &section_bar,
//	SDL_Texture *thumb_tex, SDL_Rect &section_thumb, Module *module, UIelement *parent)
//{
//	UIHorizontalScrollBar *h = new UIHorizontalScrollBar();
//	h->init(pos, bar_offset, thumb_vert_offset, bar_tex, section_bar, thumb_tex, section_thumb, module, parent);
//	UIelement_list.add(h);
//	return h;
//}

void ModuleUIManager::OnGui(GUI_EVENTS mouse_event, UI_Element *trigger)
{
	//switch (trigger->type)
	//{
	//case UI_BUTTON:
	//{
	//	UI_Button *b = (UI_Button*)trigger;
	//	switch (mouse_event)
	//	{
	//	case MOUSE_ENTER:
	//		b->setHoverState();
	//		b->is_inside = true;	
	//		break;
	//	case MOUSE_LEAVE:
	//		b->setIdleState();
	//		b->is_inside = false;
	//		break;
	//	case MOUSE_CLICK_LEFT:
	//	case MOUSE_CLICK_RIGHT:
	//		b->setClickedState();
	//		break;
	//	case MOUSE_REPEAT_LEFT:
	//		b->dragElement();
	//		break;
	//	case MOUSE_LEAVE_LEFT:
	//	case MOUSE_LEAVE_RIGHT:
	//		b->setHoverState();
	//		break;
	//	case GAIN_FOCUS:
	//		if (focus != trigger)
	//		{
	//			focus = trigger;
	//			//app->input->stopTextInput();
	//		}
	//		break;
	//	}
	//	break;
	//}

	//case UI_IMAGE:
	//{
	//	UI_Image *i = (UI_Image*)trigger;
	//	switch (mouse_event)
	//	{
	//	case MOUSE_REPEAT_LEFT:
	//		i->dragElement();
	//		break;
	//	}
	//	break;
	//}

	/*case UI_INPUTBOX:
	{
		UIinputBox *i = (UIinputBox*)trigger;
		switch (mouse_event)
		{
		case GAIN_FOCUS:
			if (focus != trigger)
			{
				focus = trigger;
				app->input->startTextInput(i);

				if (!i->active && strcmp(i->text.text.GetString(), i->default_string) == 0)
					i->text.setText("");
				i->active = true;
			}
			break;
		case LOST_FOCUS:
				i->active = false;
			break;
		case MOUSE_REPEAT_LEFT:
			i->dragElement();
			break;
		case TEXT_CHANGED:
			i->moveCursor();
			break;
		}
		break;
	}*/

	//// EXERCISE 2
	//case UI_HORIZONTALSCROLLBAR:
	//{
	//	UIHorizontalScrollBar *h = (UIHorizontalScrollBar*)trigger;
	//	switch (mouse_event)
	//	{
	//	case GAIN_FOCUS:
	//		if (focus != trigger)
	//			focus = trigger;
	//		break;
	//	case MOUSE_REPEAT_LEFT:
	//		iPoint p;
	//		app->input->getMousePosition(p);
	//		if (focus == trigger && h->thumb.isMouseIn(p))
	//			h->dragElement();
	//		break;
	//	}
	//	break;
	//}
	//}
}

const GameObject *ModuleUIManager::whichUIelemOnMouse(const math::float2 &mouse_pos) const
{
	for (std::vector<GameObject*>::const_reverse_iterator it = UI_list.rbegin(); it != UI_list.rend(); ++it)
	{
		if ((*it)->transform_2d->ContainsPoint(mouse_pos))
			return (*it);
	}

	return nullptr;
}
