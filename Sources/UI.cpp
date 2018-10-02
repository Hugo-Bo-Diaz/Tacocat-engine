#include "UI.h"

#include "mmgr\mmgr.h"

UI::UI()
{
	enabled = false;
	pos.x = 200;
	pos.y = 100;
}

UI::UI(bool enabled, ImVec2 pos) : enabled(enabled), pos(pos) 
{

}

UI::~UI() 
{

}

bool UI::isEnabled() const
{
	return enabled;
}

ImVec2 UI::getPos() const
{
	return pos;
}

void UI::Render()
{
	//Dear ImGui functions needed to draw the UI
}

void UI::Enable(bool enable)
{
	enabled = enable;
}

void UI::setPos(ImVec2 position)
{
	pos = position;
}