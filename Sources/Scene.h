#ifndef SCENE
#define SCENE

#include "Quadtree.h"
#include "GameObject.h"
#include "CameraComponent.h"
#include <string>
#include <vector>


class Scene
{
public:

	int id = -1;
	std::string name;

	Scene() {};
	Scene(const char* _name) 
	{ 
		name = _name;
		spookamera = new Component_Camera(); 
		tree = new NOTQuadtree(float3(-100,-1,-100),float3(100,1,100));
	};

	~Scene();

	virtual void Init() {};
	void Update(float dt);

	virtual void CleanUp() {};

	void LoadToScene(const char* file);

	void Enable() { enabled = true; };
	void Disable() { enabled = false; };

	GameObject* AddGameObject();

	std::vector<GameObject*> GameObjects;

	Component_Camera* spookamera;

	NOTQuadtree* tree;

private:

	//todo
	//add light
	//add camera
	//add root(empty object on 0,0,0)

	bool enabled = false;

};

#endif 