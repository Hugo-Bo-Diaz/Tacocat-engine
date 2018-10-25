#ifndef COMPONENT
#define COMPONENT

#include "parson/parson.h"

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

	virtual void Save_Component(JSON_Object* root, const char* prev) {};
	virtual void Load_Component(JSON_Object* root, const char* prev) {};

	GameObject* parent;

};

#endif // !COMPONENT
