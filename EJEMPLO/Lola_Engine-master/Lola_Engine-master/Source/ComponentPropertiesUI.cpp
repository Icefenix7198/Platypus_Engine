#include "ComponentPropertiesUI.h"

#include "imgui\imgui.h"

ComponentPropertiesUI::ComponentPropertiesUI() : Component()
{
	type = COMPONENT_TYPE::COMP_UI_PROPERTIES;
	name = "UI Properties";

	interactable = false;
	draggable = false;
}

ComponentPropertiesUI::~ComponentPropertiesUI()
{ }


void ComponentPropertiesUI::ShowEditorInfo()
{
	ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component: "); ImGui::SameLine();
	ImGui::Text(name);

	ImGui::Separator();
}

void ComponentPropertiesUI::AddListener(Module *listener)
{
	listeners.push_back(listener);
}

const std::list<Module*> &ComponentPropertiesUI::GetListeners() const
{
	return listeners;
}

bool ComponentPropertiesUI::Save(JSONParser &go)
{
	//JSONParser component;

	//component.AddInt("Type", type);
	//component.AddBoolean("Active", active);
	//component.AddInt("Opacity", resource->opacity);

	//// Colors and opacity is loaded into Resource, so if the resource exists, it will have these information
	//component.AddPoints("Color diffuse", resource->color_diffuse, 3);
	//component.AddPoints("Color specular", resource->color_specular, 3);
	//component.AddPoints("Color ambient", resource->color_ambient, 3);
	//component.AddPoints("Color emissive", resource->color_emissive, 3);
	//component.AddPoints("Color transparent", resource->color_transparent, 3);

	//component.AddUUID("Resource ID", resource->id);

	//go.AddArray(component);

	return true;
}

bool ComponentPropertiesUI::Load(JSONParser &comp)
{
	/*AddResource(App->resource_manager->Get(comp.GetUUID("Resource ID")));

	active = comp.GetBoolean("Active");
	resource->opacity = comp.GetInt("Opacity");

	comp.GetPoints("Color diffuse", resource->color_diffuse, 3);
	comp.GetPoints("Color specular", resource->color_specular, 3);
	comp.GetPoints("Color ambient", resource->color_ambient, 3);
	comp.GetPoints("Color emissive", resource->color_emissive, 3);
	comp.GetPoints("Color transparent", resource->color_transparent, 3);*/

	return true;
}