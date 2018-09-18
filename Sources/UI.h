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
	bool isEnabled() const;
	ImVec2 getPos() const;

	//Add needed info
	void Enable(bool enable);
	void setPos(ImVec2 position);

	virtual void Render();

private:
	bool enabled;
	ImVec2 pos;
};

#endif // !UI_CLASS