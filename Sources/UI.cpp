#include "UI.h"

UI::UI()
{

}

UI::UI(bool enabled, ImVec2 pos) : enabled(enabled), pos(pos) 
{

}

UI::~UI() 
{

}

bool UI::isEnabled() const
{
	return true;
}

ImVec2 UI::getPos() const
{
	return pos;
}

void UI::Render()
{
	//Dear ImGui functions needed to draw the UI
}
