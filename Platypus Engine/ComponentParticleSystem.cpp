#include "Component.h"
#include "ComponentParticleSystem.h"
#include "ComponentMesh.h" //Necesary for AABBs
#include "GameObject.h"

#include "MathGeoLib/include/Math/Quat.h"

#include "imGui/imgui.h"
#include "imGui/backends/imgui_impl_opengl3.h"
#include "imGui/backends/imgui_impl_SDL2.h"

#include "EmitterInstance.h"

ComponentParticleSystem::ComponentParticleSystem()
{

}

ComponentParticleSystem::ComponentParticleSystem(GameObject* own)
{
	owner = own;
	UUID = App->resources->GenerateNewUID();
}


ComponentParticleSystem::~ComponentParticleSystem()
{
}

ParticleEmitter* ComponentParticleSystem::CreateEmitter()
{
	//Creamos un nuevo emisor
	ParticleEmitter* emisor = new ParticleEmitter;
	allEmitters.push_back(emisor);
	emisor->Init(this);

	return emisor;
}

void ComponentParticleSystem::OnEditor()
{
	if(ImGui::CollapsingHeader("ParticleSystem"))
	{
		ImGui::Checkbox("##ParticleSystem", &active);
		
		int treeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		int leafFlags = treeFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

		//Crear emitter
		if (ImGui::TreeNodeEx((void*)(intptr_t)-1, treeFlags, "Emitters"))
		{
			for (int i = 0; i < allEmitters.size(); i++)
			{
				std::string nameEmitter;
				//ImGui::Text("ParticleEmmiter %i", i);
				nameEmitter.append("Particle Emitter ");
				nameEmitter.append(std::to_string(i+1));

				if (ImGui::TreeNodeEx((void*)(intptr_t)i, (!allEmitters.at(i)->modules.empty()) ? treeFlags : leafFlags, nameEmitter.c_str())) //nameEmitter.c_str()
				{
					auto listModule = allEmitters.at(i)->modules;
					for (int j = 0; j < listModule.size(); j++)
					{
						std::string particleModule; //Les opciones
						switch (listModule.at(j)->type)
						{
						case SPAWN:
							ImGui::Text(particleModule.append("Spawn ##").append(std::to_string(j)).c_str());
							break;
						case MAX:
							break;
						default:
							break;
						}
					}
					if (!allEmitters.at(i)->modules.empty())
					{
						ImGui::TreePop();
					}
				}
				if (ImGui::CollapsingHeader("Create Emitter"))
				{
					for (int k = 0; k < EmiterType::MAX; k++)
					{
						std::string emitterType;

						switch (k)
						{
						case SPAWN:
							emitterType.assign("Spawn Emitter");

							break;
						case MAX:
							break;
						default:
							break;
						}
						if (ImGui::Button(emitterType.c_str()))
						{
							allEmitters.at(i)->CreateEmitterByType((EmiterType)k);
						}

					}
					//ImGui::End();
					//ImGui::TreePop();
				}
				
			}
			if(ImGui::Button("Create Particle Emitter"))
			{
				CreateEmitter();
			}

			ImGui::TreePop();
		}
	}
}




