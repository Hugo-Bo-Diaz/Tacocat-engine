#pragma once
#include "Module.h"
#include <array>
#include "Globals.h"
//#include "Primitive.h"

//#include "MathGeoLib/MathGeoLib.h"

#include "pcg32\pcg_variants.h"

#define MAX_SNAKE 2

//struct PhysBody3D;
//struct PhysMotor3D;


class ModuleSceneController : public Module
{
public:
	ModuleSceneController(bool start_enabled = true);
	~ModuleSceneController();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	//void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	void Draw();

	//void SetToCP(PhysVehicle3D* v);

	//sph1 and sph2 collide
	//Sphere sph1;
	//Sphere sph2;

	////cap1 and cap2 don't collide
	//AABB cube1;
	//AABB cube2;

	//Plane grid;

public:

};
