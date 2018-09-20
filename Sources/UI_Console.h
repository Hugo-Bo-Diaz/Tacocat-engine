#ifndef UI_CONSOLE_CLASS
#define UI_CONSOLE_CLASS

#include "UI.h"

class UI_Console : public UI
{
public:
	UI_Console();
	~UI_Console();

	void Render() override;
};

#endif // !UI_CONSOLE_CLASS