#ifndef UI_ABOUT_CLASS
#define UI_ABOUT_CLASS

#include "UI.h"

class UI_About : public UI
{
public:
	UI_About();
	~UI_About();

	void Render() override;
};

#endif // !UI_ABOUT_CLASS
