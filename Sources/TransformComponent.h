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

	float3 GetPos() const;
	float3 GetRot() const;
	float3 GetScale() const;

	void Update();

	float4x4 GetTransform();

	void Properties();

private:
	float3 pos;
	Quat rot;
	float3 scale;

	float4x4 transform;
};

#endif