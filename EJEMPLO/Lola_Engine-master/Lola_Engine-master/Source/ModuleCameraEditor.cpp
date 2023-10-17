#include "ModuleCameraEditor.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleGameObjectManager.h"

#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"

//#include "PhysBody3D.h"

#include "MathGeoLib\MathGeoLib.h"

ModuleCameraEditor::ModuleCameraEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	sprintf_s(name, SHORT_STRING, "Camera Editor");
}

ModuleCameraEditor::~ModuleCameraEditor()
{}


bool ModuleCameraEditor::Awake(JSONParser &config)
{
	camera = App->gameobject_manager->CreateGameObject("Main_camera", nullptr);
	camera->AddComponent(COMPONENT_TYPE::COMP_TRANSFORM);
	ComponentCamera *c = (ComponentCamera*)camera->AddComponent(COMPONENT_TYPE::COMP_CAMERA);
	c->editor_camera = true;
	c->SetComponent();

	return true;
}

bool ModuleCameraEditor::Start()
{
	DEBUG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCameraEditor::CleanUp()
{
	DEBUG("Cleaning camera");
	return true;
}

// -----------------------------------------------------------------
UPDATE_STATUS ModuleCameraEditor::Update(float dt)
{
	math::vec movement = math::vec::zero;

	float speed = 3.0f * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) speed = 8.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) movement += speed * camera->transform->up;
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) movement += speed * -camera->transform->up;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) movement += speed * camera->transform->forward;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) movement += speed * -camera->transform->forward;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) movement += speed * camera->transform->left;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) movement += speed * -camera->transform->left;

	camera->transform->Move(movement);

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = App->input->GetMouseYMotion();

		float sensitivity = 0.025f;
		
		if(dx != 0) camera->transform->RotateAngleAxis(dx * sensitivity, math::vec(0.0f, 1.0f, 0.0f));
		if(dy != 0) camera->transform->RotateAngleAxis(dy * sensitivity, math::vec(1.0f, 0.0f, 0.0f));
	}

	// Mouse picking
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && !ImGui::IsMouseHoveringAnyWindow())
		MousePick();
	
	return UPDATE_CONTINUE;
}

void ModuleCameraEditor::MousePick()
{
	math::float2 half_screen_size(App->window->GetScreenWidth() / 2.0f, App->window->GetScreenHeight() / 2.0f);
	math::float2 normalized_pos((App->input->GetMouseX() - half_screen_size.x) / half_screen_size.x, (App->input->GetMouseY() - half_screen_size.y) / half_screen_size.y);

	math::Frustum cam_frustum;
	camera->GetFrustum(cam_frustum);

	ray_cast = cam_frustum.UnProjectLineSegment(normalized_pos.x, -normalized_pos.y);
	App->gameobject_manager->RayCast(ray_cast);
}

void ModuleCameraEditor::SetAsEditorCamera(GameObject *go) { camera = go; }
const math::vec &ModuleCameraEditor::GetCameraPos() const { return camera->transform->GetPos(); }
const GameObject *ModuleCameraEditor::GetEditorCamera() const { return camera; }