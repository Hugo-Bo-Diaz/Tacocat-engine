#ifndef GAMEOBJECT
#define GAMEOBJECT

#include <list>
#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"
#include <time.h>

class Component;

class GameObject
{
public:



	GameObject();
	~GameObject();
	GameObject* parent = nullptr;

	void AddComponent(Component* comp);
	void AddChild(GameObject* child);

	void Update(float dt);

	std::list<Component*> components;
	std::list<GameObject*> children;

	uint GetTexture(uint index);

	bool static_object = true;

	AABB GetBoundingBox();

	void Hierarchy();

	uint UID;

	void Save();

private:


};

#endif // !1
