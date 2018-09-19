#ifndef MODULE_UI
#define MODULE_UI

#include "Module.h"
#include "Globals.h"
#include <list>

#include "UI.h"
#include "UI_About.h"
#include "UI_Configuration.h"

class ModuleUI : public Module
{
public:
	ModuleUI( bool start_enabled = true);
	~ModuleUI();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	//Send the fps data to the config window
	void send_FPSdata(float value);

private:
	//List of UI elements
	std::list<UI*> UI_Elements;

	//UI Components
	bool draw_menu = true;
	bool draw_demo;

	UI_About* about;
	UI_Configuration* config;
};

#endif // !MODULE_UI