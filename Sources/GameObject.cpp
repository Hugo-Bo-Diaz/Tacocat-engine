#include "GameObject.h"
#include "Component.h"


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

void GameObject::AddComponent(Component* comp)
{

	components.push_back(comp);

}

GameObject::~GameObject()
{
	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		delete(*it);
	}
	components.clear();

}