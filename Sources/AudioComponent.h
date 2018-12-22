#pragma once

#include "Component.h"
#include "Audio.h"

enum audio_component
{
 EMMISOR,
 LISTENER,
 DISTORTER,

};

class Component_Audio : public Component
{
public:
	AkGameObjectID Wwise_obj;//UID of the main object??
	bool listener;

	audio_component type;

	Audio* audio;
	AkUniqueID id;//test
	
	union
	{
		struct
		{
			//i'm
		} emmisor;

		struct
		{
			//working
		} distorter;

		struct
		{
			//on
		} listener;

			//it
	};


};