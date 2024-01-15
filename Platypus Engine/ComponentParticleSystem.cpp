#include "Component.h"
#include "ComponentParticleSystem.h"
#include "ComponentMesh.h" //Necesary for AABBs
#include "GameObject.h"

#include "MathGeoLib/include/Math/Quat.h"

#include "imGui/imgui.h"
#include "imGui/backends/imgui_impl_opengl3.h"
#include "imGui/backends/imgui_impl_SDL2.h"

#include "EmitterInstance.h"
#include "ParticleEmitter.h"

ComponentParticleSystem::ComponentParticleSystem()
{
	active = true;
	Enable();
}

ComponentParticleSystem::ComponentParticleSystem(GameObject* own)
{
	owner = own;
	UUID = App->resources->GenerateNewUID();
	active = true;
	Enable();
}


ComponentParticleSystem::~ComponentParticleSystem()
{
	for(auto it = allEmitters.rbegin(); it != allEmitters.rend(); ++it)
	{
		delete (*it);
		(*it) = nullptr;
	}
}

bool ComponentParticleSystem::Update()
{
	return Update(App->dt);
}

bool ComponentParticleSystem::Update(float dt)
{
	bool ret = true;
	for (unsigned int i = 0; i < allEmitters.size(); ++i)
	{
		allEmitters.at(i)->Update(dt);
	}

	return ret;
}

bool ComponentParticleSystem::GetActive()
{
	return active;
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
	std::string butonChar;

	//Give an ID to each colapsing header to be able to have more than one of the same time
	//This must be done due to ImGui using the names as the ids of all menus and things
	int myPosInComponents = owner->GetComponentPosition(this);
	std::string idComponent;
	idComponent.append("##");
	idComponent.append(std::to_string(myPosInComponents).c_str());

	butonChar.append(butonChar.append(idComponent).c_str());
	if(ImGui::CollapsingHeader("ParticleSystem"))
	{
		butonChar.clear();
		butonChar.append("##Particle System Active");
		if (ImGui::Checkbox(butonChar.append(idComponent).c_str(), &this->active)) //El doble ## hace que no se muestre el texto. Es necesario poner un nombre distinto a cada checkbox y boton ya que ImGui usa el nombre como la ID
		{
			if(active)
			{
				Enable();
			}
		} 
		ImGui::SameLine();

		butonChar.clear();
		butonChar.append("Delete component");
		if (ImGui::Button(butonChar.append(idComponent).c_str()))
		{
			this->~ComponentParticleSystem();
		}
		
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

						//BASE
						
						//SPAWN
						//POSITION
						switch (listModule.at(j)->type)
						{
						case BASE:
						{
							ImGui::Text(particleModule.append("Base ##").append(std::to_string(j)).c_str());

							//Positions
							EmitterBase* eBase = (EmitterBase*)listModule.at(j);
							ImGui::DragFloat3("Initial Pos.", &(eBase->emitterOrigin[0]), 0.1f);
							ImGui::DragFloat("Life Time", &(eBase->particlesLifeTime), 0.5F, 1.0F, 720.0F);
							break;
						}
						case SPAWN:
						{	ImGui::Text(particleModule.append("Spawn ##").append(std::to_string(j)).c_str());

							int numParticles;
							std::string numParticlesWithID = "Particles ##";
						
							EmitterSpawner* eSpawner = (EmitterSpawner*)listModule.at(j);
							numParticles = eSpawner->numParticlesToSpawn;
							//Numero particulas que libera el 
							
							ImGui::Checkbox("Time Based Spawn", &(eSpawner->basedTimeSpawn));
							if (eSpawner->basedTimeSpawn)
							{
								float ratio = eSpawner->spawnRatio - 1.0f;
								if (ImGui::SliderFloat("Delay", &ratio, 0.0f, 11.0f))
								{
									eSpawner->spawnRatio = ratio + 1.0f;
								}
							}
							else
							{
								if (ImGui::SliderInt(numParticlesWithID.append(std::to_string(i)).c_str(), &numParticles, 0, MAXPARTICLES))
								{
									eSpawner->numParticlesToSpawn = numParticles;
								}
							}
							

							break;
						}
						case POSITION:
						{
							EmitterPosition* ePosition = (EmitterPosition*)listModule.at(j);

							ImGui::Text(particleModule.append("Position ##").append(std::to_string(j)).c_str());
							ImGui::Checkbox("Random Movement", &ePosition->randomized);
							if (ePosition->randomized)
							{
								ImGui::DragFloat3("Range 1", &(ePosition->direction1[0]), 0.1f);
								ImGui::DragFloat3("Range 2", &(ePosition->direction2[0]), 0.1f);
							}
							else
							{
								ImGui::DragFloat3("Position", &(ePosition->direction1[0]), 0.1f);
							}
							ImGui::DragFloat("Speed", &(ePosition->particleSpeed),0.2F);

							break;
						}	
						case MAX:
							//Color 
							//ImGui::ColorEdit4;
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
				std::string CEid;
				if (ImGui::CollapsingHeader(CEid.append("Create Emitter").append(std::to_string(i)).c_str()))
				{
					for (int k = 0; k < EmiterType::MAX; k++)
					{
						std::string emitterType;

						switch (k)
						{
						case BASE:
							emitterType.assign("Base Emitter");
							break;
						case SPAWN:
							emitterType.assign("Spawn Emitter");
							break;
						case POSITION:
							emitterType.assign("Position Emitter");
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




