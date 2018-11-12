#ifndef GAMEOBJECT
#define GAMEOBJECT

#include <list>
#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"
#include <time.h>

#include "MeshComponent.h"
#include "TransformComponent.h"

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
	Component_Transform* GetTransform();

	bool static_object = true;

	bool selected = false;
	bool Iselected() 
	{
		if (selected)
		{
			return true;
		}
		else if (parent != nullptr && parent->Iselected())
		{
			return true;
		}
		else
		{
			return false;
		}
	};

	AABB GetBoundingBox();
	AABB BoundingBox;

	void Hierarchy();

	uint UID;

	void Save(rapidjson::Document* d, rapidjson::Value* v);

	std::vector<Component_Mesh*>* GetAllMeshes(std::vector<Component_Mesh*>& ret);

private:


};

#endif // !1
