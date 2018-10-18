//#include "Globals.h"
//#include "Application.h"
//#include "ModuleCamera3D.h"
//
//ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
//{
//	name = "Camera3D";
//
//	CalculateViewMatrix();
//
//	X = vec3(1.0f, 0.0f, 0.0f);
//	Y = vec3(0.0f, 1.0f, 0.0f);
//	Z = vec3(0.0f, 0.0f, 1.0f);
//
//	Position = vec3(0.0f, 0.0f, 5.0f);
//	Reference = vec3(0.0f, 0.0f, 0.0f);
//}
//
//ModuleCamera3D::~ModuleCamera3D()
//{}
//
//// -----------------------------------------------------------------
//bool ModuleCamera3D::Start()
//{
//	App->UI->console->AddLog("Setting up the camera");
//	bool ret = true;
//
//	newPos = { 0,0,0 };
//
//	return ret;
//}
//
//// -----------------------------------------------------------------
//bool ModuleCamera3D::CleanUp()
//{
//	App->UI->console->AddLog("Cleaning camera");
//
//	return true;
//}
//
//// -----------------------------------------------------------------
//update_status ModuleCamera3D::Update(float dt)
//{
//	// Implement a debug camera with keys and mouse
//	// Now we can make this movememnt frame rate independant!
//
//	float real_speed = speed * dt;
//	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
//		real_speed =speed*2 * dt;
//
//	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
//	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;
//
//
//	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
//	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;
//
//	Position.Set(newPos.x + Position.x, newPos.y + Position.y, newPos.z + Position.z);
//	Reference.Set(newPos.x + Position.x, newPos.y + Position.y, newPos.z + Position.z);
//
//	newPos = { 0,0,0 };
//
//
//	// Mouse motion ----------------
//
//	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT || (App->input->GetKey(SDL_SCANCODE_LALT) && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT))
//	{
//		if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
//		{
//			Reference = (0,0,0);
//		}
//
//		int dx = -App->input->GetMouseXMotion() * sensibility;
//		int dy = -App->input->GetMouseYMotion() * sensibility;
//
//		float Sensitivity = 0.25f;
//
//		Position -= Reference;
//
//		if(dx != 0)
//		{
//			float DeltaX = (float)dx * Sensitivity;
//
//			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
//			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
//			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
//		}
//
//		if(dy != 0)
//		{
//			float DeltaY = (float)dy * Sensitivity;
//
//			Y = rotate(Y, DeltaY, X);
//			Z = rotate(Z, DeltaY, X);
//
//			if(Y.y < 0.0f)
//			{
//				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
//				Y = cross(Z, X);
//			}
//		}
//
//		Position = Reference + Z * length(Position);
//	}
//	
//	// Recalculate matrix -------------
//	CalculateViewMatrix();
//
//	return UPDATE_CONTINUE;
//}
//
//// -----------------------------------------------------------------
//void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)//TOCHANGE
//{
//	this->Position = Position;
//	this->Reference = Reference;
//
//
//
//	Z = normalize(Position - Reference);
//	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
//	Y = cross(Z, X);
//
//	if(!RotateAroundReference)
//	{
//		this->Reference = this->Position;
//		this->Position += Z * 0.05f;
//	}
//
//	CalculateViewMatrix();
//}
//
//
//
//// -----------------------------------------------------------------
//void ModuleCamera3D::Move(const vec3 &Movement)
//{
//	Position += Movement;
//
//	Reference += Movement;
//
//	CalculateViewMatrix();
//}
//
//// -----------------------------------------------------------------
//float* ModuleCamera3D::GetViewMatrix()
//{
//	return &ViewMatrix;
//}
//
//// -----------------------------------------------------------------
//void ModuleCamera3D::CalculateViewMatrix()
//{
//
//	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f,
//		X.y, Y.y, Z.y, 0.0f,
//		X.z, Y.z, Z.z, 0.0f,
//		-dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
//
//	ViewMatrixInverse = inverse(ViewMatrix);
//
//}
//
//void ModuleCamera3D::Configuration()
//{
//
//	if (ImGui::CollapsingHeader("Camera settings"))
//	{
//		ImGui::SliderFloat("Camera speed", &speed, 0 , 20);
//		ImGui::SliderFloat("Camera sensibility", &sensibility, 1,5);
//	}
//
//}