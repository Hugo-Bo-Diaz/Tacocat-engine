#include "GameObject.h"


void GameObject::Update(float dt)
{

	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		(*it)->Update(dt);

		delete(*it);
	}

}

//void GameObject::AddComponent()
//{
//
//
//
//}