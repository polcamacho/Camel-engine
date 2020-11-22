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
	aspect_ratio = 16 / 9;
	frustum.horizontalFov = 2 * atanf(tanf(frustum.verticalFov / 2) * aspect_ratio);
}

Camera::~Camera()
{

}

void Camera::Update()
{
	frustum.ProjectionMatrix();
	DrawCameraLines();
}

void Camera::OnEditor()
{
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float new_near_plane = GetNearPlane();
		float new_far_plane = GetFarPlane();
		if(ImGui::SliderFloat("Near Plane", &new_near_plane, 0, 100) && new_near_plane < new_far_plane)
			SetNearPlane(new_near_plane);

		if(ImGui::SliderFloat("Far Plane", &new_far_plane, 0, 100) && new_near_plane < new_far_plane)
			SetFarPlane(new_far_plane);

		float new_FOV = GetFOV();
		if (ImGui::SliderFloat("Vertical FOV", &new_FOV, 0, 50))
			SetFOV(new_FOV);

		float new_aspect_ratio = GetAspectRatio();
		if (ImGui::SliderFloat("Aspect Ratio", &new_aspect_ratio, 0, 50))
			SetAspectRatio(new_aspect_ratio);
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

float Camera::GetFOV()
{
	return RadToDeg(frustum.verticalFov);
}

void Camera::SetNearPlane(float nearP) {
	frustum.nearPlaneDistance = nearP;
}

void Camera::SetFarPlane(float farP) {
	frustum.farPlaneDistance = farP;
}

void Camera::SetFOV(float vFOV) {
	frustum.verticalFov = DegToRad(vFOV);
	frustum.horizontalFov = 2 * atanf(tanf(frustum.verticalFov / 2) * aspect_ratio);
}

void Camera::SetAspectRatio(float a_ratio) {
	aspect_ratio = a_ratio;
	frustum.horizontalFov = 2 * atanf(tanf(frustum.verticalFov / 2) * a_ratio);
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