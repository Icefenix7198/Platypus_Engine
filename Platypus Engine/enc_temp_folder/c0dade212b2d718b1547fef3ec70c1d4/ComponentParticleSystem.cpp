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

#include "parson-master/parson.h" //For JSONs

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
	//Si la primera vez que se ejecuta
	return Update(App->editor->GetPlayDT());
}

bool ComponentParticleSystem::Update(float dt)
{
	bool ret = true;
	if (dt>0.0f)
	{
		for (unsigned int i = 0; i < allEmitters.size(); ++i)
		{
			allEmitters.at(i)->Update(dt);
		}
	}
	else
	{
		for (unsigned int i = 0; i < allEmitters.size(); ++i)
		{
			allEmitters.at(i)->Reset();
		}
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
				if (std::strcmp(allEmitters.at(i)->name.c_str(),"") == 0)
				{
					
					//ImGui::Text("ParticleEmmiter %i", i);
					nameEmitter.append("Particle Emitter ");
					nameEmitter.append(std::to_string(i + 1));
				}
				else
				{
					nameEmitter += allEmitters.at(i)->name;
					nameEmitter.append(" ## ");
					nameEmitter.append(std::to_string(i + 1));

				}
				//std::string nameEmitter;
				////ImGui::Text("ParticleEmmiter %i", i);
				//nameEmitter.append("Particle Emitter ");
				//nameEmitter.append(std::to_string(i+1));

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
							ImGui::Checkbox("Random Movement ##POSITION", &ePosition->randomized);
							if (ePosition->randomized)
							{
								ImGui::DragFloat3("Range 1 ##POSITION", &(ePosition->direction1[0]), 0.1f);
								ImGui::DragFloat3("Range 2 ##POSITION", &(ePosition->direction2[0]), 0.1f);
							}
							else
							{
								ImGui::DragFloat3("Position", &(ePosition->direction1[0]), 0.1f);
							}
							ImGui::Checkbox("Acceraltion ##POSITION", &ePosition->acceleration);
							if (ePosition->acceleration)
							{
								ImGui::DragFloat("SpeedInit ##POSITION", &(ePosition->particleSpeed1), 0.2F);
								ImGui::DragFloat("SpeedFinal ##POSITION", &(ePosition->particleSpeed2), 0.2F);
							}
							else
							{
								ImGui::DragFloat("Speed ##POSITION", &(ePosition->particleSpeed1), 0.2F);
							}
							

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
				if (ImGui::CollapsingHeader(CEid.append("Create Emitter ##").append(std::to_string(i)).c_str()))
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
				std::string inputNameID;
				/*char* textInfo = "";
				ImGui::InputText( inputNameID.append("Emitter Name ##").append(std::to_string(i)).c_str(),textInfo, IM_ARRAYSIZE(textInfo));
				{
					allEmitters.at(i)->name.assign(textInfo);
				}*/
				std::string SEid;
				if ( ImGui::Button( SEid.append("Create Emitter ##").append(std::to_string(i)).c_str() ) )
				{
					SaveEmmiterJSON(allEmitters.at(i));
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

const char* ComponentParticleSystem::SaveMetaEmitters()
{
	std::string filePath;

	JSON_Value* root_value = json_value_init_object();
	JSON_Object* root_object = json_value_get_object(root_value);

	//If there is no failure loading
	if (root_value != nullptr && root_object != nullptr)
	{
		
		//Crear path
		char* serialized_string = NULL;

		json_object_set_string(root_object, "FilePath", filePath.c_str());
		json_object_set_number(root_object, "NumEmitters", allEmitters.size());
		json_object_set_number(root_object, "OwnerID", this->GetUUID());

		//Create array of all emitters UUID
		JSON_Array* arr;
		JSON_Value* new_val = json_value_init_array();

		arr = json_value_get_array(new_val);
		json_object_dotset_value(root_object, "Emitters", new_val);
		//We create the value to assign and insert into the array
		JSON_Value* arrayRecord;
		arrayRecord = json_value_init_object();
		//We call save Emitter in Json for each mesh and we save their UUID
		for (int i = 0; i < allEmitters.size(); i++)
		{
			json_array_append_number(arr,SaveEmmiterJSON(allEmitters.at(i)));
		}

		
		serialized_string = json_serialize_to_string_pretty(root_value);
		puts(serialized_string);

		//Crear el archivo en assets
		std::string nameMeta;
		nameMeta += ASSETS_PARTICLES;
		nameMeta += std::to_string(this->GetUUID());
		nameMeta += ".meta";
		json_serialize_to_file(root_value, nameMeta.c_str());
		json_free_serialized_string(serialized_string);
		json_value_free(root_value);

		return nameMeta.c_str();
	}
}

uint32_t ComponentParticleSystem::SaveEmmiterJSON(ParticleEmitter * emitter)
{
	uint32_t docID = App->resources->GenerateNewUID();
	JSON_Value* root_value = json_value_init_object();
	JSON_Object* root_object = json_value_get_object(root_value);

	//If there is no failure loading
	if (root_value != nullptr && root_object != nullptr)
	{

		//Crear path
		char* serialized_string = NULL;

		json_object_set_number(root_object, "ModulesSize", emitter->modules.size());
		json_object_set_number(root_object, "OwnerID", owner->GetUUID());

		//Create array of all modules info
		JSON_Array* arr;
		JSON_Value* new_val = json_value_init_array();

		arr = json_value_get_array(new_val);
		json_object_dotset_value(root_object, "Emitters", new_val);
		//We call save Emitter in Json for each mesh and we save their UUID
		for (int i = 0; i < emitter->modules.size(); i++)
		{
			//Components (son object)
			JSON_Value* modulo;
			modulo = json_value_init_object();
			JSON_Object* child_object = json_value_get_object(modulo);

			json_object_set_number(child_object, "Type", (int)emitter->modules.at(i)->type);

			//Y aqui un switch para crear en funcion del tipo
			switch (emitter->modules.at(i)->type)
			{
			case BASE:
			{
				EmitterBase* eBase = (EmitterBase*)emitter->modules.at(i);

				//Lifetime
				json_object_set_number(child_object, "Lifetime", eBase->particlesLifeTime);
				//InitialPosition
				JSON_Array* arrInitialPos;

				//Position
				JSON_Value* jValuePos = json_value_init_array();
				arrInitialPos = json_value_get_array(jValuePos);

				json_object_dotset_value(child_object, "Position", jValuePos);
				json_array_append_number(arrInitialPos, eBase->emitterOrigin.x);
				json_array_append_number(arrInitialPos, eBase->emitterOrigin.y);
				json_array_append_number(arrInitialPos, eBase->emitterOrigin.z);

				break;
			}
			case SPAWN:
			{
				EmitterSpawner* eSpawn = (EmitterSpawner*)emitter->modules.at(i);

				//Time of num based
				json_object_set_boolean(child_object, "TimeBased", eSpawn->basedTimeSpawn);
				//ParticlesToSpawn
				json_object_set_number(child_object, "NumParticles", eSpawn->numParticlesToSpawn);
				//SpawnSpeed
				json_object_set_number(child_object, "SpawnRatio", eSpawn->spawnRatio);

				break;
			}
			case POSITION:
			{
				EmitterPosition* ePosition = (EmitterPosition*)emitter->modules.at(i);

				//Random
				json_object_set_boolean(child_object, "Random", ePosition->randomized);

				//Directions
				//InitialDirection
				JSON_Array* arrInitialDir1;

				//Position
				JSON_Value* jValueDir1 = json_value_init_array();
				arrInitialDir1 = json_value_get_array(jValueDir1);

				json_object_dotset_value(child_object, "Direction1", jValueDir1);
				json_array_append_number(arrInitialDir1, ePosition->direction1.x);
				json_array_append_number(arrInitialDir1, ePosition->direction1.y);
				json_array_append_number(arrInitialDir1, ePosition->direction1.z);

				ePosition->direction1;
				//SecondDirection
				JSON_Array* arrInitialDir2;

				//Position
				JSON_Value* jValueDir2 = json_value_init_array();
				arrInitialDir2 = json_value_get_array(jValueDir2);

				json_object_dotset_value(child_object, "Direction2", jValueDir2);
				json_array_append_number(arrInitialDir2, ePosition->direction2.x);
				json_array_append_number(arrInitialDir2, ePosition->direction2.y);
				json_array_append_number(arrInitialDir2, ePosition->direction2.z);

				//Accelerate
				json_object_set_boolean(child_object, "Accelerates", ePosition->acceleration);

				//Speed
				json_object_set_number(child_object, "Speed1", ePosition->particleSpeed1);
				json_object_set_number(child_object, "Speed2", ePosition->particleSpeed2);

				break;
			}
			case ROTATION:
			{
				//Doesn't store any value (Don't have different billboarding orientations)

				break;
			}
			case SIZEPARTICLE:
			{	
				EmitterSize* eSize = (EmitterSize*)emitter->modules.at(i);

				//Bool size increases progresivly
				json_object_set_boolean(child_object, "Progressive", eSize->progresive);

				//Scaling Factor
				json_object_set_number(child_object, "Size1", eSize->sizeMultiplier1);
				json_object_set_number(child_object, "Size2", eSize->sizeMultiplier2);

				//Time
				json_object_set_number(child_object, "TimeStart", eSize->startChange);
				json_object_set_number(child_object, "TimeStop", eSize->stopChange);

				break;
			}
			case COLOR:
			{	
				EmitterColor* eColor = (EmitterColor*)emitter->modules.at(i);

				//Bool size increases progresivly
				json_object_set_boolean(child_object, "Progressive", eColor->progresive);

				//Colors
				//InitialColor
				JSON_Array* arrInitialColor;

				//Position
				JSON_Value* jValueColor1 = json_value_init_array();
				arrInitialColor = json_value_get_array(jValueColor1);

				json_object_dotset_value(child_object, "Color1", jValueColor1);
				json_array_append_number(arrInitialColor, eColor->color1.r);
				json_array_append_number(arrInitialColor, eColor->color1.g);
				json_array_append_number(arrInitialColor, eColor->color1.b);
				json_array_append_number(arrInitialColor, eColor->color1.a);

				//InitialColor
				JSON_Array* arrFinalColor;

				//Position
				JSON_Value* jValueColor2 = json_value_init_array();
				arrFinalColor = json_value_get_array(jValueColor2);

				json_object_dotset_value(child_object, "Color2", jValueColor2);
				json_array_append_number(arrFinalColor, eColor->color2.r);
				json_array_append_number(arrFinalColor, eColor->color2.g);
				json_array_append_number(arrFinalColor, eColor->color2.b);
				json_array_append_number(arrFinalColor, eColor->color2.a);

				//Time
				json_object_set_number(child_object, "TimeStart", eColor->startChange);
				json_object_set_number(child_object, "TimeStop", eColor->stopChange);

				break;
			}
			case MAX:
				break;
			default:
				break;
			}

			json_array_append_value(arr, modulo);
		}


		serialized_string = json_serialize_to_string_pretty(root_value);
		puts(serialized_string);

		//Crear el archivo en assets
		std::string nameMeta;
		nameMeta += PARTICLES_PATH;
		nameMeta += std::to_string(docID);
		nameMeta += PAR;
		json_serialize_to_file(root_value, nameMeta.c_str());
		json_free_serialized_string(serialized_string);
		json_value_free(root_value);
	}
	return docID;
}

ParticleEmitter* ComponentParticleSystem::LoadEmitterFromMeta(const char* pathMeta)
{
	ParticleEmitter* pE = new ParticleEmitter;
	pE->owner = this;
	JSON_Value* root_value = json_parse_file(pathMeta);
	JSON_Object* root_object = json_value_get_object(root_value);

	int numEmitters = json_object_get_number(root_object, "ModulesSize");

	JSON_Array* arr = json_object_get_array(root_object, "Emitters");
	for (int i = 0; i < numEmitters; i++)
	{
		JSON_Object* modulo = json_array_get_object(arr, i);
		
		EmiterType type = (EmiterType)json_object_get_number(modulo, "Type");
		
		EmitterInstance* instancia = pE->CreateEmitterByType(type);

		switch (type)
		{
		case BASE:
		{
			EmitterBase* eBase = (EmitterBase*)instancia;
			eBase->particlesLifeTime = (float)json_object_get_number(modulo, "Lifetime");

			//Get position array
			JSON_Array* posArr = json_object_get_array(modulo, "Position");

			//Get elements of position
			float posX = json_array_get_number(posArr, 0);
			float posY = json_array_get_number(posArr, 1);
			float posZ = json_array_get_number(posArr, 2);
			eBase->emitterOrigin = { posX,posY,posZ };

			break;
		}
		case SPAWN:
		{
			EmitterSpawner* eSpawn = (EmitterSpawner*)instancia;

			eSpawn->basedTimeSpawn = json_object_get_boolean(modulo, "TimeBased");
			eSpawn->numParticlesToSpawn = json_object_get_number(modulo, "NumParticles");
			eSpawn->spawnRatio = (float)json_object_get_number(modulo, "SpawnRatio");
			
			break;
		}
		case POSITION:
		{
			EmitterPosition* ePos = (EmitterPosition*)instancia;

			ePos->randomized = json_object_get_boolean(modulo, "Random");
			ePos->particleSpeed1 = json_object_get_number(modulo, "Speed1");
			ePos->particleSpeed2 = json_object_get_number(modulo, "Speed2");
			ePos->acceleration = json_object_get_boolean(modulo, "Accelerates");

			//Get position array
			JSON_Array* dirArr1 = json_object_get_array(modulo, "Direction1");

			//Get elements of position
			float posX1 = json_array_get_number(dirArr1, 0);
			float posY1 = json_array_get_number(dirArr1, 1);
			float posZ1 = json_array_get_number(dirArr1, 2);
			ePos->direction1 = { posX1,posY1,posZ1 };

			//Get position array
			JSON_Array* dirArr2 = json_object_get_array(modulo, "Direction2");

			//Get elements of position
			float posX2 = json_array_get_number(dirArr2, 0);
			float posY2 = json_array_get_number(dirArr2, 1);
			float posZ2 = json_array_get_number(dirArr2, 2);
			ePos->direction2 = { posX2,posY2,posZ2 };

			break;
		}
		case ROTATION:
		{
			break; 
		}
		case SIZEPARTICLE:
		{
			EmitterSize* eSize = (EmitterSize*)instancia;

			eSize->progresive = json_object_get_boolean(modulo, "Progressive");
			eSize->sizeMultiplier1 = (float)json_object_get_number(modulo, "Size1");
			eSize->sizeMultiplier2 = (float)json_object_get_number(modulo, "Size2");
			eSize->startChange = (float)json_object_get_number(modulo, "TimeStart");
			eSize->stopChange = (float)json_object_get_number(modulo, "TimeStop");
			break;
		}
		case COLOR:
		{
			EmitterColor* eColor = (EmitterColor*)instancia;

			eColor->progresive = json_object_get_boolean(modulo, "Progressive");

			eColor->startChange = (float)json_object_get_number(modulo, "TimeStart");
			eColor->stopChange = (float)json_object_get_number(modulo, "TimeStop");

			//Get COLOR array
			JSON_Array* colArr1 = json_object_get_array(modulo, "Color1");

			//Get elements of position
			eColor->color1.r = (float)json_array_get_number(colArr1, 0);
			eColor->color1.g = (float)json_array_get_number(colArr1, 1);
			eColor->color1.b = (float)json_array_get_number(colArr1, 2);
			eColor->color1.a = (float)json_array_get_number(colArr1, 3);

			//Get COLOR array
			JSON_Array* colArr2 = json_object_get_array(modulo, "Color2");

			//Get elements of position
			eColor->color2.r = (float)json_array_get_number(colArr2, 0);
			eColor->color2.g = (float)json_array_get_number(colArr2, 1);
			eColor->color2.b = (float)json_array_get_number(colArr2, 2);
			eColor->color2.a = (float)json_array_get_number(colArr2, 3);

			break;
		}
		case MAX:
			break;
		default:
			break;
		}
	}
	return pE;
}

void ComponentParticleSystem::LoadAllEmmitersJSON(const char* path)
{
	JSON_Value* root_value = json_parse_file(path);
	JSON_Object* root_object = json_value_get_object(root_value);

	int numEmitters = json_object_get_number(root_object, "NumEmitters");

	JSON_Array* arr = json_object_get_array(root_object, "Emitters");
	for (int i = 0; i < numEmitters; i++)
	{
		uint32_t emmiterID = json_array_get_number(arr,i);

		std::string pathEmitter;
		pathEmitter += PARTICLES_PATH;
		pathEmitter += std::to_string(emmiterID);
		pathEmitter += PAR;
		this->allEmitters.push_back(LoadEmitterFromMeta(pathEmitter.c_str()));
	}
}




