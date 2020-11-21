#include "Camera.h"
#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "ImGui/imgui.h"


#include "glew/include/glew.h"

Camera::Camera() : Component() {
	
	name = { "Camera" };
	type = ComponentType::CAMERA;

	frustum.type = PerspectiveFrustum;

	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;
	frustum.pos = float3(0.0f, 0.0f, 0.0f);

	frustum.farPlaneDistance = 50.0f;
	frustum.nearPlaneDistance = 1.0f;
	frustum.verticalFov = DegToRad(70.0f);
	
	/*frustum.horizontalFov = 9;
	frustum.AspectRatio();*/
}

Camera::~Camera()
{

}

void Camera::Update()
{
	//frustrum.ProjectionMatrix();
	DrawCameraLines();
}

void Camera::OnEditor()
{
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::SliderFloat("Near Plane", &frustum.nearPlaneDistance, 0, 50);
		ImGui::SliderFloat("Far Plane", &frustum.farPlaneDistance, 50, 400);
	}
}

float Camera::GetNearPlane()
{
	return frustum.nearPlaneDistance;
}

float Camera::GetFarPlane()
{
	return frustum.farPlaneDistance;
}

float Camera::GetAspectRatio()
{
	return frustum.AspectRatio();
}

float Camera::GetFov()
{
	return RadToDeg(frustum.verticalFov);
}



void Camera::DrawCameraLines()
{
	glBegin(GL_LINES);
	glLineWidth(5.0f);
	glColor4f(1.0f, 0.0f, 0.5f, 1.0f);

	for (uint i = 0; i < 12; i++)
	{
		glVertex3f(frustum.Edge(i).a.x, frustum.Edge(i).a.y, frustum.Edge(i).a.z);
		glVertex3f(frustum.Edge(i).b.x, frustum.Edge(i).b.y, frustum.Edge(i).b.z);
	}
	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}