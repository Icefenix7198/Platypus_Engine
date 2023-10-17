#include "ModuleEditor.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleGameObjectManager.h"
#include "ModuleUIManager.h"

#include "GameObject.h"
#include "Component.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_sdl_gl3.h"

#include "Console.h"

// Constructor
ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	sprintf_s(name, SHORT_STRING, "Editor");
}

// Destructor
ModuleEditor::~ModuleEditor()
{ }

bool ModuleEditor::Awake(JSONParser &config)
{	
	if (App->window->window != NULL)
	{
		// Setup ImGui binding
		DEBUG("Setup ImGUI binding");
		ImGui_ImplSdlGL3_Init(App->window->window);
	}		

	node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	leaf_flags = node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

	return true;
}

UPDATE_STATUS ModuleEditor::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	
	if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)
		console_menu = !console_menu;

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		hierarchy_menu = !hierarchy_menu;

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		conf_menu = !conf_menu;

	if (App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN && go_selected)
		warning_alert = true;

	/*if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
		load_menu = !load_menu;*/
	
	return UPDATE_CONTINUE;
}

UPDATE_STATUS ModuleEditor::Update(float dt)
{
	ShowMenuBar();
	ShowTimeControl();
	//ImGui::ShowTestWindow();

	if (about_menu) ShowAboutMenu();
	if (conf_menu) ShowConfMenu();
	if (console_menu) ShowConsole();
	if (hierarchy_menu) ShowHierarchy();
	if (warning_alert) ShowWarning();
	//if (load_menu) ShowLoadMenu();

	ShowComponentInfo();

	return UPDATE_CONTINUE;
}

UPDATE_STATUS ModuleEditor::PostUpdate(float dt)
{
	ImGui::Render();
	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	DEBUG("Closing ImGUI");
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}

// Showing Windows
void ModuleEditor::ShowMenuBar()
{
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File"))
	{
		ShowMenuFile();
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Create"))
	{
		ShowCreateMenu();
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Debug"))
	{	
		ImGui::MenuItem("Show AABB", nullptr, &App->debug_mode.show_aabb);
		ImGui::MenuItem("Show Frustum", nullptr, &App->debug_mode.show_frustum);
		ImGui::MenuItem("Show OcTree", nullptr, &App->debug_mode.show_octree);
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Help"))
	{
		ImGui::MenuItem("About", NULL, &about_menu);
		if (ImGui::MenuItem("GitHub Wiki"))
			App->RequestBrowser("https://github.com/crandino/Lola_Engine/wiki");
		ImGui::EndMenu();
	}	

	ImGui::EndMainMenuBar();
}

void ModuleEditor::ShowMenuFile()
{
	if (ImGui::MenuItem("Quit", "ESC")) { App->CloseApp(); }	
}

void ModuleEditor::ShowCreateMenu()
{
	if (ImGui::MenuItem("Camera")) { App->gameobject_manager->CreateCamera(); }
	if (ImGui::MenuItem("UI Image")) { App->ui_manager->CreateImage(); }
	if (ImGui::MenuItem("UI Label")) { App->ui_manager->CreateLabel(); }
	if (ImGui::MenuItem("UI Button")) { App->ui_manager->CreateButton(); }
}

void ModuleEditor::ShowAboutMenu()
{
	ImGui::Begin("About", &about_menu, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("LOLA Engine");
	ImGui::Separator();
	ImGui::Text("3D Engine created and developed by Carlos Randino (crandino on GitHub) in VideoGame");
	ImGui::Text("Engines Subject on CITM (Terrassa). This engine is hosted on a GitHub repository");
	ImGui::Text("(available on Help/GitHub Wiki menu). LOLA Engine is under a MIT License.");
	ImGui::End();
}

void ModuleEditor::ShowConfMenu()
{
	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Configuration", &conf_menu);//ImGuiWindowFlags_AlwaysAutoResize
	if(ImGui::CollapsingHeader("Application"))
	{
		// App name and organization
		ImGui::InputText("App name", (char*)App->GetAppName(), 128);
		ImGui::InputText("Organization", (char*)App->GetOrganization(), 128);
		ImGui::Separator();

		// FPS and dt histograms
		char title[25];
		ImGui::SliderInt("Max FPS", &App->perf_info.getFrameRate(), 0, 75);
		sprintf_s(title, 25, "Framerate %.1f", App->perf_info.getFrameRateHist()[HISTOGRAM_VALUES - 1]);
		ImGui::PlotHistogram("##framerate", App->perf_info.getFrameRateHist(), HISTOGRAM_VALUES, 0, title, 0.0f, 75.0f, ImVec2(310, 100));
		sprintf_s(title, 25, "Miliseconds %.1f", App->perf_info.getMilisecondsHist()[HISTOGRAM_VALUES - 1]);
		ImGui::PlotHistogram("##miliseconds", App->perf_info.getMilisecondsHist(), HISTOGRAM_VALUES, 0, title, 0.0f, 40.0f, ImVec2(310, 100));
	}	
	if (ImGui::CollapsingHeader("Software & Hardware"))
	{
		ImGui::LabelText("", "%s", "Software");
		ImGui::Separator();

		// SDL
		static SDL_version ver; SDL_GetVersion(&ver);
		ImGui::LabelText("", "%s", "SDL Version:"); ImGui::SameLine();		
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "%u.%u.%u", ver.major, ver.minor, ver.patch);

		// OpenGL
		ImGui::LabelText("", "%s", "OpenGL Version:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "%s", App->renderer3D->gl_version);

		// Glew
		ImGui::LabelText("", "%s", "GLew Version:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "%s", App->renderer3D->glew_version);

		// Bullet
		ImGui::LabelText("", "%s", "Bullet Version:");// ImGui::SameLine();
		//ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "%u.%u", BT_BULLET_VERSION / 100, BT_BULLET_VERSION % 200);

		// ImGUI
		ImGui::LabelText("", "%s", "ImGUI Version:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "%s", ImGui::GetVersion());

		//MathGeoLib
		ImGui::LabelText("", "%s", "MathGeoLib Version:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "%s", "1.5");
	}
	if (ImGui::CollapsingHeader("Window"))
	{
		ImGui::LabelText("", "%s", "Software");
		ImGui::Separator();

		if(ImGui::SliderInt("Screen Width", &App->window->GetScreenWidth(), 600, 1920)) App->window->ChangeWindowSize();
		if(ImGui::SliderInt("Screen Height", &App->window->GetScreenHeight(), 400, 1080)) App->window->ChangeWindowSize();
		if(ImGui::CheckboxFlags("Fullscreen", &App->window->flags, SDL_WINDOW_FULLSCREEN)) App->window->SetFullscreenOptions();
		ImGui::SameLine();
		if(ImGui::CheckboxFlags("Fullscreen desktop", &App->window->flags, SDL_WINDOW_FULLSCREEN_DESKTOP)) App->window->SetFullscreenOptions();

		// SDL
		static SDL_version ver; SDL_GetVersion(&ver);
		ImGui::LabelText("", "%s", "SDL Version:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "%u.%u.%u", ver.major, ver.minor, ver.patch);

		// OpenGL
		ImGui::LabelText("", "%s", "OpenGL Version:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "%s", App->renderer3D->gl_version);

		// Glew
		ImGui::LabelText("", "%s", "GLew Version:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "%s", App->renderer3D->glew_version);

		// Bullet
		ImGui::LabelText("", "%s", "Bullet Version:");// ImGui::SameLine();
													  //ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "%u.%u", BT_BULLET_VERSION / 100, BT_BULLET_VERSION % 200);

													  // ImGUI
		ImGui::LabelText("", "%s", "ImGUI Version:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "%s", ImGui::GetVersion());

		//MathGeoLib
		ImGui::LabelText("", "%s", "MathGeoLib Version:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "%s", "1.5");
	}

	ImGui::End();
}

void ModuleEditor::ShowConsole()
{
	App->console->Draw("Console", &console_menu);
}

void ModuleEditor::ShowHierarchy()
{
	ImGui::Begin("Hierarchy", &hierarchy_menu);	
	ExpandTree(App->gameobject_manager->GetRoot());
	ImGui::End();
}

void ModuleEditor::ShowComponentInfo()
{
	if (go_selected != nullptr)
	{
		float width = 350.0f;
		ImGui::SetNextWindowSizeConstraints(ImVec2(width, 0), ImVec2(width, FLT_MAX));
		ImGui::SetNextWindowPos(ImVec2(App->window->GetScreenWidth() - width, 20.0f));
		ImGui::Begin(go_selected->GetName(), nullptr, ImGuiWindowFlags_ShowBorders);

		if (ImGui::Checkbox("Active", &go_selected->active))
			go_selected->ChangeActiveState(&go_selected->active);
		ImGui::SameLine();
		ImGui::Checkbox("Static", &go_selected->bstatic);
		ImGui::Separator();

		for (uint i = 0; i < go_selected->components.size(); ++i)
			go_selected->components[i]->ShowEditorInfo();			

		ImGui::End();
	}	
}

void ModuleEditor::ShowWarning()
{
	ImGuiWindowFlags window_flags = (ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
									 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
	ImGui::SetNextWindowPos(ImVec2(App->window->GetScreenWidth()/ 2.0f, App->window->GetScreenHeight() / 2.0f), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Warning", nullptr, window_flags);
	ImGui::Text("%s%s%s", "Are you sure you wanna delete ", go_selected->GetName(), " ?");

	ImVec2 button_size = ImVec2(80.0f, 20.0f);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.5f, 0.0f, 0.75f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.75f, 0.0f, 0.75f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.75f, 0.0f, 0.9f));
	if (ImGui::Button("Yes", button_size))
	{
		App->gameobject_manager->MarkChildToDelete(go_selected);
		go_selected = nullptr;
		warning_alert = false;
	}
		
	ImGui::PopStyleColor(3);

	ImGui::SameLine(0.0f, 100.0f);

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.0f, 0.0f, 0.75f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.75f, 0.0f, 0.0f, 0.75f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.75f, 0.0f, 0.0f, 0.9f));
	if (ImGui::Button("No", button_size))
		warning_alert = false;
	ImGui::PopStyleColor(3);

	ImGui::End();
}

void ModuleEditor::ShowLoadMenu()
{
	ImGui::OpenPopup("Load File");
	if (ImGui::BeginPopupModal("Load File", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		//in_modal = true;

		ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
		ImGui::BeginChild("File Browser", ImVec2(0, 300), true);
		//DrawDirectoryRecursive(from_dir, filter_extension);
		ImGui::EndChild();
		ImGui::PopStyleVar();

		ImGui::PushItemWidth(250.f);
		/*if (ImGui::InputText("##file_selector", selected_file, FILE_MAX, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			file_dialog = ready_to_close;*/

		ImGui::PopItemWidth();
		ImGui::SameLine();
		/*if (ImGui::Button("Ok", ImVec2(50, 20)))
			file_dialog = ready_to_close;*/
		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(50, 20)))
		{
			/*file_dialog = ready_to_close;
			selected_file[0] = '\0';*/
		}

		ImGui::EndPopup();
	}
	else
	{
		//in_modal = false;
	}
		
}

void ModuleEditor::ShowTimeControl()
{
	ImGuiWindowFlags window_flags = (ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
									 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
	ImGui::SetNextWindowPos(ImVec2(App->window->GetScreenWidth() / 2.0f, 30.0f), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Time Control", nullptr, window_flags);

	if (App->GetEngineMode() == ENGINE_MODE::EDITOR)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.75f, 0.0f, 0.0f, 0.8f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.75f, 0.1f, 0.1f, 0.9f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.0f, 0.0f, 0.9f));
	}		
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.75f, 0.0f, 0.8f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.75f, 0.1f, 0.9f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.9f, 0.0f, 0.9f));
	}		

	if (ImGui::Button("PLAY"))
		App->ChangeEngineMode();
	ImGui::SameLine();
	if (ImGui::Button("PAUSE"))
		App->PausePlayMode();
	ImGui::SameLine();
	ImGui::Button("NEXT FRAME"); // Not requiered for Assigment 2, only Play, Stop and Pause.
	ImGui::SameLine();

	ImGui::PopStyleColor(3);

	ImGui::End();
}

// Expand Tree recursively shows all GameObjects on the Hierarchy Window
void ModuleEditor::ExpandTree(const GameObject* go_to_expand)
{
	int num_children = go_to_expand->children.size();
	GameObject *child = nullptr;

	for (int i = 0; i < num_children; ++i)
	{
		child = go_to_expand->children[i];
		node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((child->UUID == UUID_selected) ? ImGuiTreeNodeFlags_Selected : 0);

		if (child->children.size() > 0)
		{
			bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, child->GetName());

			if (ImGui::IsItemClicked())
				ChangeSelectedGameObject(child);

			if (node_open)
			{
				ExpandTree(child);
				ImGui::TreePop();
			}
		}
		else
		{
			leaf_flags = node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
			ImGui::TreeNodeEx((void*)(intptr_t)i, leaf_flags, child->GetName());
			if (ImGui::IsItemClicked())
				ChangeSelectedGameObject(child);
		}
	}
}

void ModuleEditor::ChangeSelectedGameObject(GameObject *new_go)
{
	// Previous GO selected becomes unselected.
	if (go_selected)
	{
		go_selected->selected = false;
		go_selected = nullptr;
	}		
	
	if (new_go != nullptr)
	{
		// Now, we make the new GO selected and use its ID to show highlighted	
		go_selected = new_go;
		go_selected->selected = true;
		UUID_selected = new_go->UUID;
	}
}

bool ModuleEditor::Save(JSONParser &module)
{
	module.AddBoolean("About menu", about_menu);
	module.AddBoolean("Configuration menu", conf_menu);
	module.AddBoolean("Console menu", console_menu);
	module.AddBoolean("Hierarchy menu", hierarchy_menu);
	module.AddBoolean("Component menu", component_menu);
	module.AddBoolean("Warning alert", warning_alert);

	if (go_selected != nullptr)
	{
		module.AddBoolean("Gameobject selected", true);
		module.AddUUID("Gameobject selected ID", go_selected->UUID);
		module.AddUUID("ID selected", UUID_selected);
	}
	else
		module.AddBoolean("Gameobject selected", false);	

	return true;
}

bool ModuleEditor::Load(JSONParser &module)
{
	about_menu = module.GetBoolean("About menu");
	conf_menu = module.GetBoolean("Configuration menu");
	console_menu = module.GetBoolean("Console menu");
	hierarchy_menu = module.GetBoolean("Hierarchy menu");
	component_menu = module.GetBoolean("Component menu");
	warning_alert = module.GetBoolean("Warning alert");

	if (module.GetBoolean("Gameobject selected"))
	{
		ChangeSelectedGameObject(App->gameobject_manager->Get(module.GetUUID("Gameobject selected ID")));
		/*UUID_selected = module.GetUUID("ID selected");*/
	}
	
	return true;
}
