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
					int securityCheckTree = 999;
					for (int j = 0; j < listModule.size(); j++)
					{
						
						std::string particleModule; //Les opciones
						std::string deleteButton; //Lo creamos aqui fuera para evitar petadas, pero como la ID va por nombre ha de ser un string para diferenciarlos
						switch (listModule.at(j)->type)
						{
						case BASE:
						{
							ImGui::Text(particleModule.append("Base ##").append(std::to_string(j)).c_str());
							ImGui::SameLine();
							deleteButton.append("Delete ##").append(std::to_string(j));
							if(ImGui::SmallButton(deleteButton.c_str()))
							{
								securityCheckTree = allEmitters.at(i)->DestroyEmitter(j);
							}
							deleteButton.clear();

							//Positions
							EmitterBase* eBase = (EmitterBase*)listModule.at(j);
							ImGui::DragFloat3("Initial Pos. ## BASE", &(eBase->emitterOrigin[0]), 0.1f);
							ImGui::DragFloat("Life Time ## BASE", &(eBase->particlesLifeTime), 0.5F, 1.0F, 720.0F);
							break;
						}
						case SPAWN:
						{	ImGui::Text(particleModule.append("Spawn ##").append(std::to_string(j)).c_str());
							ImGui::SameLine();
							deleteButton.append("Delete ##").append(std::to_string(j));
							if (ImGui::SmallButton(deleteButton.c_str()))
							{
								securityCheckTree = allEmitters.at(i)->DestroyEmitter(j);
							}
							deleteButton.clear();

							int numParticles;
							std::string numParticlesWithID = "Particles ##";
						
							EmitterSpawner* eSpawner = (EmitterSpawner*)listModule.at(j);
							numParticles = eSpawner->numParticlesToSpawn;
							//Numero particulas que libera el 
							
							ImGui::Checkbox("(Time / Num) Spawn ", &(eSpawner->basedTimeSpawn));
							if (eSpawner->basedTimeSpawn)
							{

								if (ImGui::SliderFloat("Delay ##SPAWN", &(eSpawner->spawnRatio), 0.1f, 1.0f))
								{

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
							ImGui::SameLine();
							deleteButton.append("Delete ##").append(std::to_string(j));
							if (ImGui::SmallButton(deleteButton.c_str()))
							{
								securityCheckTree = allEmitters.at(i)->DestroyEmitter(j);
							}
							deleteButton.clear();
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
							ImGui::DragFloat("Speed ##POSITION", &(ePosition->particleSpeed),0.2F);

							break;
						}
						case ROTATION:
						{
							ImGui::Text(particleModule.append("Rotation ##").append(std::to_string(j)).c_str());
							ImGui::SameLine();
							deleteButton.append("Delete ##").append(std::to_string(j));
							if (ImGui::SmallButton(deleteButton.c_str()))
							{
								securityCheckTree = allEmitters.at(i)->DestroyEmitter(j);
							}
							deleteButton.clear();
							break;
						}
						case SIZEPARTICLE:
						{
							EmitterSize* eSize = (EmitterSize*)listModule.at(j);

							ImGui::Text(particleModule.append("Scale ##").append(std::to_string(j)).c_str());
							ImGui::SameLine();
							deleteButton.append("Delete ##").append(std::to_string(j));
							if (ImGui::SmallButton(deleteButton.c_str()))
							{
								securityCheckTree = allEmitters.at(i)->DestroyEmitter(j);
							}
							deleteButton.clear();
							
							ImGui::Checkbox("Progresive Scaling", &(eSize->progresive));
							ImGui::SliderFloat("First Scale", &(eSize->sizeMultiplier1), 0.1f, 10.0f);
							if (eSize->progresive)
							{
								ImGui::SliderFloat("End Scale", &(eSize->sizeMultiplier2), 0.1f, 10.0f);
								ImGui::SliderFloat("Start Change ##SCALE", &(eSize->startChange), 0.0f, (eSize->stopChange - 0.05f));
								ImGui::SliderFloat("Stop Change ##SCALE", &(eSize->stopChange), eSize->startChange + 0.05f, 1.0f);
							}
							break;
						}
						case COLOR:
						{
							EmitterColor* eColor = (EmitterColor*)listModule.at(j);

							ImGui::Text(particleModule.append("Color ##").append(std::to_string(j)).c_str());
							ImGui::SameLine();
							deleteButton.append("Delete ##").append(std::to_string(j));
							if (ImGui::SmallButton(deleteButton.c_str()))
							{
								securityCheckTree = allEmitters.at(i)->DestroyEmitter(j);
							}
							deleteButton.clear();

							ImGui::Checkbox("Progresive Color", &(eColor->progresive));
							ImGui::ColorEdit4("First Color", &(eColor->color1));
							if(eColor->progresive)
							{
								if(ImGui::ColorEdit4("End Color", &(eColor->color2)));
								{
									eColor->color2 = eColor->color2;
								}
								ImGui::SliderFloat("Start ReSize ##COLOR", &(eColor->startChange), 0.0f, (eColor->stopChange-0.05f));
								ImGui::SliderFloat("Stop ReSize ##COLOR", &(eColor->stopChange), eColor->startChange+0.05f, 1.0f);
							}
							
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
					if (!allEmitters.at(i)->modules.empty() || securityCheckTree == 0) //Necesario para que no pete el tree pop
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
						case ROTATION:
							emitterType.assign("Rotation Emitter");
							break;
						case SIZEPARTICLE:
							emitterType.assign("Scale Emitter");
							break;
						case COLOR:
							emitterType.assign("Color Emitter");
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




