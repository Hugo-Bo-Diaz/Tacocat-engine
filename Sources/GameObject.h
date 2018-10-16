#ifndef GAMEOBJECT
#define GAMEOBJECT

#include <list>

class Component;

class GameObject
{
public:

	GameObject() {};
	~GameObject();
	std::list<GameObject*> children;
	GameObject* parent = nullptr;

	void AddComponent(Component* comp);

	void Update(float dt);

private:

	std::list<Component*> components;


};

#endif // !1
