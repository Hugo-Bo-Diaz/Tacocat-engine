#include "Globals.h"
#include "Application.h"
#include "ModuleSceneController.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneController::ModuleSceneController(bool start_enabled) : Module(start_enabled)
{
}

ModuleSceneController::~ModuleSceneController()
{}

// Load assets
bool ModuleSceneController::Start()
{
	LOG("Loading Intro assets");
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
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneController::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	
	

	return UPDATE_CONTINUE;
}

void ModuleSceneController::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
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
