#include "CameraComponent.h"
#include "Application.h"


Component_Camera::Component_Camera()
{
	CalculateViewMatrix();

	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	Position = float3(0.0f, 0.0f,10.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);

	//frustum.horizontalFov = fov_x;
	frustum.horizontalFov = DegToRad(106.67);
	frustum.verticalFov = DegToRad(60);
	frustum.nearPlaneDistance = 0.5;
	frustum.farPlaneDistance = 500;
	//frustum.orthographicWidth = 1000;
	//frustum.orthographicHeight = 800;
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.up = float3(0, 1, 0);

	frustum.SetWorldMatrix(float3x4::identity);

	//frustum.Translate(float3(0,0,0));
	//frustum.pos = Position;
	frustum.pos = float3(0, 0, 0);

	frustum.Translate(Position);

	Generate_frustum();
}

void Component_Camera::Update(float dt)
{
	frustum.pos = Position;

	Generate_frustum();

	float real_speed = speed * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		real_speed = speed * 3 * dt;

	speed = 3;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;


	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) RotateFrustum_Yaxis(0.5);
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)  RotateFrustum_Yaxis(-0.5);

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)  RotateFrustum_XZaxis(0.5);
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)  RotateFrustum_XZaxis(-0.5);

	//if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) frustum.pos.z +=5;
	//if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) frustum.pos.x += 5;

	Position.Set(newPos.x + Position.x, newPos.y + Position.y, newPos.z + Position.z);
	Reference.Set(newPos.x + Position.x, newPos.y + Position.y, newPos.z + Position.z);

	newPos = { 0,0,0 };

	int dx = -App->input->GetMouseXMotion()* sensibility;
	int dy = -App->input->GetMouseYMotion()* sensibility;

	float3 pos_now = frustum.pos;//saving the position of the frustum

	frustum.pos = float3(0, 0, 0);
	frustum.Transform((transform_y * transform_xz).Inverted());//we revert the changes so that they don't stack up

	frustum.pos = float3(0, 0, 0);//setting it to 0 so that the transforms apply right

	transform_y =//calculating transform matrices
	{ cos(DegToRad(angle_XZ)),		0,	sin(DegToRad(angle_XZ)),
		0,							1,	0,
		-sin(DegToRad(angle_XZ)),	0,	cos(DegToRad(angle_XZ)) };

	transform_xz =
	{ 1,	0,		0,
		0,	cos(DegToRad(angle_Y)),-sin(DegToRad(angle_Y)),
		0,	sin(DegToRad(angle_Y)),cos(DegToRad(angle_Y)) };

	float3x3 transform = transform_y * transform_xz;//final transform matrix, first spins around x axis then around y

	frustum.Transform(transform);//we apply transformations
	frustum.pos = pos_now;//we move the transformed frustum to its position
	Generate_frustum();
	Draw_frustum();//we draw it

	frustum.pos = pos_now;//we move the transformed frustum to its position


	App->scene_controller->current_scene->Camera->GetTransformComponent()->position.x = Position.x;
	App->scene_controller->current_scene->Camera->GetTransformComponent()->position.y = Position.y;
	App->scene_controller->current_scene->Camera->GetTransformComponent()->position.z = Position.z;
	App->scene_controller->current_scene->Camera->GetTransformComponent()->rotation = Quat::FromEulerXYZ(angle_Y,angle_XZ,0);
	App->scene_controller->current_scene->Camera->GetTransformComponent()->Caluculate_Local_Matrix();
	App->UI->console->AddLog("%f, %f, %f", Position.x, Position.y, Position.z);


	//// Mouse picking

	//first we need the two points of the ray, we need the position in the near and far plane of the frustum
	//so if the mouse is in the middle it'll be halfway of the frustum plane

	float mousex, mousey;
	mousex = App->input->GetMouseX();
	mousey = App->input->GetMouseY();
	//
	float percent_x = (mousex / App->window->width )*2 - 1;
	float percent_y = (mousey / App->window->height)*2 - 1;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glColor3f(0, 0, 1);
	glBegin(GL_LINES);

	glVertex3fv((GLfloat*)&ray.a[0]);
	glVertex3fv((GLfloat*)&ray.b[0]);

	glEnd();

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		ray = frustum.UnProjectLineSegment(percent_x, -percent_y);
		Ray r; 
		r.dir = ray.Dir();
		r.pos = frustum.pos;
	
		std::vector<GameObject*> objects_hit;

		for (std::vector<GameObject*>::iterator it = App->scene_controller->current_scene->GameObjects.begin(); it != App->scene_controller->current_scene->GameObjects.end(); it++)
		{
			AABB newAABB = (*it)->GetBoundingBox(); 
			//if ((*it)->GetTransform() != nullptr)
			//{
			//	newAABB.Translate(float3((*it)->GetTransform()->position.x, (*it)->GetTransform()->position.y, (*it)->GetTransform()->position.z));
			//}

			if(r.Intersects(newAABB))
			{
				(*it)->selected = true;
				//App->UI->console->AddLog("HA %d, (%f %f %f) (%f %f %f)", (*it)->UID, (*it)->GetBoundingBox().minPoint.x, (*it)->GetBoundingBox().minPoint.y, (*it)->GetBoundingBox().minPoint.z, (*it)->GetBoundingBox().maxPoint.x, (*it)->GetBoundingBox().maxPoint.y, (*it)->GetBoundingBox().maxPoint.z);
				objects_hit.push_back(*it);
			}
			else
			{
				(*it)->selected = false;
			}
		}

		GameObject* object_found = nullptr;
		bool found_something = false;
		std::vector<Component_Mesh*> meshesinscene;
		for (std::vector<GameObject*>::iterator it_AABB = objects_hit.begin(); it_AABB != objects_hit.end(); it_AABB++)
		{
			//here we should check geometry if something is hit directly then the loop will exit
			std::vector<Component_Mesh*> list;
			(*it_AABB)->GetAllMeshes(list);

			if (found_something)
			{
				//tell the object it has been selected
				break;
			}
		}
		if (objects_hit.size() >0 && object_found == nullptr)
		{
			//App->UI->console->AddLog("this ain't it chief");
		}

		
	}


	//// Mouse motion ----------------

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT || (App->input->GetKey(SDL_SCANCODE_LALT) && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT))
	{
		if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			Reference.x = 0;
			Reference.y = 0;
			Reference.z = 0;
		}

		float Sensitivity = 0.01f;

		Position -= Reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			RotateFrustum_XZaxis(RadToDeg(DeltaX));

			X = Rotate(X, DeltaX, float3(0.0f, 1.0f, 0.0f));
			Y = Rotate(Y, DeltaX, float3(0.0f, 1.0f, 0.0f));
			Z = Rotate(Z, DeltaX, float3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			RotateFrustum_Yaxis(RadToDeg(DeltaY));

			Y = Rotate(Y, DeltaY, X);
			Z = Rotate(Z, DeltaY, X);

			if (Y.y < 0.0f)
			{
				Z = float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = Z.Cross(X);
			}
		}
		Position = Reference + Z * Position.Length();
	}


	CalculateViewMatrix();
}

void Component_Camera::Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;


	float3 new_vec1 = Position-Reference;
	Z = new_vec1.Normalized();
	float3 new_vec2 = float3(0.0f,1.0f,0.0f).Cross(Z);
	X = new_vec2.Normalized();
	Y = Z.Cross(X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();

}

void Component_Camera::LookAt(const float3 &Spot)
{

}

void Component_Camera::Move(const float3 &Movement)
{
	Position += Movement;

	Reference += Movement;

	CalculateViewMatrix();
}

float* Component_Camera::GetViewMatrix()
{

	return &ViewMatrix[0][0];

}

void Component_Camera::CalculateViewMatrix()
{
	ViewMatrix = float4x4(X.x, Y.x, Z.x, 0.0f,
		X.y, Y.y, Z.y, 0.0f,
		X.z, Y.z, Z.z, 0.0f,
		-X.Dot(Position), -Y.Dot(Position), -Z.Dot(Position), 1.0f);

	ViewMatrixInverse = ViewMatrix.Inverted();
		//inverse(ViewMatrix);
}

void Component_Camera::RotateFrustum_Yaxis(float angle)
{

	angle_Y += angle;

	if (angle_Y > 90)
	{
		angle_Y = 90;
	}

	if (angle_Y < -90)
	{
		angle_Y = -90;
	}
}

void Component_Camera::RotateFrustum_XZaxis(float angle)
{
	angle_XZ += angle;
}

void Component_Camera::Generate_frustum()
{
	frustum.GetCornerPoints(vert);
}

void Component_Camera::Draw_frustum()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);


	glVertex3fv((GLfloat*)&vert[1]);
	glVertex3fv((GLfloat*)&vert[5]);
	glVertex3fv((GLfloat*)&vert[7]);
	glVertex3fv((GLfloat*)&vert[3]);

	//glVertex3fv((GLfloat*)&vert[3]);
	//glVertex3fv((GLfloat*)&vert[1]);

	glVertex3fv((GLfloat*)&vert[4]);
	glVertex3fv((GLfloat*)&vert[0]);
	glVertex3fv((GLfloat*)&vert[2]);
	glVertex3fv((GLfloat*)&vert[6]);

	//glVertex3fv((GLfloat*)&vert[6]);
	//glVertex3fv((GLfloat*)&vert[4]);

	glVertex3fv((GLfloat*)&vert[5]);
	glVertex3fv((GLfloat*)&vert[4]);
	glVertex3fv((GLfloat*)&vert[6]);
	glVertex3fv((GLfloat*)&vert[7]);
	
	//glVertex3fv((GLfloat*)&vert[7]);
	//glVertex3fv((GLfloat*)&vert[5]);

	glVertex3fv((GLfloat*)&vert[0]);
	glVertex3fv((GLfloat*)&vert[1]);
	glVertex3fv((GLfloat*)&vert[3]);
	glVertex3fv((GLfloat*)&vert[2]);

	//glVertex3fv((GLfloat*)&vert[2]);
	//glVertex3fv((GLfloat*)&vert[6]);
	
	glVertex3fv((GLfloat*)&vert[3]);
	glVertex3fv((GLfloat*)&vert[7]);
	glVertex3fv((GLfloat*)&vert[6]);
	glVertex3fv((GLfloat*)&vert[2]);

	//glVertex3fv((GLfloat*)&vert[2]);
	//glVertex3fv((GLfloat*)&vert[0]);

	glVertex3fv((GLfloat*)&vert[0]);
	glVertex3fv((GLfloat*)&vert[4]);
	glVertex3fv((GLfloat*)&vert[5]);
	glVertex3fv((GLfloat*)&vert[1]);

	glEnd();

}

//float4x4 Component_Camera::projection_matrix_todelete(float fovy, float aspect, float n, float f)
//{
//	float4x4 Perspective;
//
//	float coty = 1.0f / tan(fovy * (float)pi / 360.0f);
//
//	Perspective[0][0] = coty / aspect;					//0 0
//	Perspective[1][1] = coty;							//1 1
//	Perspective[2][2] = (n + f) / (n - f);				//2 2
//	Perspective[3][2] = -1.0f;							//3 2
//	Perspective[2][3] = 2.0f * n * f / (n - f);			//2 3
//	Perspective[3][3] = 0.0f;							//3 3
//
//	return Perspective;
//}


void Component_Camera::Save_Component(rapidjson::Document* d, rapidjson::Value* v)
{
	rapidjson::Document::AllocatorType& all = d->GetAllocator();

	rapidjson::Value module_obj(rapidjson::kObjectType);

	module_obj.AddMember("angleXZ", angle_XZ, all);
	module_obj.AddMember("angleY", angle_Y, all);

	module_obj.AddMember("position_x", Position.x, all);
	module_obj.AddMember("position_y", Position.y, all);
	module_obj.AddMember("position_z", Position.z, all);

	module_obj.AddMember("reference_x", Reference.x, all);
	module_obj.AddMember("reference_y", Reference.y, all);
	module_obj.AddMember("reference_z", Reference.z, all);

	v->AddMember("CAMERA", module_obj, all);
}

void Component_Camera::Load_Component(rapidjson::Value& v)
{

}