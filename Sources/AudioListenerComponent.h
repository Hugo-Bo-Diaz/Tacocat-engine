#pragma once

#include "Component.h"
#include "Wwise_include.h"

class Component_Audio_Listener: public Component
{

public:

	Component_Audio_Listener();

	void CalculatePositionWithObjectTransform();

	void Properties();
	void SetWwiseObject();//parent needed for this to work
	void Update(float dt);

	bool attached = true;
	std::string name;
	AkGameObjectID Wwise_obj;//UID of the main object??
	AkSoundPosition position;

};