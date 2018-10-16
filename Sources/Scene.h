#ifndef SCENE
#define SCENE

#include "GameObject.h"
#include <string>

class Scene
{
public:

	int id = -1;
	std::string name;

	Scene() {};
	Scene(const char* _name) { name = _name; };

	virtual void Init() {};
	void Update(float dt);

	virtual void CleanUp() {};

	void LoadToScene(const char* file);

	void Enable() { enabled = true; };
	void Disable() { enabled = false; };

private:

	//todo
	//add light
	//add camera
	//add root(empty object on 0,0,0)

	std::list<GameObject*> GameObjects;
	bool enabled = false;

};

#endif 