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
	pl = new Cube;
	pl->size = { 1000, 1, 1000 };
	pl->SetPos(10, 0, 0);
	

	plane_sensor = App->physics->AddBody(*pl, 0);
	plane_sensor->SetAsSensor(true);
	plane_sensor->collision_listeners.add(this);

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

	pl->Render();

	p2List_item<Sensor*>* item = sensors.getFirst();
	while (item != NULL) {
		item->data->shape->Render();
		item = item->next;
	}

	p2List_item<CircuitPart*>*  item_2 = circuit.getFirst();

	while (item_2 != NULL) {
		
		item_2->data->shape->Render();

		item_2 = item_2->next;
	}

	
	

	return UPDATE_CONTINUE;
}

void ModuleSceneController::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body1->IsSensor()) {
		if (body1 == plane_sensor) {
			if(body2->body->getCenterOfMassPosition() == App->player->vehicle->body->getCenterOfMassPosition())
				set_to_cp = true;
		
		}
		else {
			p2List_item<Sensor*> *item = sensors.getFirst();
			while (item != NULL) {
				if (body1 == item->data->body) {
					last_cp = item->data->id;
					break;
				}
				item = item->next;
			}
		}

		
	}
	else if (body2->IsSensor()) {
		if (body2 == plane_sensor) {
			if (body1->body->getCenterOfMassTransform() == App->player->vehicle->body->getCenterOfMassTransform());
				set_to_cp = true;
		
		}
		else {
			p2List_item<Sensor*> *item = sensors.getFirst();
			while (item != NULL) {
				if (body2 == item->data->body) {
					last_cp = item->data->id;
					break;
				}
				item = item->next;
			}
		}
	}

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
