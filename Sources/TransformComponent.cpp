#include "TransformComponent.h"

TransformComponent::TransformComponent()
{
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::Move(float3 newpos)
{
	pos = newpos;
}

void TransformComponent::Rotate(float3 newrot)
{
	quaternion.FromEulerXYZ(rot.x, rot.y, rot.z);

	Quat X, Y, Z;

	X.FromEulerXYZ(newrot.x, 0.0f, 0.0f);
	Y.FromEulerXYZ(0.0f, newrot.y, 0.0f);
	Z.FromEulerXYZ(0.0f, 0.0f, newrot.z);

	quaternion.RotateX(newrot.x);
}