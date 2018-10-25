#include "GameObject.h"
#include "Component.h"
#include "MaterialComponent.h"
#include "MeshComponent.h"
#include "imgui-docking/imgui.h"
#include "Application.h"

void GameObject::Update(float dt)
{
	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		(*it)->Update(dt);
	}

	for (std::list<GameObject*>::iterator it = children.begin(); it != children.end(); it++)
	{
		(*it)->Update(dt);
	}

}

uint GameObject::GetTexture(uint index)
{
	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->type == MATERIAL && ((Component_Material*)(*it))->material_index == index)
		{
			return ((Component_Material*)(*it))->texture_buffer_id;//the first texture it finds in its components is given
		}
	}

	return 0;
}

AABB GameObject::GetBoundingBox()
{
	AABB box = {float3(100,100,100),float3(-100-100-100)};

	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->type == MESH)
		{
			if (((Component_Mesh*)(*it))->bounding_box.minPoint.x < box.minPoint.x)
				box.minPoint.x = ((Component_Mesh*)(*it))->bounding_box.minPoint.x;
			if (((Component_Mesh*)(*it))->bounding_box.minPoint.y < box.minPoint.y)
				box.minPoint.y = ((Component_Mesh*)(*it))->bounding_box.minPoint.y;
			if (((Component_Mesh*)(*it))->bounding_box.minPoint.z < box.minPoint.z)
				box.minPoint.z = ((Component_Mesh*)(*it))->bounding_box.minPoint.z;

			if (((Component_Mesh*)(*it))->bounding_box.maxPoint.x > box.maxPoint.x)
				box.maxPoint.x = ((Component_Mesh*)(*it))->bounding_box.maxPoint.x;
			if (((Component_Mesh*)(*it))->bounding_box.maxPoint.y > box.maxPoint.y)
				box.maxPoint.y = ((Component_Mesh*)(*it))->bounding_box.maxPoint.y;
			if (((Component_Mesh*)(*it))->bounding_box.maxPoint.z > box.maxPoint.z)
				box.maxPoint.z = ((Component_Mesh*)(*it))->bounding_box.maxPoint.z;
		}
	}

	return box;
}

void GameObject::AddComponent(Component* comp)
{
	comp->parent = this;
	components.push_back(comp);

}

void GameObject::AddChild(GameObject * child)
{
	child->parent = this;
	children.push_back(child);

}

GameObject::GameObject()
{
	//NOTATION day-month-6 number random

	//it should be unique unless there are more than 1.000.000 objects added in the scene in the span of a day

	//this uid can be decoded to get when approximately was created

	UID = 0;

	time_t theTime = time(NULL);
	struct tm *aTime = localtime(&theTime);

	uint hours = aTime->tm_hour;//2 digits
	uint day = aTime->tm_mday; // 2 digits
	uint month = aTime->tm_mon + 1;//from 0-11 to 1-12  //2 digits

	uint random = App->random_int(0,999999);

	UID += day *   100000000;
	UID += month * 1000000;
	UID += random ;

}

GameObject::~GameObject()
{
	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		delete(*it);
	}
	components.clear();

}


void GameObject::Hierarchy()
{

}
