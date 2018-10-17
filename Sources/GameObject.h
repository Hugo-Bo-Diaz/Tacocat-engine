#ifndef GAMEOBJECT
#define GAMEOBJECT

#include <list>

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

private:




};

#endif // !1
