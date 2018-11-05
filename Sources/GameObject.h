#ifndef GAMEOBJECT
#define GAMEOBJECT

#include <list>
#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"
#include <time.h>
#include "rapidjson/rapidjson.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/reader.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"

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

	void Save(rapidjson::Document* d, rapidjson::Value* v);


private:


};

#endif // !1
