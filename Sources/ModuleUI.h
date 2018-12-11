#ifndef MODULE_UI
#define MODULE_UI

#include "Module.h"
#include "Globals.h"
#include <list>

#include "UI.h"
#include "UI_About.h"
#include "UI_Configuration.h"
#include "UI_Console.h"
#include "UI_Primitives.h"
#include "UI_Properties.h"
#include "UI_Gameobject.h"
#include "UI_Time.h"
#include "imgui-docking/ImGuizmo.h"

class ModuleUI : public Module
{
public:
	ModuleUI( bool start_enabled = true);
	~ModuleUI();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	UI_About* about;
	UI_Configuration* config;
	UI_Console* console;
	UI_Primitives* primitives;
	UI_Properties* properties;
	UI_GameObject* hierarchy;
	UI_Time* time;

	//Send the fps data to the config window
	void send_FPSdata(float value, float ms);

	ImGuiIO* io;
	ImGuizmo::OPERATION gizmo_operation = ImGuizmo::TRANSLATE;
	ImGuizmo::MODE gizmo_mode = ImGuizmo::WORLD;

	void DrawGuizmo();
	bool draw_guizmo = true;

	void Draw();

private:
	//List of UI elements
	std::list<UI*> UI_Elements;

	//UI Components
	bool draw_menu = true;
	bool draw_demo = true;




};

#endif // !MODULE_UI