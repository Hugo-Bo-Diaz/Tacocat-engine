#ifndef UI_TIME_CLASS
#define UI_TIME_CLASS

#include "UI.h"
#include "GameObject.h"

class UI_Time : public UI
{
public:
	UI_Time();
	~UI_Time();

	void Render() override;

private:
	std::vector<GameObject*> Save;
};

#endif