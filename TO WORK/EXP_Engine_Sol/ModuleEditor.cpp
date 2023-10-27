#include "Application.h"
#include "ModuleEditor.h"
#include "imGui/imgui.h"
#include "imGui/backends/imgui_impl_opengl3.h"
#include "imGui/backends/imgui_impl_SDL2.h"
#include "Globals.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "aasimp.h"

#include "GameObject.h"
#include "Component.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleEditor::~ModuleEditor()
{
}

//extern std::string stringTest; //NO PUEDE SER ENCONTRADO

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

	showFPS = false;
	showConfig = false;

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
			if (ImGui::Button("Configuration"))
			{
				showConfig = true;
			}
			
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Assets"))
		{
			if(ImGui::Button("Load BakerHouse"))
			{
				aasimp::Load("Assets/3DObjects/baker_house/BakerHouse.fbx");
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("GameObject"))
		{
			if(ImGui::Checkbox("Show GameObject tree", &showInspector))
			{
				//Activate
			}
			
			if (ImGui::Button("CreateGameObject"))
			{
				App->scene->CreateGameObject(App->scene->root);
			}
			if (ImGui::Button("CreateChildGameObject"))
			{
				if (App->scene->root->children.size()>0)
				{
					App->scene->CreateGameObject(App->scene->selectedGO);
				}
				
			}
			
			

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Show"))
		{
			//Al pulsar el boton y solo entonces se abre el menu que muestra los FPS
			if (ImGui::Button("ShowFPS"))
			{
				showFPS = true;
			}
			if (showFPS)
			{
				//ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver); //Try
				ImGui::Begin("FPS", &showFPS);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
				{	//Testeo
					//AddFPS((float)(rand() % 100) / 100);
					AddFPS(App->GetDeltaTime() * 1000); //??  No se si esto esta bien o mal
					ImGui::Text("Delta Time");
					if (ImGui::Button("Close"))
						showFPS = false;
					ImGui::End();
				}
			}
			if (ImGui::Button("ShowConsole"))
			{
				showConsole = true;
			}
			


			// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
			if (ImGui::Button("Show ImGui:Demo"))
			{
				showDemo = !showDemo;
			}			

		ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::BeginMenu("About"))
			{
				//Quzas para no ocupar tanto espacio aqui añado un about.h
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

	if (showDemo) { ImGui::ShowDemoWindow(); }
	if (showConfig) { Configuration(); }
	if (showInspector) 
	{ 
		//GameObjectsTree
		ImGui::Begin("Hierarchy", &showInspector);
		GameObjectHierarchy(App->scene->root); 
		ImGui::End();

		//GameObject Inspector
		if(App->scene->selectedGO != App->scene->root && App->scene->selectedGO != nullptr)
		{
			ImGui::Begin("Inspector", &showInspector);
			Inspector(App->scene->selectedGO);
			ImGui::End();
		}
		
	}
	if (showConsole)
	{
		//ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver); //Try
		ImGui::Begin("Console", &showConsole);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked				

		ConsoleLog(consoleLog);

		ImGui::End();
	}

	
}

bool ModuleEditor::CleanUp()
{
	mFPSLog.clear();
	return true;
}

void ModuleEditor::Configuration()
{
	if (showConfig)
	{
		ImGui::Begin("Config", &showConfig);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

		if (ImGui::CollapsingHeader("Aplication"))
		{
			int FPS = App->fixedFPS;
			if (ImGui::SliderInt("FPS", &FPS, 1, 120))
			{
				App->fixedFPS = FPS;
			}
		}

		//WINDOW
		if (ImGui::CollapsingHeader("Window"))
		{
			//Sliders scale windows
			float winScale = App->window->GetScreenSize();
			if (ImGui::SliderFloat("Window Scale", &winScale, 0.1f, 1.0f))
			{
				App->window->SetScreenSize(winScale);
				//App->window->Init(); ESTO NO FUNCIONA, CREA UNA NUEVA VENTANA
				App->window->ResizeWindow();
			}
			int winW = App->window->GetScreenWidth();
			if (ImGui::SliderInt("Window Width", &winW, 64, 5000))
			{
				App->window->SetScreenWidth(winW);
				/*App->window->CleanUp(); COMBINADO ES AUN PEOR, HACE QUE PETE TODO
				App->window->Init();*/
				App->window->ResizeWindow();
			}
			int winH = App->window->GetScreenHeigth();
			if (ImGui::SliderInt("Window Heigth", &winH, 64, 5000))
			{
				App->window->SetScreenHeigth(winH);
				//App->window->Init();
				App->window->ResizeWindow();
			}

			//CheckBoxes of screen flags
			bool VSYNCactive = App->window->GetVSYNC();
			if (ImGui::Checkbox("VSYNC", &VSYNCactive))
			{
				App->window->SetVSYNC(VSYNCactive);
			}
			bool borderlessActive = App->window->GetWinBorderless();
			if (ImGui::Checkbox("Borderless", &borderlessActive))
			{
				App->window->SetWinBorderless(borderlessActive);
				App->window->ResizeWindow();
			}
			bool fullscreenActive = App->window->GetWinFullscreen();
			if (ImGui::Checkbox("Fullscren", &fullscreenActive))
			{
				App->window->SetWinFullscreen(fullscreenActive);
				App->window->ResizeWindow();
			}
			bool fullscreenDesktopActive = App->window->GetWinFullscreenDesktop();
			if (ImGui::Checkbox("Desktop Fullscren", &fullscreenDesktopActive))
			{
				App->window->SetWinFullscreenDesktop(fullscreenDesktopActive);
				App->window->ResizeWindow();
			}
			bool resizableActive = App->window->GetWinResizable();
			if (ImGui::Checkbox("Resizable", &resizableActive))
			{
				App->window->SetWinResizable(resizableActive);
				App->window->ResizeWindow();
			}
		}
		//INPUT
		if (ImGui::CollapsingHeader("Input"))
		{
			ImGuiIO& io = ImGui::GetIO();

			if (ImGui::IsMousePosValid())
				ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
			else
				ImGui::Text("Mouse pos: <INVALID>");
			ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
			ImGui::Text("Mouse down:");
			for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDown(i)) { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
			ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);
		}
		//RENDER
		if (ImGui::CollapsingHeader("Render"))
		{
			//Draw OpenGL wireframe mode
			bool drawWireframeMode = App->renderer3D->GetDrawingMode();
			if (ImGui::Checkbox("Wireframe Mode", &drawWireframeMode))
			{
				App->renderer3D->SetDrawingMode(drawWireframeMode);
			}


			//TODO Eric: Añadir botones to directly enable / disable 	 GL_DEPTH_TEST, GL_CULL_FACE, GL_LIGHTING	 GL_COLOR_MATERIAL, GL_TEXTURE_2D + two other

		}
		if (ImGui::CollapsingHeader("Hardware"))
		{
			ImGui::Text("CPUs: %i (Cache: %ikb)", cpus, cpuCache);
			ImGui::Text("System RAM: %iMb", ram);
			ImGui::Text("Caps: ");
			ImGui::SameLine();
			if (caps[0] == true) { ImGui::Text("3D"); ImGui::SameLine(); }
			if (caps[1] == true) { ImGui::Text("AltiVec"); ImGui::SameLine(); }
			if (caps[2] == true) { ImGui::Text("AVX"); ImGui::SameLine(); }
			if (caps[3] == true) { ImGui::Text("AVX2"); ImGui::SameLine(); }
			if (caps[4] == true) { ImGui::Text("MMX"); ImGui::SameLine(); }
			if (caps[5] == true) { ImGui::Text("RDTSC"); }
			if (caps[6] == true) { ImGui::Text("SSE"); ImGui::SameLine(); }
			if (caps[7] == true) { ImGui::Text("SSE2"); ImGui::SameLine(); }
			if (caps[8] == true) { ImGui::Text("SSE3"); ImGui::SameLine(); }
			if (caps[9] == true) { ImGui::Text("SSE41"); ImGui::SameLine(); }
			if (caps[10] == true) { ImGui::Text("SSE42"); }

			ImGui::Text("GPU: %s", glGetString(GL_VENDOR));
		}
		ImGui::End();
	}
}

void ModuleEditor::AddFPS(const float aFPS)
{
	mFPSLog.push_back(aFPS);
	ImGui::PlotHistogram("FPS", mFPSLog.data(), 25, 0, NULL, 0.0f, 1.0f, ImVec2(0, 80.0f)); //Name,iterador,tamaño vector, ??,???,???,???, tamaño espacio azul

	if (mFPSLog.size() >= 25)
	{
		//Hacer copia del vector inversa
		std::vector<float> tempVec;
		for (int i = mFPSLog.size() - 1; i > 1; i--)
		{
			tempVec.push_back(mFPSLog.at(i)); //We add in the temporal vector all the values except the first one
		}
		mFPSLog.clear(); //With all the relevant values copied we clear the vector
		for (auto it = tempVec.cend(); it != tempVec.end(); ++it)
		{
			mFPSLog.push_back(*it);//We add the values on the vector
		}
		//Destroy temporalVector
		tempVec.clear();

	}
}

void ModuleEditor::ConsoleLog(const std::string& str)
{
	ImGui::TextUnformatted(consoleLog.data());

	//Habria que añadir un boton para limpiar el log
}

void ModuleEditor::GameObjectHierarchy(GameObject* go)
{
	//The first node passed (go) should not be printed becose it will be the root
	GameObject* ctp = nullptr; //Childs to print

	int treeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	int leafFlags = treeFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

	int num_children = go->children.size();
	
	
	
	for (int i = 0; i < num_children; ++i)
	{
		ctp = go->children[i];
		if(ctp == App->scene->selectedGO)
		{
			treeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_Selected;
			//LOG("Selected Game Object")
		}

		if (ctp->children.size() > 0)
		{
			bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, treeFlags, ctp->name.c_str());

			if (ImGui::IsItemClicked())
			{
				if (App->scene->selectedGO == ctp)
				{
					App->scene->selectedGO = App->scene->root; //No estoy seguro si se ha de poner nullptr o siempre ha de apuntar a algo
				}
				else
				{
					App->scene->selectedGO = ctp;
				}
				
			}
				

			if (node_open)
			{
				GameObjectHierarchy(ctp); //If the game object has a child we call it recursive to print its childs
				ImGui::TreePop();
			}
		}
		else
		{
			ImGui::TreeNodeEx((void*)(intptr_t)i, leafFlags, ctp->name.c_str()); // Leafs are game objects without childs
			
			if (ImGui::IsItemClicked())
			{
				if (App->scene->selectedGO == ctp)
				{
					App->scene->selectedGO = App->scene->root; //No estoy seguro si se ha de poner nullptr o siempre ha de apuntar a algo
				}
				else
				{
					App->scene->selectedGO = ctp;
				}

			}
		}
	}
	
}

void ModuleEditor::Inspector(GameObject* go)
{
	for (int i = 0; i < go->components.size(); i++) 
	{ 
		//Print all components
		go->components.at(i)->OnEditor();
	}
}
