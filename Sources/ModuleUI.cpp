#include "ModuleUI.h"
#include "Application.h"

#include "imgui-docking/imgui.h"
#include "imgui-docking/imgui_impl_sdl.h"
#include "imgui-docking/imgui_impl_opengl2.h"

ModuleUI::ModuleUI(bool start_enabled) : Module( start_enabled)
{
	name = "UI";
}

ModuleUI::~ModuleUI()
{

}

bool ModuleUI::Init()
{
	CONSOLE_LOG("Initiating UI module");
	bool ret = true;

	CONSOLE_LOG("Initiating dear ImGui");
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();

	CONSOLE_LOG("Creating UI components")
	draw_demo = false;
	about = new UI_About();
	config = new UI_Configuration();
	console = new UI_Console();
	primitives = new UI_Primitives();
	properties = new UI_Properties();

	UI_Elements.push_back(about);
	UI_Elements.push_back(config);
	UI_Elements.push_back(console);
	UI_Elements.push_back(primitives);

	return ret;
}

update_status ModuleUI::Update(float dt)
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	// Docking window ---

	static bool opt_fullscreen_persistant = true;
	static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
	bool opt_fullscreen = opt_fullscreen_persistant;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		opt_flags = ImGuiDockNodeFlags_PassthruDockspace;
	}

	// When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (opt_flags & ImGuiDockNodeFlags_PassthruDockspace)
		ImGui::SetNextWindowBgAlpha(0.0f);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Scene", &draw_menu, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Dockspace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
	}

	// Test window ------
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		draw_demo = !draw_demo;

	// Draw menu bar ----
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File", &draw_menu))
		{
			if (ImGui::MenuItem("Save"))
			{
				App->SaveConfig("config.json");
			}
			if (ImGui::MenuItem("Load"))
			{
				App->LoadConfig("config.json");
			}
			if (ImGui::MenuItem("Options"))
			{
			}
			if (ImGui::MenuItem("Exit"))
			{
				App->Close();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Configuration"))
				config->Enable(!config->isEnabled());
			if (ImGui::MenuItem("Console"))
				console->Enable(!console->isEnabled());
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("GameObject"))
		{
			if (ImGui::MenuItem("Primitives"))
				primitives->Enable(!primitives->isEnabled());
			if (ImGui::MenuItem("Model Properties"))
				properties->Enable(!properties->isEnabled());
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About"))
				about->Enable(!about->isEnabled());

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (draw_demo)
		ImGui::ShowDemoWindow(&draw_demo);
	
	for (std::list<UI*>::iterator it = UI_Elements.begin(); it != UI_Elements.end(); ++it)
	{
		if ((*it)->isEnabled())
		{
			(*it)->Render();
		}
	}

	ImGui::End();

	return UPDATE_CONTINUE;
}

bool ModuleUI::CleanUp()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ModuleUI::Draw()
{
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

}

void ModuleUI::send_FPSdata(float FPS, float ms)
{
	config->store_app_FPS(FPS);
	config->store_app_ms(ms);
}