#pragma once
#include "Module.h"
#include <array>
#include "Globals.h"
#include "Primitive.h"

//#include "MathGeoLib/MathGeoLib.h"

#include "pcg32\pcg_variants.h"


class ModuleSceneController : public Module
{
public:
	ModuleSceneController(bool start_enabled = true);
	~ModuleSceneController();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:

};
