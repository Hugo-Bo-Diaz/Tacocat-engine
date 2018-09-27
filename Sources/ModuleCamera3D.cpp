#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	name = "Camera3D";

	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	App->UI->console->AddLog("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	App->UI->console->AddLog("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	vec3 newPos(0,0,0);//TOCHANGE
	float speed = 3.0f * dt;
	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	/*if(App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
	if(App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;*/

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

	Position.Set(newPos.x + Position.x, newPos.y + Position.y, newPos.z + Position.z);
	Reference.Set(newPos.x + Position.x, newPos.y + Position.y, newPos.z + Position.z);

	

	//Position.setX(newPos.x);
	//Position.setY(newPos.y);
	//Position.setZ(newPos.z);
	//Reference += newPos;

	// Mouse motion ----------------

	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		Position -= Reference;

		if(dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if(dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if(Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		Position = Reference + Z * length(Position);
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)//TOCHANGE
{
	this->Position = Position;
	this->Reference = Reference;

	//float3 to_normalize = Position - Reference;
	//float normalizing_factor = sqrt(to_normalize.x * to_normalize.x + to_normalize.y * to_normalize.y + to_normalize.z * to_normalize.z);

	//Z = {to_normalize.x/normalizing_factor,to_normalize.y / normalizing_factor,to_normalize.z / normalizing_factor };


	//to_normalize = { 1.0f * Z.z - 0.0f * Z.y, 0.0f * Z.x - 0.0f * Z.z, 0.0f * Z.y - 1.0f * Z.x };
	//normalizing_factor = sqrt(to_normalize.x * to_normalize.x + to_normalize.y * to_normalize.y + to_normalize.z * to_normalize.z);//cross product

	//X = {to_normalize.x / normalizing_factor, to_normalize.y / normalizing_factor, to_normalize.z / normalizing_factor };//normalized

	//Y = {	Z.y * X.z - Z.z * X.y,
	//		Z.z * X.x - Z.x * X.z,
	//		Z.x * X.y - Z.y * X.x};

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
//void ModuleCamera3D::LookAt( const float3 &Spot)
//{
//	Reference = Spot;
//
//	//float3x3::LookAt(, {});
//
//	Z = normalize(Position - Reference);
//	X = normalize(cross(float3(0.0f, 1.0f, 0.0f), Z));
//	Y = cross(Z, X);
//
//	CalculateViewMatrix();
//}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;

	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	//ViewMatrix = mat4x4(	X.x,				Y.x,				Z.x,				0.0f,
	//						X.y,				Y.y,				Z.y,				0.0f,
	//						X.z,				Y.z,				Z.z,				0.0f, 
	//						(X.x*Position.x+X.y*Position.y+X.z*Position.z), (Y.x*Position.x + Y.y*Position.y + Y.z*Position.z), (Z.x*Position.x + Z.y*Position.y + Z.z*Position.z),	1.0f);
	//ViewMatrixInverse = ViewMatrix.Inverted();

	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f,
		X.y, Y.y, Z.y, 0.0f,
		X.z, Y.z, Z.z, 0.0f,
		-dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);

	ViewMatrixInverse = inverse(ViewMatrix);

}