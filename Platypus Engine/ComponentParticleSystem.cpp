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

//ParticleEmitter ComponentParticleSystem::CreateEmitter()
//{
//	//allEmitters.push_back();
//
//	return ParticleEmitter();
//}

void ComponentParticleSystem::OnEditor()
{
	if(ImGui::CollapsingHeader("ParticleSystem"))
	{

		ImGui::Checkbox("##ParticleSystem", &active);
		
		//Crear emitter
		/*if (ImGui::BeginTabBar("ParticlesEmitters"))
		{*/
		ImGui::SameLine;
			if (ImGui::CollapsingHeader("Emitters"))
			{
				for (int i = 0; i < allEmitters.size(); i++)
				{
					std::string nameEmitter;
					//ImGui::Text("ParticleEmmiter %i", i);
					nameEmitter.append("Emitter ");
					nameEmitter.append(std::to_string(i+1));

					if (ImGui::CollapsingHeader(nameEmitter.c_str()))
					{
						auto listModule = allEmitters.at(i).modules;
						for(int i = 0; i < listModule.size(); i++)
						{
							switch (listModule.at(i)->type)
							{
							case SPAWN:
								ImGui::Text("Spawn");
								break;
							case MAX:
								break;
							default:
								break;
							}
						}
							
					}
					ImGui::End;
				}
				ImGui::End;
			}
			//ImGui::EndTabBar();
		/*}*/
		//ImGui::EndTabItem();
	}
}




