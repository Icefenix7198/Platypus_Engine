#include "ComponentCamera.h"

#include "Application.h"

#include "imGui/imgui.h"
#include "imGui/backends/imgui_impl_opengl3.h"
#include "imGui/backends/imgui_impl_SDL2.h"

ComponentCamera::ComponentCamera()
{
	type = ComponentType::CAMERA;
}

ComponentCamera::ComponentCamera(GameObject* own)
{
	owner = own;
	type = ComponentType::CAMERA;
}

ComponentCamera::~ComponentCamera()
{
}
