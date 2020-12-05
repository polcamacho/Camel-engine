#include "Camera.h"
#include "Globals.h"
#include "Transform.h"
#include "Mesh.h"
#include "GameObject.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "ImGui/imgui.h"
#include "ModuleCamera3D.h"

#include "glew/include/glew.h"

Camera::Camera() : Component() {
	
	name = { "Camera" };
	type = ComponentType::CAMERA;

	frustum.type = PerspectiveFrustum;

	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;
	frustum.pos = float3(0,0,0);

	frustum.farPlaneDistance = 50.0f;
	frustum.nearPlaneDistance = 1.0f;
	frustum.verticalFov = 60.0f*DEGTORAD;
	aspect_ratio = (float)16/9;
	frustum.horizontalFov = 2 * atanf(tanf(frustum.verticalFov / 2) * aspect_ratio);

}

Camera::~Camera()
{

}

void Camera::Update()
{
	frustum.pos = gameObject->GetTransform()->GetPosition();
	frustum.up = gameObject->GetTransform()->GetGlobalTransform().WorldY();
	frustum.front = gameObject->GetTransform()->GetGlobalTransform().WorldZ();

	GetProjectionMatrix();
	DrawCameraLines();
	
}

void Camera::OnEditor()
{
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float new_near_plane = GetNearPlane();
		float new_far_plane = GetFarPlane();
		float new_FOV = GetFOV();

		if (ImGui::SliderFloat("Near Plane", &new_near_plane, 0, 50) && new_near_plane < new_far_plane) {
			SetNearPlane(new_near_plane);
		}
		if (ImGui::SliderFloat("Far Plane", &new_far_plane, 0, 1000) && new_near_plane < new_far_plane) {
			SetFarPlane(new_far_plane);
		}
		if (ImGui::SliderFloat("Vertical FOV", &new_FOV, 0, 90)) {
			SetFOV(new_FOV);
		}
		ImGui::Text("Aspect ratio: %.2f", aspect_ratio);
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
	return frustum.verticalFov * RADTODEG;
}

void Camera::SetNearPlane(float nearP) {
	frustum.nearPlaneDistance = nearP;
}

void Camera::SetFarPlane(float farP) {
	frustum.farPlaneDistance = farP;
}

void Camera::SetFOV(float vFOV) {
	frustum.verticalFov = DEGTORAD * vFOV;
	frustum.horizontalFov = 2 * atanf(tanf(frustum.verticalFov / 2) * aspect_ratio);
}

void Camera::SetAspectRatio(float a_ratio) {
	aspect_ratio = a_ratio;
	frustum.horizontalFov = 2 * atanf(tanf(frustum.verticalFov / 2) * a_ratio);
}

float* Camera::GetProjectionMatrix() {
	float4x4 new_matrix;
	new_matrix = frustum.ProjectionMatrix();
	new_matrix.Transpose();

	return (float*)new_matrix.v;
}

float* Camera::GetViewMatrix() {
	float4x4 new_matrix;
	new_matrix = frustum.ViewMatrix();
	new_matrix.Transpose();

	return (float*)new_matrix.v;
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

// -----------------------------------------------------------------
void Camera::LookAt(float3& Spot)
{
	
	float3 direction = Spot - frustum.pos;
	float3x3 matrix = float3x3::LookAt(frustum.front, direction.Normalized(), frustum.up, float3(0, 1, 0));

	frustum.front = matrix.MulDir(frustum.front).Normalized();
	frustum.up = matrix.MulDir(frustum.up).Normalized();

}

void Camera::CullingObjects(GameObject* go)
{	
	GnMesh* mesh = (GnMesh*)go->GetComponent(ComponentType::MESH);

	if (ContainsAaBox(go->GetAABB())==true) {
		if(mesh!= nullptr)
		mesh->Enable();
	}
	else {
		if (mesh != nullptr)
		mesh->Disable();
	}
}

bool Camera::ContainsAaBox(AABB refBox)
{
	bool ret = true;
	float3 corner_vert[8];
	int iTotalIn = 0;
	Plane* m_plane = new Plane[6];
	
	refBox.GetCornerPoints(corner_vert); // get the corners of the box into the vCorner array
	frustum.GetPlanes(m_plane);
	// test all 8 corners against the 6 sides
	// if all points are behind 1 specific plane, we are out
	// if we are in with all points, then we are fully in
	for (int p = 0; p < 6; ++p) {
		int iInCount = 8;
		int iPtIn = 1;
		for (int i = 0; i < 8; ++i) {
			if (m_plane[p].IsOnPositiveSide(corner_vert[i]))
			{
				iPtIn = 0;
				--iInCount;
			}
		}
		if (iInCount == 0) {
			return false;
		}
		iTotalIn += iPtIn;
	}
	// so if iTotalIn is 6, then all are inside the view
	if (iTotalIn == 6){
		return true;
	}
	// we must be partly in then otherwise
	return ret;
}