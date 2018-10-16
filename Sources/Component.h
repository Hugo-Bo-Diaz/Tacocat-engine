#ifndef COMPONENT
#define COMPONENT

enum ComponentType
{
	MESH,
	MATERIAL,
	TRANSFORM,
	LIGHT,
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


};

#endif // !COMPONENT
