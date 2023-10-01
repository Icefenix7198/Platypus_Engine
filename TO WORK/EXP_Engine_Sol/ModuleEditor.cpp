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

	caps[0] = SDL_Has3DNow();
	caps[1] = SDL_HasAltiVec();
	caps[2] = SDL_HasAVX();
	caps[3] = SDL_HasAVX2();
	caps[4] = SDL_HasMMX();
	caps[5] = SDL_HasRDTSC();
	caps[6] = SDL_HasSSE();
	caps[7] = SDL_HasSSE2();
	caps[8] = SDL_HasSSE3();
	caps[9] = SDL_HasSSE41();
	caps[10] = SDL_HasSSE42();


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
		if (ImGui::BeginMenu("Show"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::BeginMenu("About"))
			{
				//Quzas para no ocupar tanto espacio aqui a�ado un about.h
				ImGui::Text("Platypus Engine v0.1\n"
							"The Wildest 3D Videogame Engine\n"
							"By Eric Segovia & Andreu Miro\n"
							"https://github.com/Icefenix7198/Platypus_Engine \n\n"
							"Third party libraries used:\n"
							"- ImGui v1.89.9 \n- SDL2 \n- MathGeoLib \n- OpenGL \n- Glew \n- mmgr \n- parson \n\n"
							"License: \nMIT License \n\n"
							"Copyright (c) 2023 by Eric Segovia & Andreu Miro \n\n"
							"Permission is hereby granted, free of charge, to any person obtaining\n"
							"a copy of this software and associated documentation files(the\n"
							"Software""), to deal in the Software without restriction, including\n"
							"without limitation the rights to use, copy, modify, merge, publish,\n"
							"distribute, sublicense, and /or sell copies of the Software, and to\n"
							"permit persons to whom the Software is furnished to do so, subject to\n"
							"the following conditions :\n\n"

							"The above copyright noticeand this permission notice shall be\n"
							"included in all copies or substantial portions of the Software.\n\n"

							"THE SOFTWARE IS PROVIDED ""AS IS"", WITHOUT WARRANTY OF ANY KIND,\n"
							"EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF\n"
							"MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND\n"
							"NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE\n"
							"LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION\n"
							"OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION\n"
							"WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (ImGui::Begin("Hardware"))
	{
		ImGui::Text("CPUs: %i (Cache: %ikb)", cpus, cpuCache);
		ImGui::Text("System RAM: %iMb", ram);
		ImGui::Text("Caps: ");
		if (caps[0] == true) { ImGui::Text("3D"); }
		if (caps[1] == true) { ImGui::Text("AltiVec"); }
		if (caps[2] == true) { ImGui::Text("AVX"); }
		if (caps[3] == true) { ImGui::Text("AVX2"); }
		if (caps[4] == true) { ImGui::Text("MMX"); }
		if (caps[5] == true) { ImGui::Text("RDTSC"); }
		if (caps[6] == true) { ImGui::Text("SSE"); }
		if (caps[7] == true) { ImGui::Text("SSE2"); }
		if (caps[8] == true) { ImGui::Text("SSE3"); }
		if (caps[9] == true) { ImGui::Text("SSE41"); }
		if (caps[10] == true) { ImGui::Text("SSE42"); }
		
	}
	ImGui::End();
	//Testeo
	//AddFPS(0.16);
}

bool ModuleEditor::CleanUp()
{
	mFPSLog.clear();
	return true;
}

void ModuleEditor::AddFPS(const float aFPS)
{
	mFPSLog.push_back(aFPS);
	ImGui::PlotHistogram("FPS", mFPSLog.data(), mFPSLog.size(),0,NULL,0.0f,1.0f, ImVec2(0, 80.0f)); //Name,iterador,tama�o vector, ??,???,???,???, tama�o espacio azul
	
	if (mFPSLog.size()>=25)
	{
		//Hacer copia del vector inversa
		mFPSLog.clear();
	}



}
