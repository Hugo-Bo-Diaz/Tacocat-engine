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

void Component_Mesh::Move(float x, float y, float z)
{
	for (int i = 0; i < num_vertex * 3; i += 3)
	{
		vertex[i] += x;
		vertex[i + 1] += y;
		vertex[i + 2] += z;
	}
}

void Component_Mesh::Scale(float scalex, float scaley, float scalez)
{
	for (int i = 0; i < num_index; i += 3)
	{
		vertex[i] *= scalex;
		vertex[i + 1] *= scaley;
		vertex[i + 2] *= scalez;
	}
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
	if (CheckFrustumCulling(App->scene_controller->GetMainCamera()))
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
	else if (parent->GetTexture(material_index) != 0)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D, parent->GetTexture(material_index));
		glTexCoordPointer(2, GL_FLOAT, 0, &tex_coords[0]);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	//glRotatef(imdumb++, 0, 1, 0);

	glVertexPointer(3, GL_FLOAT, 0, &vertex[0]);
	glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, NULL);

	//glRotatef(-imdumb, 0, 1, 0);

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
	glBegin(GL_LINES);

	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);

	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.minPoint.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.minPoint.z);

	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.minPoint.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.minPoint.y, bounding_box.minPoint.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.minPoint.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.maxPoint.y, bounding_box.minPoint.z);

	glVertex3f(bounding_box.minPoint.x, bounding_box.minPoint.y, bounding_box.minPoint.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.maxPoint.y, bounding_box.minPoint.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);

	glVertex3f(bounding_box.minPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);

	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);

	glVertex3f(bounding_box.minPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.minPoint.y, bounding_box.minPoint.z);

	glVertex3f(bounding_box.minPoint.x, bounding_box.minPoint.y, bounding_box.minPoint.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.minPoint.z);

	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.minPoint.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);

	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);


	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);

	glVertex3f(bounding_box.minPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.maxPoint.y, bounding_box.minPoint.z);

	glVertex3f(bounding_box.minPoint.x, bounding_box.maxPoint.y, bounding_box.minPoint.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.minPoint.z);

	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.minPoint.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);


	glEnd();
}

void Component_Mesh::Update(float dt)
{
	draw();

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		App->importer->mesh->CreateOwnFile(this);
	}
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		Move(10, 0, 10);
		bounding_box.Translate(float3(10, 0, 10));
		//App->importer->mesh->Import();
	}
}

//void Component_Mesh::generate_buffer()
//{
//
//
//}

Component_Mesh::~Component_Mesh()
{
	if (tex_coords != nullptr)
	{
		delete[] tex_coords;
		tex_coords = nullptr;
	}
	if (vertex != nullptr)
	{
		delete[] vertex;
		vertex = nullptr;
	}
	if (index != nullptr)
	{
		delete[] index;
		index = nullptr;
	}
	glDeleteBuffers(1, &buffer_id);
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

	module_obj.AddMember("material_id", material_index, all);

	rapidjson::Value transform_node(rapidjson::kObjectType);

	transform_node.AddMember("transform_scale_x", scaling.x, all);
	transform_node.AddMember("transform_scale_y", scaling.y, all);
	transform_node.AddMember("transform_scale_z", scaling.z, all);
	
	transform_node.AddMember("transform_rotation_x", rotation.x, all);
	transform_node.AddMember("transform_rotation_y", rotation.y, all);
	transform_node.AddMember("transform_rotation_z", rotation.z, all);
	
	transform_node.AddMember("transform_position_x", position.x, all);
	transform_node.AddMember("transform_position_y", position.y, all);
	transform_node.AddMember("transform_position_z", position.z, all);

	module_obj.AddMember("transform", transform_node, all);

	v->AddMember("MESH", module_obj, all);
}

void Component_Mesh::Load_Component(rapidjson::Value& v)
{

}