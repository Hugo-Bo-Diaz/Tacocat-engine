#include "TransformComponent.h"
#include "imgui-docking/imgui.h"

TransformComponent::TransformComponent()
{
	type = TRANSFORM;
	pos = { 0, 0, 0 };
	rot = rot.FromEulerXYZ(0, 0, 0);
	scale = { 1, 1, 1 };

	Update();
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::SetPos(float x, float y, float z)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;

	Update();
}

void TransformComponent::SetRot(float x, float y, float z)
{
	rot = rot.FromEulerXYZ(x, y, z);
	
	Update();
}

void TransformComponent::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;

	Update();
}

void TransformComponent::Update()
{
	transform = transform.FromTRS(pos, rot, scale);
}

float4x4 TransformComponent::GetTransform()
{
	return transform;
}

float3 TransformComponent::GetPos() const
{
	return pos;
}

float3 TransformComponent::GetRot() const
{
	return rot.ToEulerXYZ();
}

float3 TransformComponent::GetScale() const
{
	return scale;
}

void TransformComponent::Properties()
{
	float3 Position, Rotation, Scale;

	Position = GetPos();
	Rotation = GetRot();
	Scale = GetScale();

	ImGui::CollapsingHeader("Transform");

	ImGui::InputFloat3("Pos", &Position[0], 2);
	ImGui::InputFloat3("Rot", &Rotation[0], 2);
	ImGui::InputFloat3("Scale", &Scale[0], 2);
	ImGui::Separator();

	if (Position.x != pos.x || Position.y != pos.y || Position.z != pos.z)
	{
		SetPos(pos.x, pos.y, pos.z);
	}

	ImGui::End();
}