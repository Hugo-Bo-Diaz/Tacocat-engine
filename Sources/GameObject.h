#ifndef GAMEOBJECT
#define GAMEOBJECT

#include <list>
#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"

class Component;

class GameObject
{
public:

	GameObject() {};
	~GameObject();
	GameObject* parent = nullptr;

	void AddComponent(Component* comp);
	void AddChild(GameObject* child);

	void Update(float dt);

	std::list<Component*> components;
	std::list<GameObject*> children;

	uint GetTexture(uint index);

	AABB GetBoundingBox();

	void Hierarchy();

private:




};

#endif // !1
