#ifndef GAMEOBJECT
#define GAMEOBJECT

#include "Component.h"
#include <list>


class GameObject
{
public:

	GameObject() {};
	std::list<Component*> components;
	std::list<GameObject*> children;
	GameObject* parent = nullptr;

	//Component* AddComponent();

	void Update(float dt);

private:


};

#endif // !1
