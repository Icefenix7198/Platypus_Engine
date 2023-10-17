#include "ComponentButtonUI.h"

#include "ResourceTexture.h"
#include "TextureImporter.h"

#include "imgui\imgui.h"

ComponentButtonUI::ComponentButtonUI() : Component()
{
	type = COMPONENT_TYPE::COMP_UI_BUTTON;
	name = "UI Button";

	idle = hover = pushed = nullptr;
	current_state = nullptr;
}

ComponentButtonUI::~ComponentButtonUI()
{
	// IDLE
	if (idle->texture_data != nullptr)
	{
		idle->DecrReferences();

		if (idle->GetNumReferences() == 0)
		{
			idle->UnloadFromMemory();
			RELEASE_ARRAY(idle->texture_data);
			idle->texture_data = nullptr;
		}
	}

	// HOVER
	if (hover->texture_data != nullptr)
	{
		hover->DecrReferences();

		if (hover->GetNumReferences() == 0)
		{
			hover->UnloadFromMemory();
			RELEASE_ARRAY(hover->texture_data);
			hover->texture_data = nullptr;
		}
	}

	// PUSHED
	if (pushed->texture_data != nullptr)
	{
		pushed->DecrReferences();

		if (pushed->GetNumReferences() == 0)
		{
			pushed->UnloadFromMemory();
			RELEASE_ARRAY(pushed->texture_data);
			pushed->texture_data = nullptr;
		}
	}
}

void ComponentButtonUI::InitIdleTexture(const char *tex_name)
{
	idle = (ResourceTexture*)App->resource_manager->Get(App->resource_manager->Find(tex_name));
	idle->IncrReferences();

	if (!idle->LoadedInMemory())
		TextureImporter::Load(idle->imported_file, idle);

	idle->LoadToMemory();

	current_state = idle;
}

void ComponentButtonUI::InitHoverTexture(const char *tex_name)
{
	hover = (ResourceTexture*)App->resource_manager->Get(App->resource_manager->Find(tex_name));
	hover->IncrReferences();

	if (!hover->LoadedInMemory())
		TextureImporter::Load(hover->imported_file, hover);

	hover->LoadToMemory();
}

void ComponentButtonUI::InitPushedTexture(const char *tex_name)
{
	pushed = (ResourceTexture*)App->resource_manager->Get(App->resource_manager->Find(tex_name));
	pushed->IncrReferences();

	if (!pushed->LoadedInMemory())
		TextureImporter::Load(pushed->imported_file, pushed);

	pushed->LoadToMemory();
}

void ComponentButtonUI::ShowEditorInfo()
{
	ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component: "); ImGui::SameLine();
	ImGui::Text(name);

	ImGui::Checkbox("Active##Mat", &active);
	ImGui::Text("Idle button texture");
	if (idle->tex_buffer != 0)
	{
		// Showing texture image on a 200x200 frame
		ImGui::Image((void*)idle->tex_buffer, ImVec2(119, 35), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(0.0f, 0.5f, 0.5f, 1.0f));
		ImGui::Text("%s%s", "Path: ", idle->tex_path);
	}

	ImGui::Text("Hover button texture");
	if (hover->tex_buffer != 0)
	{
		// Showing texture image on a 200x200 frame
		ImGui::Image((void*)hover->tex_buffer, ImVec2(119, 35), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(0.0f, 0.5f, 0.5f, 1.0f));
		ImGui::Text("%s%s", "Path: ", hover->tex_path);
	}

	ImGui::Text("Pushed button texture");
	if (pushed->tex_buffer != 0)
	{
		// Showing texture image on a 200x200 frame
		ImGui::Image((void*)pushed->tex_buffer, ImVec2(119, 35), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(0.0f, 0.5f, 0.5f, 1.0f));
		ImGui::Text("%s%s", "Path: ", pushed->tex_path);
	}

	ImGui::Separator();
}

bool ComponentButtonUI::Save(JSONParser &go)
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

bool ComponentButtonUI::Load(JSONParser &comp)
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