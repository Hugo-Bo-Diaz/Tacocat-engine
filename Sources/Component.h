#ifndef COMPONENT
#define COMPONENT

#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"

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

	virtual void Save_Component(rapidjson::Document* d, rapidjson::Value* v) {};
	virtual void Load_Component(rapidjson::Value::ConstMemberIterator v) {};

	GameObject* parent;

	virtual void Properties() {}; //Elements to draw in the UI
};

#endif // !COMPONENT
