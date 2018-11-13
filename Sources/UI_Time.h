#ifndef UI_TIME_CLASS
#define UI_TIME_CLASS

#include "UI.h"

class UI_Time : public UI
{
public:
	UI_Time();
	~UI_Time();

	void Render() override;
};

#endif