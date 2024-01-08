#include "Component.h"
#include "ComponentParticleSystem.h"
#include "ComponentMesh.h" //Necesary for AABBs
#include "GameObject.h"

#include "MathGeoLib/include/Math/Quat.h"

#include "imGui/imgui.h"
#include "imGui/backends/imgui_impl_opengl3.h"
#include "imGui/backends/imgui_impl_SDL2.h"

#include "EmmiterInstance.h"

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

void ComponentParticleSystem::SetValues(aiVector3D translation, aiVector3D scaling, aiQuaternion rotation)
{
	pos = { translation.x,translation.y,translation.z };

	scale = { scaling.x, scaling.y, scaling.z };

	rot = { rotation.x, rotation.y, rotation.z, rotation.w };
}



void ComponentParticleSystem::OnEditor()
{
	if(ImGui::CollapsingHeader("ParticleSystem"))
	{

		ImGui::Checkbox("##ParticleSystem", &active);
		
		
	}
}




