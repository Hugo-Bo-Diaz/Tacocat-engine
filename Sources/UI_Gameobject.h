#ifndef UI_GAMEOBJECT_CLASS
#define UI_GAMEOBJECT_CLASS

#include "UI.h"

class UI_GameObject : public UI
{
public:
	UI_GameObject();
	~UI_GameObject();

	void Render() override;
};

#endif // !UI_GAMEOBJECT_CLASS