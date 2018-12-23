#include "ModuleUI.h"
#include "Application.h"

#include "TransformComponent.h"


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
	hierarchy = new UI_GameObject();
	time = new UI_Time();

	UI_Elements.push_back(about);
	UI_Elements.push_back(config);
	UI_Elements.push_back(console);
	UI_Elements.push_back(primitives);
	UI_Elements.push_back(properties);
	UI_Elements.push_back(hierarchy);
	UI_Elements.push_back(time);

	return ret;
}

update_status ModuleUI::Update(float dt)
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	// Docking window ---

	static bool opt_fullscreen_persistant = true;
	static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_PassthruDockspace;
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

	ImGuiStyle& style =  ImGui::GetStyle();

	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05882352941, 0.05882352941, 0.05882352941,1);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.03921568627, 0.03921568627, 0.03921568627, 1);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.05882352941, 0.05882352941, 0.05882352941, 1);
	// Test window ------
	//if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	//	draw_demo = !draw_demo;

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
			if (ImGui::MenuItem("Hierarchy"))
				hierarchy->Enable(!hierarchy->isEnabled());
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

	App->fsys->DrawUI();

	DrawGuizmoUI();

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

void ModuleUI::DrawGuizmoUI()
{

	if (draw_guizmo)
	{
		ImGuizmo::BeginFrame();
		float4x4 projection4x4;
		float4x4 view4x4;

		glGetFloatv(GL_MODELVIEW_MATRIX, (float*)view4x4.v);
		glGetFloatv(GL_PROJECTION_MATRIX, (float*)projection4x4.v);

		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

		if ((Component_Transform*)App->scene_controller->GetMainCamera()->selected != nullptr)
		{
			Component_Transform* transform = (Component_Transform*)App->scene_controller->GetMainCamera()->selected->GetTransformComponent();
			Component_Transform* original;
			Component_Transform aux;
			if (transform == nullptr)
				return;

			switch (gizmo_operation)
			{
			case ImGuizmo::OPERATION::TRANSLATE:
				aux.rotation = transform->rotation;
				aux.position = transform->position;
				break;
			case ImGuizmo::OPERATION::ROTATE:
				aux.position = transform->position;
				aux.rotation = transform->rotation;
				break;
			case ImGuizmo::OPERATION::SCALE:
				aux.position = transform->position;
				aux.rotation = transform->rotation;
				aux.scaling = transform->scaling;
				break;
			default:
				break;
			}

			aux.Caluculate_Local_Matrix();
			float4x4 mat = aux.transform_local;
			mat.Transpose();
			ImGuizmo::Manipulate((float*)view4x4.v, (float*)projection4x4.v, gizmo_operation, gizmo_mode, (float*)mat.v);
			if (ImGuizmo::IsUsing())
			{
				float3 new_pos = float3::zero;
				float3 new_rot = float3::zero;
				float3 new_scale = float3::zero;
				mat.Transpose();
				switch (gizmo_operation)
				{
				case ImGuizmo::OPERATION::TRANSLATE:
					transform->position.x = mat.TranslatePart().x;
					transform->position.y = mat.TranslatePart().y;
					transform->position.z = mat.TranslatePart().z;
					break;
				case ImGuizmo::OPERATION::ROTATE:
					new_rot.x = mat.RotatePart().ToEulerXYZ().x;
					new_rot.y = mat.RotatePart().ToEulerXYZ().y;
					new_rot.z = mat.RotatePart().ToEulerXYZ().z;
					transform->rotation = (Quat::FromEulerXYZ(new_rot.x, new_rot.y, new_rot.z));
					break;
				case ImGuizmo::OPERATION::SCALE:
					transform->scaling.x = mat.GetScale().x;
					transform->scaling.y = mat.GetScale().y;
					transform->scaling.z = mat.GetScale().z;
					break;
				default:
					break;
				}
				transform->Caluculate_Local_Matrix();
				//transform->GlobalToLocal();
			}
		}
	}
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