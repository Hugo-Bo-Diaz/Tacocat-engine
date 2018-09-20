#ifndef UI_CONFIGURATION_CLASS
#define UI_CONFIGURATION_CLASS

#include "UI.h"

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

private:
	//Vectors to store the data for the graphs
	std::vector<float> fps_app;

	// Calculate average value from vector function and value
	float avg;
	void Calc_avg(std::vector<float> aux);

	std::string fps_text;
};

#endif // !UI_CONFIGURATION_CLASS