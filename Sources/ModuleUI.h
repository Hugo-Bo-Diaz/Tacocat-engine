#ifndef MODULE_UI
#define MODULE_UI

#include "Module.h"
#include "Globals.h"
#include <list>

#include "UI.h"
#include "UI_About.h"

class ModuleUI : public Module
{
public:
	ModuleUI( bool start_enabled = true);
	~ModuleUI();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

private:
	//List of UI elements
	std::list<UI*> UI_Elements;

	//UI Components
	bool draw_menu = true;
	bool draw_demo;

	UI_About* about;
};

#endif // !MODULE_UI