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
	vertex = nullptr;
	index = nullptr;
}

bool Component_Mesh::CheckFrustumCulling(Component_Camera * camera_to_check)
{
	bool ret = false;

	if (ContainsAaBox(bounding_box, camera_to_check->frustum))
		ret = true;

	return ret;
}

void Component_Mesh::draw()
{	
	if (CheckFrustumCulling(App->scene_controller->GetMainCamera()) && !not_working)
	{

		//if (buffer_id > 2)
		//{
		//	bounding_box.Translate(float3(0, 0.1, 0));
		//	Move(0, 0.1, 0);
		//}


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);

	if (App->renderer3D->draw_checkers)
	{	
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D, App->renderer3D->texture_buffer);
		glTexCoordPointer(2, GL_FLOAT, 0, &tex_coords[0]);
	}
	//else if (parent->GetTexture(material_index) != 0)
	else if (material != nullptr && material->texture_buffer_id != 0)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D, material->texture_buffer_id);
		glTexCoordPointer(2, GL_FLOAT, 0, &tex_coords[0]);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glVertexPointer(3, GL_FLOAT, 0, &vertex[0]);
	glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, NULL);

	if (parent->GetTexture(material_index) != 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);

	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisableClientState(GL_VERTEX_ARRAY);


	}
	draw_bounding_box();

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
	mesh->Draw();

	//if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	//{
	//	App->importer->mesh->CreateOwnFile(mesh);
	//}
	//if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	//{
	//	bounding_box.Translate(float3(10, 0, 10));
	//	//App->importer->mesh->Import();
	//}
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

void Component_Mesh::Load_Component(rapidjson::Value& v)
{

}