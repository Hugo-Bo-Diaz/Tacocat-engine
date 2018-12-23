#ifndef SCENE
#define SCENE

#include "GameObject.h"
#include "CameraComponent.h"
#include "AudioEmitterComponent.h"
#include "AudioListenerComponent.h"
#include <string>
#include <vector>
#include "Quadtree.h"

#include "Wwise_include.h"

class Scene
{
public:

	int id = -1;
	std::string name;

	Scene() {
	};
	Scene(const char* _name) 
	{ 
		name = _name;
		spookamera = new Component_Camera();
		tree = new Spooktree(ROOT, -500, -500, 500, 500, 0);

	};

	~Scene();

	void Init();
	void Update(float dt);

	virtual void CleanUp() {};

	void LoadToScene(const char* file);

	void Enable() { enabled = true; };
	void Disable() { enabled = false; };

	GameObject* AddGameObject();
	GameObject* GetObjectFromUID(uint UID);

	std::vector<GameObject*> GameObjects;

	GameObject* Camera;
	Component_Audio_Listener* listener;
	Component_Camera* spookamera;

	void Save(const char* filename);
	void Load(const char* filename);

	Spooktree* tree;
	int number_of_gameobjects = 0;

private:
	//todo
	//add light
	//add camera
	//add root(empty object on 0,0,0)

	bool enabled = false;

};

#endif 