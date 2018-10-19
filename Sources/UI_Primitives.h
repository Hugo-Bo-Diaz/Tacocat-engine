#ifndef UI_Primitives_H
#define UI_Primitives_H

#include "UI.h"
#include "MathGeoLib/MathGeoLib.h"
class NOTprimitive;

class UI_Primitives : public UI
{
public:
	UI_Primitives();
	~UI_Primitives();

	void Render() override;

private:
	void Init();

	float3 pos;
	float3 pol;
	float size;

	int segments;
	int rings;

	NOTprimitive* primitive = nullptr;
};

#endif // !UI_Primitives_H