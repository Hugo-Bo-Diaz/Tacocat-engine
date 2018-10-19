#pragma once
#include "Module.h"
#include <array>
#include "Globals.h"
#include "Primitive.h"

//#include "MathGeoLib/MathGeoLib.h"

#include "pcg32\pcg_variants.h"

#include "Scene.h"

class ModuleSceneController : public Module
{
public:
	ModuleSceneController(bool start_enabled = true);
	~ModuleSceneController();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	Scene* CreateScene();
	Scene* CreateScene(const char* name);
	void DeleteScene(Scene*);

	void ChangeScene(Scene*);
	void ChangeScene(int id);

	Scene* current_scene = nullptr;

	int scene_number;

	Component_Camera* GetMainCamera();

private:

	std::vector<Scene*> scenes;
	Scene* FindSceneById(int id);

};
