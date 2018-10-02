#ifndef UI_CONFIGURATION_CLASS
#define UI_CONFIGURATION_CLASS

#include "UI.h"
#include "Globals.h"

#include <vector>
#include <string>

#define GRAPH_SIZE 50

class UI_Configuration : public UI
{
public:
	UI_Configuration();
	~UI_Configuration();

	void Render() override;

	void store_app_FPS(float value);
	void store_app_ms(float value);
	void store_memory(float value);

	bool conf_depth_test = false;
	bool conf_cull_face = false;
	bool conf_lighting = false;
	bool conf_color_material = false;
	bool conf_texture_2D = false;

	//float width = SCREEN_WIDTH;
	//float height = SCREEN_HEIGHT;

private:
	//Vectors to store the data for the graphs
	std::vector<float> fps_app;
	std::vector<float> ms_app;
	std::vector<float> memory;

	// Calculate average value from vector function and value
	float avg;
	void Calc_avg(std::vector<float> aux);

	std::string fps_text;
};

#endif // !UI_CONFIGURATION_CLASS