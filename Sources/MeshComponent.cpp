#include "MeshComponent.h"

#include "Application.h"
#include "ModuleRenderer3D.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

Component_Mesh::Component_Mesh()
{
	type = MESH;
}

bool Component_Mesh::CheckFrustumCulling(Component_Camera * camera_to_check)
{
	bool ret = false;

	if (ContainsAaBox(bounding_box, camera_to_check->frustum))
		ret = true;

	return ret;
}



void Component_Mesh::draw_bounding_box()
{
	float3 c;
	if (parent->Iselected())
	{
		c = float3(0,0,0);
	}
	else
	{
		c = float3(1, 1, 1);
	}

	glDisable(GL_LIGHTING);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_LINES);

	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);
	glColor3f(c.x,c.y,c.z);
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

}

void Component_Mesh::Update(float dt)
{
	if (App->renderer3D->conf_draw != 1 && CheckFrustumCulling(App->scene_controller->current_scene->spookamera))
		mesh->Draw();

	bounding_box = mesh->bounding_box; 
	bounding_box.TransformAsAABB(parent->GetTransformComponent()->transform_global);

	//if (App->renderer3D->conf_draw==2 || App->renderer3D->conf_draw == 1)
	//	draw_bounding_box();

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		App->importer->mesh->CreateOwnFile(mesh);
	}


}

//void Component_Mesh::generate_buffer()
//{
//
//
//}

Component_Mesh::~Component_Mesh()
{
	delete mesh;
}


bool Component_Mesh::ContainsAaBox(const AABB& refBox, const Frustum & frustum) const
{
	float3 vCorner[8];
	int iTotalIn = 0;
	Plane planes[6];
	frustum.GetPlanes(planes);
	refBox.GetCornerPoints(vCorner); // get the corners of the box into the vCorner array
	// test all 8 corners against the 6 sides
	// if all points are behind 1 specific plane, we are out
	// if we are in with all points, then we are fully in
	for (int p = 0; p < 6; ++p) {
		int iInCount = 8;
		int iPtIn = 1;
		for (int i = 0; i < 8; ++i) {
			// test this point against the planes
			if (!planes[p].AreOnSameSide(vCorner[i],frustum.CenterPoint())) {
				iPtIn = 0;
				--iInCount;
			}
		}
		// were all the points outside of plane p?
		if(iInCount == 0)
			return false;
		// check if they were all on the right side of the plane
		iTotalIn += iPtIn;
	}
	// so if iTotalIn is 6, then all are inside the view
	if (iTotalIn == 6)
		return true;
	// we must be partly in then otherwise
	return true;
}

void Component_Mesh::Save_Component(rapidjson::Document* d, rapidjson::Value* v)
{
	rapidjson::Document::AllocatorType& all = d->GetAllocator();

	rapidjson::Value module_obj(rapidjson::kObjectType);

	module_obj.AddMember("res_uid", mesh->Resource_UID, all);

	v->AddMember("MESH", module_obj, all);
}

void Component_Mesh::Load_Component(rapidjson::Value::ConstMemberIterator)
{

}

void Component_Mesh::Properties()
{
	if (ImGui::CollapsingHeader("Mesh"))
	{
		ImGui::Text("Resource ID: %u", mesh->Resource_UID);
		ImGui::Text("Buffer ID: %u", mesh->buffer_id);
		ImGui::Text("");
		ImGui::Text("Num of indices:  %u", mesh->num_index);
		ImGui::Text("Num of vertices: %u", mesh->num_vertex);
	}
}