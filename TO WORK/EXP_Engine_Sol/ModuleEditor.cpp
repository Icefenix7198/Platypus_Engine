#include "Application.h"
#include "ModuleEditor.h"
#include "imGui/imgui.h"
#include "imGui/backends/imgui_impl_opengl3.h"
#include "imGui/backends/imgui_impl_SDL2.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{
 //   SDL_GLContext gl_context = SDL_GL_CreateContext(App->window->window);
 //   
	//IMGUI_CHECKVERSION();
 //   ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

 //   // Setup ImGui style
 //   ImGui::StyleColorsDark();
 //   //ImGui::StyleColorsLight();

 //   // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
 //   ImGuiStyle& style = ImGui::GetStyle();
 //   if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
 //   {
 //       style.WindowRounding = 0.0f;
 //       style.Colors[ImGuiCol_WindowBg].w = 1.0f;
 //   }
    
    // Setup Platform/Renderer backends
    //ImGui_ImplSDL2_InitForOpenGL(App->window->window, gl_context); //@ANDREU This causes an special error because the backend is alredy declared
    //ImGui_ImplOpenGL3_Init(); //@ANDREU This causes an special error because the backend is alredy declared

	cpuCache = SDL_GetCPUCacheLineSize();
	cpus = SDL_GetCPUCount();
	ram = SDL_GetSystemRAM();
	return true;
}

void ModuleEditor::DrawEditor()
{
	//Barra de arriba del editor (esto tendra que ir en ModuleEditor)
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::BeginMenu("Options"))
			{	
				bool VSYNCactive = App->window->GetVSYNC();
				if(ImGui::Checkbox("VSYNC", &VSYNCactive))
				{
					App->window->SetVSYNC(VSYNCactive);
				}
				float winScale = App->window->GetScreenSize();
				if (ImGui::SliderFloat("Window Scale", &winScale,0.1f,1.0f))
				{
					App->window->SetScreenSize(winScale);
				}
			
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Assets"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("GameObject"))
		{
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (ImGui::Begin("Hardware"))
	{
		ImGui::Text("CPUs: %i (Cache: %ikb)", cpus, cpuCache);
		ImGui::Text("System RAM: %iMb", ram);
	}
	ImGui::End();
	//Testeo
	AddFPS(0.16);
}

bool ModuleEditor::CleanUp()
{
	mFPSLog.clear();
	return true;
}

void ModuleEditor::AddFPS(const float aFPS)
{
	mFPSLog.push_back(aFPS);
	ImGui::PlotHistogram("FPS", mFPSLog.data(), mFPSLog.size(),0,NULL,0.0f,1.0f, ImVec2(0, 80.0f)); //Name,iterador,tamaño vector, ??,???,???,???, tamaño espacio azul
	
	if (mFPSLog.size()>=25)
	{
		//Hacer copia del vector inversa
		mFPSLog.clear();
	}



}
