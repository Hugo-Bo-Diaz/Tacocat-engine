#ifndef MODULE_UI
#define MODULE_UI

#include "Module.h"
#include "Globals.h"
#include "UI.h"
#include <list>

class ModuleUI : public Module
{
public:
	ModuleUI( bool start_enabled = true);
	~ModuleUI();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp() { return true; };

private:
	//list of ui elements
	std::list<UI*> UI_Elements;

	bool draw_demo;
};

#endif // !MODULE_UI