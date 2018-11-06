#include "GameObject.h"
#include "Component.h"
#include "MaterialComponent.h"
#include "MeshComponent.h"
#include "imgui-docking/imgui.h"

GameObject::GameObject()
{
	transform = new TransformComponent(); //All gameobjects need transform
	AddComponent(transform);
}

GameObject::~GameObject()
{
	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		delete(*it);
	}
	components.clear();

}

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

void GameObject::Hierarchy()
{

}
