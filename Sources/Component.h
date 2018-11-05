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

	virtual void Save_Component() {};
	virtual void Load_Component() {};

	GameObject* parent;

};

#endif // !COMPONENT
