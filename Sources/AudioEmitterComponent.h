#pragma once

#include "Component.h"
#include "Wwise_include.h"
class AudioEvent
{


};

class Component_Audio_Emitter : public Component
{
public:
	Component_Audio_Emitter();

	AkGameObjectID Wwise_obj;//UID of the main object??
	std::string name;

	bool attached = true;
	void CalculatePositionWithObjectTransform();

	void PlayEvent(unsigned long int event);

	void Properties();
	void SetWwiseObject();//parent needed for this to work

	void Update(float dt);

	AkSoundPosition position;
};