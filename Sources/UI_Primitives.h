#ifndef UI_Primitives_H
#define UI_Primitives_H

#include "UI.h"
#include "glmath.h"

class UI_Primitives : public UI
{
public:
	UI_Primitives();
	~UI_Primitives();

	void Render() override;

private:
	void Init();

	vec3 pos;
	vec2 pol;
	float size;

	int segments;
	int rings;
};

#endif // !UI_Primitives_H