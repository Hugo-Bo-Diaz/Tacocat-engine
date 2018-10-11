#ifndef UI_PROPERTIES_CLASS
#define UI_PROPERTIES_CLASS

#include "UI.h"

class UI_Properties : public UI
{
public:
	UI_Properties();
	~UI_Properties();

	void Render() override;
};

#endif