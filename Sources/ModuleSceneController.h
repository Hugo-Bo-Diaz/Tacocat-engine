#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;


class ModuleSceneController : public Module
{
public:
	ModuleSceneController(bool start_enabled = true);
	~ModuleSceneController();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	//void SetToCP(PhysVehicle3D* v);

public:

};
