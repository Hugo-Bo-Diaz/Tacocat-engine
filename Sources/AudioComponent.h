#pragma once

#include "Component.h"
#include "Audio.h"

class Component_Audio : public Component
{
public:

	bool listener;

	Audio* audio;


};