#include "Component.h"
#include "ComponentParticleSystem.h"
#include "ComponentMesh.h" //Necesary for AABBs
#include "GameObject.h"

#include "MathGeoLib/include/Math/Quat.h"

#include "imGui/imgui.h"
#include "imGui/backends/imgui_impl_opengl3.h"
#include "imGui/backends/imgui_impl_SDL2.h"

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
		
	}
}




