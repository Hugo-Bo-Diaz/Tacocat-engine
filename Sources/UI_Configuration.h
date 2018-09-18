#ifndef UI_CONFIGURATION_CLASS
#define UI_CONFIGURATION_CLASS

#include "UI.h"

class UI_Configuration : public UI
{
public:
	UI_Configuration();
	~UI_Configuration();

	void Render() override;
};

#endif // !UI_CONFIGURATION_CLASS