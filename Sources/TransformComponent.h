#ifndef TRANSFORMCOMPONENT_CLASS
#define TRANSFORMCOMPONENT_CLASS

#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"

class TransformComponent : public Component
{
public:
	TransformComponent();
	~TransformComponent();

	void Move(float3 newpos);
	void Rotate(float3 newrot);

private:
	float3 pos;
	float3 rot;
	float3 scale;

	Quat quaternion;
};

#endif