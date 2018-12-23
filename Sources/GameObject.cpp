#include "GameObject.h"
#include "Component.h"
#include "MaterialComponent.h"
#include "MeshComponent.h"
#include "imgui-docking/imgui.h"
#include "Application.h"

void GameObject::Update(float dt)
{
	//glScalef(t->scaling.x,t->scaling.y,t->scaling.z);
	//glRotatef(RadToDeg(t->angle), t->rotation_angle.x, t->rotation_angle.y, t->rotation_angle.z);
	//glTranslatef(t->position.x,t->position.y,t->position.z);
	//glMultMatrix(global_transform.transposed.ptr)
	if (GetTransformComponent() !=  nullptr)
	{	
		glPushMatrix();
		float4x4 globaltransformmatrix = GetTransformComponent()->transform_global;
		float* matrixtopush = (float*)globaltransformmatrix.Transposed().v;
		glMultMatrixf(matrixtopush);
	}
	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		(*it)->Update(dt);
	}
	if (GetTransformComponent() != nullptr)
		glPopMatrix();

	for (std::list<GameObject*>::iterator it = children.begin(); it != children.end(); it++)
	{
		(*it)->Update(dt);
	}
	float3 one = GetBoundingBox().minPoint;
	float3 two = GetBoundingBox().maxPoint;
	float3 three = float3(-50, -50, -50);
	float3 four = float3(50, 50, 50);

	if(one.x != three.x && one.y != three.y &&one.z != three.z && 
		two.x != four.x && two.y != four.y && two.z != four.z)
	{
		bounding_box = GetBoundingBox();
	}


}

uint GameObject::GetTexture(uint index)
{
	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->type == MATERIAL && ((Component_Material*)(*it))->material->material_index == index)
		{
			return ((Component_Material*)(*it))->material->texture_buffer_id;//the first texture it finds in its components is given
		}
	}

	return 0;
}

AABB GameObject::GetBoundingBox()
{
	AABB box;
	box.minPoint = float3(-50,-50,-50);
	box.maxPoint = float3(50, 50, 50);
	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->type == MESH)
		{
			box = ((Component_Mesh*)(*it))->bounding_box;
		}
	}

	//for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
	//{
	//	if ((*it)->type == MESH)
	//	{
	//		if (((Component_Mesh*)(*it))->bounding_box.minPoint.x < box.minPoint.x)
	//			box.minPoint.x = ((Component_Mesh*)(*it))->bounding_box.minPoint.x;
	//		if (((Component_Mesh*)(*it))->bounding_box.minPoint.y < box.minPoint.y)
	//			box.minPoint.y = ((Component_Mesh*)(*it))->bounding_box.minPoint.y;
	//		if (((Component_Mesh*)(*it))->bounding_box.minPoint.z < box.minPoint.z)
	//			box.minPoint.z = ((Component_Mesh*)(*it))->bounding_box.minPoint.z;

	//		if (((Component_Mesh*)(*it))->bounding_box.maxPoint.x > box.maxPoint.x)
	//			box.maxPoint.x = ((Component_Mesh*)(*it))->bounding_box.maxPoint.x;
	//		if (((Component_Mesh*)(*it))->bounding_box.maxPoint.y > box.maxPoint.y)
	//			box.maxPoint.y = ((Component_Mesh*)(*it))->bounding_box.maxPoint.y;
	//		if (((Component_Mesh*)(*it))->bounding_box.maxPoint.z > box.maxPoint.z)
	//			box.maxPoint.z = ((Component_Mesh*)(*it))->bounding_box.maxPoint.z;
	//	}
	//}
	//if (children.size() > 0)
	//{
	//	for (std::list<GameObject*>::iterator it_c = children.begin(); it_c != children.end(); it_c++)
	//	{
	//		AABB tmp = (*it_c)->GetBoundingBox();

	//		if (tmp.minPoint.x < box.minPoint.x)
	//			box.minPoint.x = tmp.minPoint.x;
	//		if (tmp.minPoint.y < box.minPoint.y)
	//			box.minPoint.y = tmp.minPoint.y;
	//		if (tmp.minPoint.z < box.minPoint.z)
	//			box.minPoint.z = tmp.minPoint.z;

	//		if (tmp.maxPoint.x > box.maxPoint.x)
	//			box.maxPoint.x = tmp.maxPoint.x;
	//		if (tmp.maxPoint.y > box.maxPoint.y)
	//			box.maxPoint.y = tmp.maxPoint.y;
	//		if (tmp.maxPoint.z > box.maxPoint.z)
	//			box.maxPoint.z = tmp.maxPoint.z;
	//	}
	//}
	return box;
}

void GameObject::DrawBoundingBox()
{
	float3 c;
	if (Iselected())
	{
		c = float3(0, 0, 0);
	}
	else
	{
		c = float3(1, 1, 1);
	}

	glDisable(GL_LIGHTING);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_LINES);

	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);
	glColor3f(c.x, c.y, c.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);
	glColor3f(c.x, c.y, c.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);
	glColor3f(c.x, c.y, c.z);

	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);
	glColor3f(c.x, c.y, c.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.minPoint.z);
	glColor3f(c.x, c.y, c.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);
	glColor3f(c.x, c.y, c.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.minPoint.z);
	glColor3f(c.x, c.y, c.z);

	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.minPoint.z);
	glColor3f(c.x, c.y, c.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.minPoint.y, bounding_box.minPoint.z);
	glColor3f(c.x, c.y, c.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.minPoint.z);
	glColor3f(c.x, c.y, c.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.maxPoint.y, bounding_box.minPoint.z);
	glColor3f(c.x, c.y, c.z);

	glVertex3f(bounding_box.minPoint.x, bounding_box.minPoint.y, bounding_box.minPoint.z);
	glColor3f(c.x, c.y, c.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);
	glColor3f(c.x, c.y, c.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.maxPoint.y, bounding_box.minPoint.z);
	glColor3f(c.x, c.y, c.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);
	glColor3f(c.x, c.y, c.z);

	glVertex3f(bounding_box.minPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);
	glColor3f(c.x, c.y, c.z);

	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);
	glColor3f(c.x, c.y, c.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);
	glColor3f(c.x, c.y, c.z);

	glVertex3f(bounding_box.minPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);
	glColor3f(c.x, c.y, c.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.minPoint.y, bounding_box.minPoint.z);
	glColor3f(c.x, c.y, c.z);

	glVertex3f(bounding_box.minPoint.x, bounding_box.minPoint.y, bounding_box.minPoint.z);
	glColor3f(c.x, c.y, c.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.minPoint.z);
	glColor3f(c.x, c.y, c.z);

	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.minPoint.z);
	glColor3f(c.x, c.y, c.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);
	glColor3f(c.x, c.y, c.z);

	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);
	glColor3f(c.x, c.y, c.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);
	glColor3f(c.x, c.y, c.z);

	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);

	glVertex3f(bounding_box.minPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.maxPoint.y, bounding_box.minPoint.z);

	glVertex3f(bounding_box.minPoint.x, bounding_box.maxPoint.y, bounding_box.minPoint.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.minPoint.z);

	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.minPoint.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);


	glEnd();
	glEnable(GL_LIGHTING);

	for (std::list<GameObject*>::iterator it = children.begin(); it != children.end(); it++)
	{
		(*it)->DrawBoundingBox();
	}
}

void GameObject::AddComponent(Component* comp)
{
	comp->parent = this;
	components.push_back(comp);

}

void GameObject::AddChild(GameObject * child)
{
	child->parent = this;
	child->parent_uid = UID;
	children.push_back(child);

}

GameObject::GameObject()
{
	//NOTATION day-month-6 number random

	//it should be unique unless there are more than 1.000.000 objects added in the scene in the span of a day

	//this uid can be decoded to get when approximately was created

	UID = 0;

	time_t theTime = time(NULL);
	struct tm *aTime = localtime(&theTime);

	uint hours = aTime->tm_hour;//2 digits
	uint day = aTime->tm_mday; // 2 digits
	uint month = aTime->tm_mon + 1;//from 0-11 to 1-12  //2 digits

	uint random = App->random_int(0,999999);

	UID += day *   100000000;
	UID += month * 1000000;
	UID += random ;

	bounding_box.minPoint = float3(-1,-1,-1);
	bounding_box.maxPoint = float3(1, 1, 1);


	App->scene_controller->current_scene->number_of_gameobjects++;
	name = "gameobject " ;
	name += std::to_string(App->scene_controller->current_scene->number_of_gameobjects);

}

GameObject::~GameObject()
{
	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		delete(*it);
	}
	components.clear();

}


void GameObject::Hierarchy()
{
	int flags = 0;

	if (children.empty()) flags |= ImGuiTreeNodeFlags_Leaf;
	if (App->scene_controller->current_scene->spookamera->selected != nullptr)
	{
		if (App->scene_controller->current_scene->spookamera->selected->UID == UID)
		{
			flags |= ImGuiTreeNodeFlags_Selected;
			selected = true;
		}
		else
		{
			selected = false;
		}
	}

	if (ImGui::TreeNodeEx(name.c_str(), flags))
	{
		if (ImGui::IsItemClicked())
			App->scene_controller->current_scene->spookamera->selected = this;

		for (std::list<GameObject*>::iterator it = children.begin(); it != children.end(); it++)
		{
			(*it)->Hierarchy();
		}
		ImGui::TreePop();
	}
}

void GameObject::Save(rapidjson::Document* d, rapidjson::Value* v)
{
	rapidjson::Document::AllocatorType& all = d->GetAllocator();

	rapidjson::Value module_obj(rapidjson::kObjectType);

	module_obj.AddMember("UID", UID, all);
	if (parent != nullptr)
		module_obj.AddMember("parentUID", parent->UID, all);

	rapidjson::Value components_node(rapidjson::kObjectType);
	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		(*it)->Save_Component(d, &components_node);
	}
	module_obj.AddMember("component", components_node, all);

	for (std::list<GameObject*>::iterator it = children.begin(); it != children.end(); it++)
	{
		(*it)->Save(d,v);
	}

	v->AddMember("OBJECT", module_obj, all);


}

std::vector<Component_Mesh*>* GameObject::GetAllMeshes(std::vector<Component_Mesh*>& ret)
{

	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->type == MESH)
		{
			ret.push_back((Component_Mesh*)*it);
		}
	}

	for (std::list<GameObject*>::iterator it = children.begin(); it != children.end(); it++)
	{
		(*it)->GetAllMeshes(ret);
	}

	return &ret;
}

void GameObject::Properties()
{
	//ImGui::Text("%s", name);
	ImGui::Checkbox("Static", &static_object);

	if (!children.empty()) {
		for (std::list<GameObject*>::iterator it = children.begin(); it != children.end(); it++)
		{
			(*it)->static_object = static_object;
		}
	}
	
	if(static_object && !App->scene_controller->current_scene->tree->CheckSpooktree(this))
		App->scene_controller->current_scene->tree->IntoSpooktree(this);
	
}

Component_Transform * GameObject::GetTransformComponent()
{
	if (!components.empty())
	{
		for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
		{
			if ((*it)->type == TRANSFORM)
			{
				return ((Component_Transform*)*it);
			}
		}
	}
	return nullptr;
}

void GameObject::CalculateAllTransformMatrices(float4x4 previousmatrix)
{
	Component_Transform* t = GetTransformComponent();
	float4x4 newmatrix;

	if (t != nullptr)//calculate the matrix to give below
	{
		t->Calculate_Global_Matrix(previousmatrix);
		newmatrix = t->transform_global;
	}
	else
	{
		newmatrix = previousmatrix;
	}

	for (std::list<GameObject*>::iterator it = children.begin(); it != children.end(); it++)
	{
		(*it)->CalculateAllTransformMatrices(newmatrix);
	}
}
