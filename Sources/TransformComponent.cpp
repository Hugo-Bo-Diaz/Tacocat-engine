#include "TransformComponent.h"

TransformComponent::TransformComponent()
{
	type = TRANSFORM;
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
