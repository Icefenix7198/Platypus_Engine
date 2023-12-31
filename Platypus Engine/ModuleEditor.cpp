#include "Application.h"
#include "ModuleEditor.h"
#include "imGui/imgui.h"
#include "imGui/backends/imgui_impl_opengl3.h"
#include "imGui/backends/imgui_impl_SDL2.h"
#include "Globals.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "mmgr/mmgr.h"

#include "aasimp.h"
#include "Importer.h"
#include "ImporterFBX.h"

#include "GameObject.h"
#include "Component.h"

#include "Importer.h" //Lo pongo aqui porque solo es un text temporal
#include "JSONHelper.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleEditor::~ModuleEditor()
{
}

//extern std::string stringTest; //NO PUEDE SER ENCONTRADO

bool ModuleEditor::Init()
{
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

	SDL_GetVersion(&SDLversion);

	return true;
}

void ModuleEditor::DrawEditor()
{
	main_viewport = ImGui::GetMainViewport(); //Make generic windows (it must be declared here cause imGui/Window isn't created at Init

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
				showConfig = !showConfig;
			}
			
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Assets"))
		{
			Importer importer;
			std::string path;
			path.assign(ASSETS_MODELS);
			if(ImGui::Button("Load BakerHouse"))
			{
				//"C:\Users\ericsb\Documents\GitHub\Platypus_Engine\Platypus Engine\Game\Assets\MODELS\BakerHouse.fbx";
				path.append("BakerHouse.fbx");
				importer.Import(path.c_str());
				
			}
			
			if(ImGui::BeginMenu("Geometric Shapes"))
			{
				if (ImGui::Button("Create Cube"))
				{
					path.append("cube.fbx");
					importer.Import(path.c_str());
				}
				if (ImGui::Button("Create Sphere"))
				{
					path.append("sphere.fbx");
					importer.Import(path.c_str());
				}
				if (ImGui::Button("Create Cylinder"))
				{
					path.append("cylinder.fbx");
					importer.Import(path.c_str());
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Own FBX"))
			{
				if (ImGui::Button("Mike the mouse"))
				{
					path.append("mickey-mouse.FBX");
					importer.Import(path.c_str());
				}
				if (ImGui::Button("Create AgentP"))
				{
					path.append("perry.FBX");
					importer.Import(path.c_str());
				}
				ImGui::EndMenu();
			}
			
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("GameObject"))
		{
			if(ImGui::Checkbox("Show GameObject tree", &showInspector))
			{
				//Activate
			}
			
			if (ImGui::Button("CreateEmptyGameObject"))
			{
				App->scene->CreateGameObject(App->scene->root);
			}
			if (ImGui::Button("CreateChildEmptyGameObject"))
			{
				if (App->scene->root->children.size()>0)
				{
					App->scene->CreateGameObject(App->scene->selectedGO);
				}
				
			}
			if (ImGui::Button("Save Scene"))
			{
				App->scene->CreateSerializationGameObject(App->scene->root);
			}
			if (ImGui::Button("Load Scene"))
			{
				App->scene->InitCreateGOFromSerialization();
			}
			
			

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::Button("Console"))
			{
				showConsole = !showConsole;
			}
			
			if (ImGui::Button("Show ImGui:Demo"))
			{
				showDemo = !showDemo;
			}

			if (ImGui::Button("Inspector"))
			{
				showInspector = !showInspector;
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
							"- ImGui v1.89.9 \n- SDL2 v2.0.4 \n- MathGeoLib v2.04 \n- OpenGL v%s \n- Glew \n- mmgr \n- parson v1.5.2 \n- Assimp v3.1.1 \n- DevIL v2.1\n"
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
							"WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.", glGetString(GL_VERSION));
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("GitHub Page"))
			{
				ShellExecute(NULL, "open", "https://github.com/Icefenix7198/Platypus_Engine", 0, 0, SW_SHOWNORMAL);
			}
			ImGui::EndMenu();
		}
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.8,0.1,0.1,1 }); //ImGuiCol is the type of widget, the color go from 0 to 1, usable for AAW
		if (ImGui::Button("Quit"))
		{
			quit = true;
		}
		ImGui::PopStyleColor();

		ImGui::EndMainMenuBar();
	}

	if (showDemo) { ImGui::ShowDemoWindow(); }
	if (showConfig) { Configuration(); }
	if (showInspector) 
	{ 
		//Set size and position of inspector	
		ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 0, main_viewport->WorkPos.y + 18), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(220, 772), ImGuiCond_Once);

		//GameObjectsTree
		ImGui::Begin("Hierarchy", &showInspector);
		GameObjectHierarchy(App->scene->root); 
		ImGui::End();

		//GameObject Inspector
		if(App->scene->selectedGO != App->scene->root && App->scene->selectedGO != nullptr)
		{
			//Set size and position of inspector
			ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + (main_viewport->Size.x- 270), main_viewport->WorkPos.y + 18), ImGuiCond_Once); 
			ImGui::SetNextWindowSize(ImVec2(270, 560), ImGuiCond_Once);

			ImGui::Begin("Inspector", &showInspector);
			Inspector(App->scene->selectedGO);
			ImGui::End();
		}
		
	}
	if (showConsole)
	{
		if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Console"))
			{
				ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 220, main_viewport->WorkPos.y + (main_viewport->Size.y - 220)), ImGuiCond_Once);
				ImGui::SetNextWindowSize(ImVec2(800, 220), ImGuiCond_Once);

				ConsoleLog(consoleLog);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Assets"))
			{
				AssetsMenu();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}
	
}

bool ModuleEditor::CleanUp()
{
	mDTLog.clear();
	return true;
}

void ModuleEditor::Configuration()
{
	if (showConfig)
	{
		ImGui::Begin("Config", &showConfig);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

		if (ImGui::CollapsingHeader("Aplication"))
		{
			AddDeltaTime(App->GetDeltaTime() * 1000);
			ImGui::Text("Delta Time");
			AddFPS(App->fixedFPS);
			ImGui::Text("FPS");

			float FPS = App->fixedFPS;
			if (ImGui::SliderFloat("FPS", &FPS, 1.0f, 120.0f,"%.0f"))
			{
				App->fixedFPS = FPS;
			}
		}

		//WINDOW
		if (ImGui::CollapsingHeader("Window"))
		{
			float vBrigtness = SDL_GetWindowBrightness(App->window->window);
			if (ImGui::SliderFloat("Window Brigtness", &vBrigtness, 0.1f, 1.0f))
			{
				SDL_SetWindowBrightness(App->window->window, vBrigtness);
			}

			//Sliders scale windows
			float winScale = App->window->GetScreenSize();
			if (ImGui::SliderFloat("Window Scale", &winScale, 0.1f, 1.0f))
			{
				App->window->SetScreenSize(winScale);
				App->window->ResizeWindow();
			}
			int winW = App->window->GetScreenWidth();
			if (ImGui::SliderInt("Window Width", &winW, 64, 5000))
			{
				App->window->SetScreenWidth(winW);
				App->window->ResizeWindow();
			}
			int winH = App->window->GetScreenHeigth();
			if (ImGui::SliderInt("Window Heigth", &winH, 64, 5000))
			{
				App->window->SetScreenHeigth(winH);
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

			bool drawDepthTest = App->renderer3D->GetDepthTest();
			if (ImGui::Checkbox("Depth Test", &drawDepthTest))
			{
				if (drawDepthTest)
				{
					glEnable(GL_DEPTH_TEST);
				}
				else
				{
					glDisable(GL_DEPTH_TEST);
				}
				
				App->renderer3D->SetDepthTest(drawDepthTest);
			}
			

			bool drawCullFace = App->renderer3D->GetCullFace();
			if (ImGui::Checkbox("Face Culling", &drawCullFace))
			{
				if (drawCullFace)
				{
					glEnable(GL_CULL_FACE);
				}
				else
				{
					glDisable(GL_CULL_FACE);
				}

				App->renderer3D->SetCullFace(drawCullFace);
			}

			bool drawlLigthning = App->renderer3D->GetLigthning();
			if (ImGui::Checkbox("Ligthning", &drawlLigthning))
			{
				if (drawlLigthning)
				{
					glEnable(GL_LIGHTING);
				}
				else
				{
					glDisable(GL_LIGHTING);
				}

				App->renderer3D->SetLigthning(drawlLigthning);
			}


			bool drawColorMaterial = App->renderer3D->GetColorMaterial();
			if (ImGui::Checkbox("Draw Color Material", &drawColorMaterial))
			{
				if (drawColorMaterial)
				{
					glEnable(GL_COLOR_MATERIAL);
				}
				else
				{
					glDisable(GL_COLOR_MATERIAL);
				}

				App->renderer3D->SetColorMaterial(drawColorMaterial);
			}

			bool drawTexture2D = App->renderer3D->GetTexture2D();
			if (ImGui::Checkbox("Draw Texture 2D", &drawTexture2D))
			{
				if (drawTexture2D)
				{
					glEnable(GL_TEXTURE_2D);
				}
				else
				{
					glDisable(GL_TEXTURE_2D);
				}

				App->renderer3D->SetTexture2D(drawTexture2D);
			}

			

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

			sMStats memoryStats = m_getMemoryStatistics();

			ImGui::Text("Total Memory Reported: "); ImGui::SameLine(); ImGui::Text("%u bytes", memoryStats.totalReportedMemory);
			ImGui::Text("Total Actual Mememory: "); ImGui::SameLine(); ImGui::Text("%u bytes", memoryStats.totalActualMemory);
			ImGui::Text("Peak Memory Reported: "); ImGui::SameLine(); ImGui::Text("%u bytes", memoryStats.peakReportedMemory);
			ImGui::Text("Peak Actual Memory: "); ImGui::SameLine(); ImGui::Text("%u bytes", memoryStats.peakActualMemory);

		}
		ImGui::End();
	}
}

void ModuleEditor::AddDeltaTime(const float aFPS)
{
	mDTLog.push_back(aFPS);
	ImGui::PlotHistogram("DeltaTime", mDTLog.data(), 25, 0, NULL, 0.0f, 1.0f, ImVec2(0, 120.0f)); //Name,iterador,tamaño vector, ??,???,???,???, tamaño espacio azul

	if (mDTLog.size() >= 60)
	{
		//Hacer copia del vector inversa
		std::vector<float> tempVec;
		for (int i = mDTLog.size() - 1; i > 1; i--)
		{
			tempVec.push_back(mDTLog.at(i)); //We add in the temporal vector all the values except the first one
		}
		mDTLog.clear(); //With all the relevant values copied we clear the vector
		for (auto it = tempVec.cend(); it != tempVec.end(); ++it)
		{
			mDTLog.push_back(*it);//We add the values on the vector
		}
		//Destroy temporalVector
		tempVec.clear();

	}
}

void ModuleEditor::AddFPS(const float aFPS)
{
	mFPSLog.push_back(aFPS);
	ImGui::PlotHistogram("FPS ##HISTOGRAM", mFPSLog.data(), 25, 0, NULL, 0.0f, 1.0f, ImVec2(0, 80.0f)); //Name,iterador,tamaño vector, ??,???,???,???, tamaño espacio azul

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
	if (go == App->scene->root) 
	{
		bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)-1, treeFlags, go->name.c_str());
		
		//Root node only accept dragging into, not being dragged
		if (ImGui::BeginDragDropTarget()) {
			//Payload es el tipo de objeto que arrastramos, como todo en ImGui, va por Tag
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"/*, ImGuiDragDropFlags_SourceNoDisableHover*/);
			if (payload != nullptr && payload->IsDataType("GameObject"))
			{
				GameObject* GameObjFromPayLoad = *(GameObject**)payload->Data;
				if (GameObjFromPayLoad != nullptr)
				{
					App->scene->RequestReparentGameObject(GameObjFromPayLoad, go);
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (node_open)
		{
			for (int i = 0; i < num_children; ++i)
			{
				ctp = go->children[i];
				if (ctp == App->scene->selectedGO)
				{
					treeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_Selected;
				}
				else
				{
					treeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
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

					if (ImGui::BeginDragDropTarget()) {
						//Payload es el tipo de objeto que arrastramos, como todo en ImGui, va por Tag
						const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"/*, ImGuiDragDropFlags_SourceNoDisableHover*/);
						if (payload != nullptr && payload->IsDataType("GameObject"))
						{
							GameObject* GameObjFromPayLoad = *(GameObject**)payload->Data;
							if (GameObjFromPayLoad != nullptr)
							{
								App->scene->RequestReparentGameObject(GameObjFromPayLoad, ctp);
							}
						}
						ImGui::EndDragDropTarget();
					}
					if (ImGui::BeginDragDropSource(/*ImGuiDragDropFlags_SourceNoDisableHover*/)) {
						ImGui::SetDragDropPayload("GameObject", &ctp, sizeof(GameObject)/*, ImGuiCond_Once*/);
						ImGui::Text(ctp->name.c_str());
						ImGui::EndDragDropSource();
					}

					if (node_open)
					{
						GameObjectHierarchy(ctp); //If the game object has a child we call it recursive to print its childs
						ImGui::TreePop();
					}
				}
				else
				{
					if (ctp == App->scene->selectedGO)
					{
						leafFlags = leafFlags | ImGuiTreeNodeFlags_Selected;
					}
					else
					{
						leafFlags = treeFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
					}

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

					if (ImGui::BeginDragDropTarget()) {
						//Payload es el tipo de objeto que arrastramos, como todo en ImGui, va por Tag
						const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"/*, ImGuiDragDropFlags_SourceNoDisableHover*/);
						if (payload != nullptr && payload->IsDataType("GameObject"))
						{
							GameObject* GameObjFromPayLoad = *(GameObject**)payload->Data;
							if (GameObjFromPayLoad != nullptr)
							{
								App->scene->RequestReparentGameObject(GameObjFromPayLoad, ctp);
							}
						}
						ImGui::EndDragDropTarget();
					}
					if (ImGui::BeginDragDropSource(/*ImGuiDragDropFlags_SourceNoDisableHover*/)) {
						ImGui::SetDragDropPayload("GameObject", &ctp, sizeof(GameObject)/*, ImGuiCond_Once*/);
						ImGui::Text(ctp->name.c_str());
						ImGui::EndDragDropSource();
					}
				}
			}
			ImGui::TreePop();
		}

	}
	else
	{
		for (int i = 0; i < num_children; ++i)
		{
			ctp = go->children[i];
			if (ctp == App->scene->selectedGO)
			{
				treeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_Selected;
			}
			else
			{
				treeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
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

				if (ImGui::BeginDragDropTarget()) {
					//Payload es el tipo de objeto que arrastramos, como todo en ImGui, va por Tag
					const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"/*, ImGuiDragDropFlags_SourceNoDisableHover*/);
					if (payload != nullptr && payload->IsDataType("GameObject"))
					{
						GameObject* GameObjFromPayLoad = *(GameObject**)payload->Data;
						if (GameObjFromPayLoad != nullptr)
						{
							App->scene->RequestReparentGameObject(GameObjFromPayLoad, ctp);
						}
					}
					ImGui::EndDragDropTarget();
				}
				if (ImGui::BeginDragDropSource(/*ImGuiDragDropFlags_SourceNoDisableHover*/)) {
					ImGui::SetDragDropPayload("GameObject", &ctp, sizeof(GameObject)/*, ImGuiCond_Once*/);
					ImGui::Text(ctp->name.c_str());
					ImGui::EndDragDropSource();
				}

				if (node_open)
				{
					GameObjectHierarchy(ctp); //If the game object has a child we call it recursive to print its childs
					ImGui::TreePop();
				}
			}
			else
			{
				if (ctp == App->scene->selectedGO)
				{
					leafFlags = leafFlags | ImGuiTreeNodeFlags_Selected;
				}
				else
				{
					leafFlags = treeFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
				}

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

				if (ImGui::BeginDragDropTarget()) {
					//Payload es el tipo de objeto que arrastramos, como todo en ImGui, va por Tag
					const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"/*, ImGuiDragDropFlags_SourceNoDisableHover*/);
					if (payload != nullptr && payload->IsDataType("GameObject"))
					{
						GameObject* GameObjFromPayLoad = *(GameObject**)payload->Data;
						if (GameObjFromPayLoad != nullptr)
						{
							App->scene->RequestReparentGameObject(GameObjFromPayLoad, ctp);
						}
					}
					ImGui::EndDragDropTarget();
				}
				if (ImGui::BeginDragDropSource(/*ImGuiDragDropFlags_SourceNoDisableHover*/)) {
					ImGui::SetDragDropPayload("GameObject", &ctp, sizeof(GameObject)/*, ImGuiCond_Once*/);
					ImGui::Text(ctp->name.c_str());
					ImGui::EndDragDropSource();
				}
			}
		}
	}
}

void ModuleEditor::Inspector(GameObject* go)
{
	if (go != nullptr )
	{
		ImGui::Checkbox("Active ## GameObject", &go->active); //For future problems, 
		//ImGui::SameLine();
		char* nombre = (char*)go->name.c_str();
		if(ImGui::InputText("Name",nombre,40)) 
		{
			go->name.assign(nombre);
		}
		ImGui::SameLine(); 
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.7,0.1,0.1,1 }); //ImGuiCol is the type of widget, the color go from 0 to 1, usable for AAW
		if (ImGui::Button("Delete"))
		{
			App->scene->RequestDeleteGameObject(go);
		}
		ImGui::PopStyleColor();
		ImGui::Text(go->parent->name.c_str());
		for (int i = 0; i < go->components.size(); i++) 
		{ 
			//Print all components
			go->components.at(i)->OnEditor();
		}
		if(ImGui::Button("Add Mesh"))
		{
			go->CreateComponent(ComponentType::MESH);
		}
		if (ImGui::Button("Add Material"))
		{
			go->CreateComponent(ComponentType::MATERIAL);
		}
		if (ImGui::Button("Add Camera"))
		{
			go->CreateComponent(ComponentType::CAMERA);
		}
	}
	
}

void ModuleEditor::AssetsMenu()
{
	if (ImGui::CollapsingHeader("Models"))
	{
		std::vector<std::string> listObjects;
		App->fileSystem->GetAllFilesWithExtension(ASSETS_MODELS, "fbx", listObjects);
		App->fileSystem->GetAllFilesWithExtension(ASSETS_MODELS, "FBX", listObjects);

		for (size_t i = 0; i < listObjects.size(); i++)
		{
			const char* nameM = listObjects.at(i).c_str();
			if (ImGui::Button(nameM))
			{
				
			}
		}
	}

	if (ImGui::CollapsingHeader("Textures"))
	{
		std::vector<std::string> listTextures;
		App->fileSystem->GetAllFilesWithExtension(ASSETS_TEXTURES, "png", listTextures);
		App->fileSystem->GetAllFilesWithExtension(ASSETS_TEXTURES, "PNG", listTextures);

		for (size_t i = 0; i < listTextures.size(); i++)
		{
			const char* nameT = listTextures.at(i).c_str();
			ImGui::Image((ImTextureID)nameT, ImVec2(32, 32));
			ImGui::Button(nameT);
		}
	}
}
