#ifndef COMPONENT
#define COMPONENT

class GameObject;

enum ComponentType
{
	MESH,
	MATERIAL,
	TRANSFORM,
	LIGHT,
	CAMERA,
	MISCELANEOUS
};


class Component
{
public:

	ComponentType type;

	Component() { type = MISCELANEOUS; };
	Component(ComponentType _type) { type = _type; };

	~Component() {};
	virtual void Update(float dt) {};

	GameObject* parent;

};

#endif // !COMPONENT
