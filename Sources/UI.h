#ifndef UI_CLASS
#define UI_CLASS

#include "imgui-1.65/imgui.h"

class UI
{
public:
	UI();
	UI(bool enabled, ImVec2 pos);
	~UI();

	//Get needed info
	virtual bool isEnabled() const;
	virtual ImVec2 getPos() const;

	void Render();

private:
	bool enabled;
	ImVec2 pos;
};

#endif // !UI_CLASS