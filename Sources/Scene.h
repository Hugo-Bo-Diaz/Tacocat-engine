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
	void Update(float dt) 
	{
		for (std::list<GameObject*>::iterator it = GameObjects.begin(); it != GameObjects.end(); it++)
		{
			(*it)->Update(dt);
			delete(*it);
		}
	}
	virtual void CleanUp() {};

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