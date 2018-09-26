#include "Globals.h"
#include "Application.h"
#include "ModuleSceneController.h"
#include "Primitive.h"
#include "PhysBody3D.h"

#include "mathgeolib/Geometry/Sphere.h"
#include "mathgeolib/Geometry/AABB.h"

#include "parson\parson.h"

ModuleSceneController::ModuleSceneController(bool start_enabled) : Module(start_enabled)
{
	name = "SceneController";
}

ModuleSceneController::~ModuleSceneController()
{}

// Load assets
bool ModuleSceneController::Start()
{
	grid.normal = { 0,1,0 };
	grid.d = -1;

	float3 pos1 = { 0,0,0 };
	float3 pos2 = { 5,5,0 };
	
	sph1.pos = pos1;
	sph1.r = 5;

	sph2.pos = pos2;
	sph2.r = 5;

	if (sph1.Intersects(sph2))
	{
		CONSOLE_LOG("WORKS");
	}

	cube1.minPoint = { 0,0,0 };
	cube1.maxPoint = { 1,1,1 };

	cube2.minPoint = { 2,0,0 };
	cube2.maxPoint = { 1,1,1 };


	if (!cube1.Intersects(cube2))
	{
		CONSOLE_LOG("WORKS");
	}

	int example1 = App->random_int(1000,1912);

	float example2 = App->random_between_0_1();

	CONSOLE_LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	/*p.normal = { 0,1,0 };
	p.constant = 0;*/

	//Create Map

	//principal circuit

	return ret;
}

// Load assets
bool ModuleSceneController::CleanUp()
{
	CONSOLE_LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneController::Update(float dt)
{
	



	
	

	return UPDATE_CONTINUE;
}

void ModuleSceneController::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{


}
void ModuleSceneController::Draw()
{

	
}
//void ModuleSceneController::SetToCP(PhysVehicle3D* v){
//	p2List_item<Sensor*>*item = sensors.getFirst();
//	while (item != NULL) {
//		if (item->data->id == last_cp)
//			break;
//		item = item->next;
//	}
//
//	v->body->setCenterOfMassTransform(item->data->body->body->getCenterOfMassTransform());
//
//}
//
