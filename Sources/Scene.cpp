#include "Scene.h"

void Scene::Update(float dt)
{
	for (std::list<GameObject*>::iterator it = GameObjects.begin(); it != GameObjects.end(); it++)
	{
		(*it)->Update(dt);
		delete(*it);
	}
}

void Scene::LoadToScene(const char* file)
{
	//TODO
	//FIRST: check the file extension
	//SECOND: call the correspondant loader
	//THIRD: generate the gameobjects needed
	//FOURTH: profit

}