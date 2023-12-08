#include "ComponentCamera.h"

#include "Application.h"

#include "MathGeoLib/include/MathGeoLib.h"
#include "MathGeoLib/include/MathBuildConfig.h"

#include "ComponentTransform.h"

#include "imGui/imgui.h"
#include "imGui/backends/imgui_impl_opengl3.h"
#include "imGui/backends/imgui_impl_SDL2.h"

ComponentCamera::ComponentCamera() : nearDistance(0.1f), farDistance(500.0f), fov(60.0f)
{
	type = ComponentType::CAMERA;

	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;

	frustum.nearPlaneDistance = nearDistance;
	frustum.farPlaneDistance = farDistance;

	frustum.verticalFov = fov * DEGTORAD;
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * ((16.0f / 9.0f)));

	frustum.pos = float3::zero;
}

ComponentCamera::ComponentCamera(GameObject* own) : nearDistance(1.0f), farDistance(100.0f), fov(60.0f)
{
	owner = own;
	type = ComponentType::CAMERA;

	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;

	frustum.nearPlaneDistance = nearDistance;
	frustum.farPlaneDistance = farDistance;

	frustum.verticalFov = fov * DEGTORAD;
	frustum.horizontalFov = 2.0f * atanf(tanf(fov * DEGTORAD / 2.0f) * ((16.0f / 9.0f)));

	frustum.pos = own->objTransform->pos;
}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::OnEditor()
{

	std::string butonChar;

	//Give an ID to each colapsing header to be able to have more than one of the same time
	//This must be done due to ImGui using the names as the ids of all menus and things
	int myPosInComponents = owner->GetComponentPosition(this);
	std::string idComponent;
	idComponent.append("##");
	idComponent.append(std::to_string(myPosInComponents).c_str());

	butonChar.clear();
	butonChar.append("Camera");
	if (ImGui::CollapsingHeader(butonChar.append(idComponent).c_str()))
	{
		butonChar.clear();
		butonChar.append("##CameraActive");
		ImGui::Checkbox(butonChar.append(idComponent).c_str(), &active); //El doble ## hace que no se muestre el texto. Es necesario poner un nombre distinto a cada checkbox y boton ya que ImGui usa el nombre como la ID

		butonChar.clear();

		if (ImGui::DragFloat("Near Plane Distance", &nearDistance, 0.1f, 0.01f, nearDistance))
		{
			frustum.nearPlaneDistance = nearDistance;
		}
		if (ImGui::DragFloat("Far Plane Distance", &farDistance, 0.1f, 10000.0f, farDistance))
		{
			frustum.farPlaneDistance = farDistance;
		}
		ImGui::Text("Vertical FOV: %f", frustum.verticalFov);
		ImGui::Text("Horizontal FOV: %f", frustum.horizontalFov);

		if (ImGui::DragFloat("FOV", &fov, 0.1f, 1.0f, 180.0f))
		{
			frustum.verticalFov = fov;
		}
	}
}

bool ComponentCamera::Update()
{
	return false;
}

void ComponentCamera::StartCamera()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((GLfloat*)frustum.ProjectionMatrix().Transposed().v);
	glMatrixMode(GL_MODELVIEW);
	math::float4x4 matrix = frustum.ViewMatrix();
	glLoadMatrixf((GLfloat*)matrix.Transposed().v);
	DrawCameraLines();
}

void ComponentCamera::DrawCameraLines()
{
	static float3 points[8];
	frustum.GetCornerPoints(points);

	glLineWidth(5);
	glColor3f(255, 255, 0);
	glBegin(GL_LINES);

	glVertex3f(points[0].x, points[0].y, points[0].z);
	glVertex3f(points[1].x, points[1].y, points[1].z);

	glVertex3f(points[0].x, points[0].y, points[0].z);
	glVertex3f(points[4].x, points[4].y, points[4].z);

	glVertex3f(points[4].x, points[4].y, points[4].z);
	glVertex3f(points[5].x, points[5].y, points[5].z);

	glVertex3f(points[0].x, points[0].y, points[0].z);
	glVertex3f(points[2].x, points[2].y, points[2].z);

	glVertex3f(points[2].x, points[2].y, points[2].z);
	glVertex3f(points[3].x, points[3].y, points[3].z);

	glVertex3f(points[1].x, points[1].y, points[1].z);
	glVertex3f(points[3].x, points[3].y, points[3].z);

	glVertex3f(points[1].x, points[1].y, points[1].z);
	glVertex3f(points[5].x, points[5].y, points[5].z);

	glVertex3f(points[4].x, points[4].y, points[4].z);
	glVertex3f(points[6].x, points[6].y, points[6].z);

	glVertex3f(points[2].x, points[2].y, points[2].z);
	glVertex3f(points[6].x, points[6].y, points[6].z);

	glVertex3f(points[6].x, points[6].y, points[6].z);
	glVertex3f(points[7].x, points[7].y, points[7].z);

	glVertex3f(points[5].x, points[5].y, points[5].z);
	glVertex3f(points[7].x, points[7].y, points[7].z);

	glVertex3f(points[3].x, points[3].y, points[3].z);
	glVertex3f(points[7].x, points[7].y, points[7].z);

	glEnd();
	glLineWidth(1);
}
