#ifndef UI_CLASS
#define UI_CLASS

#include "imgui-1.65/imgui.h"

class UI
{
public:
	UI();
	~UI();

	bool Start();

	bool isEnabled();
	void Render();

private:
	bool enabled;
};

#endif // !UI_CLASS