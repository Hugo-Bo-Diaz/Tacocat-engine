#ifndef TRANSFORMCOMPONENT_CLASS
#define TRANSFORMCOMPONENT_CLASS

#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"

class TransformComponent : public Component
{
public:
	TransformComponent();
	~TransformComponent();

	void SetPos(float x, float y, float z);
	void SetRot(float x, float y, float z);
	void SetScale(float x, float y, float z);

	void Update();

	float4x4 GetTransform();

private:
	float3 pos;
	Quat rot;
	float3 scale;

	float4x4 transform;
};

#endif