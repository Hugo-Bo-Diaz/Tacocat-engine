#include "UI_Configuration.h"
#include "Application.h"

#include "SDL/include/SDL_cpuinfo.h"

UI_Configuration::UI_Configuration()
{
	Enable(true);
	setPos(ImVec2(20, 20));

	fps_app.resize(GRAPH_SIZE);
	ms_app.resize(GRAPH_SIZE);
}

UI_Configuration::~UI_Configuration()
{

}

void UI_Configuration::Render()
{

	bool prev_conf_fullscreen = conf_fullscreen;
	bool prev_conf_resizable = conf_resizable;
	bool prev_conf_borderless = conf_borderless;
	bool prev_conf_fulldesktop = conf_fulldesktop;

	bool prev_conf_depth_test = conf_depth_test;
	bool prev_conf_cull_face = conf_cull_face;
	bool prev_conf_lighting = conf_lighting;
	bool prev_conf_color_material = conf_color_material;
	bool prev_conf_texture_2D = conf_texture_2D;

	int prevwidth = App->window->width;
	int prevheight = App->window->height;

	ImGui::SetNextWindowPos(getPos(), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300,300), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Configuration",&enabled);
	ImGui::Text("Options");
	if (ImGui::CollapsingHeader("Application"))
	{
		//Info from the application
		ImGui::Text(TITLE); ImGui::SameLine();
		ImGui::Text("App name");
		ImGui::Text(ORGANIZATION); ImGui::SameLine();
		ImGui::Text("Organization");
		const int fps = 0;
		//ImGui::SliderInt("MaxFPS", fps, 0, 120);
		//TODO: Make function to set the max fps
		ImGui::Text("Frames Per Second");
		Calc_avg(fps_app);
		char title[25];
		sprintf_s(title, 25, "%.1f FPS", avg);
		ImGui::PlotHistogram("", &fps_app[0], fps_app.size(), 0, title, 0.0f, /*float(fps)*/100.0f, ImVec2(200, 100));

		//TODO: Plot histogram milliseconds
		ImGui::Text("Miliseconds");
		//char title[25];
		//sprintf_s(title, 25, "%.1f ms", avg);
		ImGui::PlotHistogram("", &ms_app[0], ms_app.size(), 0, NULL, 0.0f, /*float(fps)*/20.0f, ImVec2(200, 100));

		//TODO: Plot histogram memory consumption
	}
	if (ImGui::CollapsingHeader("Window"))
	{
		ImGui::Checkbox("Fullscreen",&conf_fullscreen);
		ImGui::Checkbox("Borderless", &conf_borderless);
		ImGui::Checkbox("Resizable", &conf_resizable);
		ImGui::Checkbox("Full desktop", &conf_fulldesktop);
		ImGui::DragFloat("Width", &App->window->width);
		ImGui::DragFloat("Height", &App->window->height);

		SDL_DisplayMode DM;
		SDL_GetCurrentDisplayMode(0, &DM);
		ImGui::Text("Refresh Rate: %d", DM.refresh_rate);

	}
	if (ImGui::CollapsingHeader("File System"))
	{
		//File systems options
	}
	if (ImGui::CollapsingHeader("Input"))
	{
		ImGui::Text("Mouse position: %d, %d", App->input->GetMouseX(), App->input->GetMouseY());
		ImGui::Text("Mouse position: %d, %d", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());
		ImGui::Checkbox("Send inputs to console", &App->input->sendinputs);

	}	
	
	if (ImGui::CollapsingHeader("OpenGL Settings"))
	{
		ImGui::Checkbox("depth_test", &conf_depth_test);
		ImGui::Checkbox("cull_face", &conf_cull_face);
		ImGui::Checkbox("lighting", &conf_lighting);
		ImGui::Checkbox("color_material", &conf_color_material);
		ImGui::Checkbox("texture_2D", &conf_texture_2D);
	}

	if (ImGui::CollapsingHeader("Hardware"))
	{
		ImGui::Text("Total Ram - %d GB", SDL_GetSystemRAM()/1024);
		ImGui::Text("Number of CPU cores - %d", SDL_GetCPUCount());
		if (SDL_HasRDTSC() == SDL_TRUE)
		{ImGui::Text("RDTSC"); ImGui::SameLine();}
		if (SDL_HasSSE() == SDL_TRUE)
		{ImGui::Text("SSE"); ImGui::SameLine();}
		if (SDL_HasSSE2() == SDL_TRUE)
		{ImGui::Text("SSE2"); ImGui::SameLine();}
		if (SDL_HasSSE3() == SDL_TRUE)
		{ImGui::Text("SSE3"); ImGui::SameLine();}
		if (SDL_HasSSE41() == SDL_TRUE)
		{ImGui::Text("SSE41"); ImGui::SameLine();}
		if (SDL_HasSSE42() == SDL_TRUE)
		{ImGui::Text("SSE42"); ImGui::SameLine();}
	}
	ImGui::End();

	if (prev_conf_borderless != conf_borderless)
	{
		App->window->Toggle_Borderless();
	}
	if (prev_conf_fullscreen != conf_fullscreen)
	{
		App->window->Toggle_FullScreen();
	}
	if (prev_conf_resizable != conf_resizable)
	{
		App->window->Toggle_Resizable();
	}
	if (prev_conf_fulldesktop != conf_fulldesktop)
	{
		App->window->Toggle_FullDesktop();
	}
	if (prevwidth != App->window->width || prevheight != App->window->height)
	{
	SDL_SetWindowSize(App->window->window, App->window->width, App->window->height);
	App->renderer3D->OnResize(App->window->width, App->window->height);
	}

	if (prev_conf_depth_test != conf_depth_test)
	{
		if (conf_depth_test)
		{
			App->renderer3D->enable_flag_depth_test();
		}
		else
		{
			App->renderer3D->disable_flag_depth_test();
		}
	}

	if (prev_conf_cull_face != conf_cull_face)
	{
		if (conf_cull_face)
		{
			App->renderer3D->enable_flag_cull_face();
		}
		else
		{
			App->renderer3D->disable_flag_cull_face();
		}
	}

	if (prev_conf_lighting != conf_lighting)
	{
		if (conf_lighting)
		{
			App->renderer3D->enable_flag_lighting();
		}
		else
		{
			App->renderer3D->disable_flag_lighting();
		}
	}

	if (prev_conf_color_material != conf_color_material)
	{
		if (conf_color_material)
		{
			App->renderer3D->enable_flag_color_material();
		}
		else
		{
			App->renderer3D->disable_flag_color_material();
		}
	}

	if (prev_conf_texture_2D != conf_texture_2D)
	{
		if (conf_texture_2D)
		{
			App->renderer3D->enable_flag_texture_2D();
		}
		else
		{
			App->renderer3D->disable_flag_texture_2D();
		}
	}


}

void UI_Configuration::store_app_FPS(float value)
{
	if (fps_app.capacity() != fps_app.size())
	{
		fps_app.push_back(value);
	}
	else
	{
		for (int val = 0; val < (GRAPH_SIZE - 1); val++)
		{
			fps_app[val] = fps_app[val + 1];
		}
		fps_app[fps_app.size() - 1] = value;
	}
}


void UI_Configuration::store_app_ms(float value)
{
	if (ms_app.capacity() != ms_app.size())
	{
		ms_app.push_back(value);
	}
	else
	{
		for (int val = 0; val < (GRAPH_SIZE - 1); val++)
		{
			ms_app[val] = ms_app[val + 1];
		}
		ms_app[ms_app.size() - 1] = value;
	}
}

void UI_Configuration::Calc_avg(std::vector<float> aux) {
	float num_values = 0;
	avg = 0;

	for (int counter = 0; counter < aux.size(); counter++) {
		avg += aux[counter];
		num_values++;
	}

	avg = avg / num_values;
}