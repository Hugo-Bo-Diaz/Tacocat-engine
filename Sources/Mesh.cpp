#include "Mesh.h"
#include "Application.h"


#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>



bool Mesh::CheckFrustumCulling(Component_Camera * camera_to_check)
{
	bool ret = false;

	if (ContainsAaBox(bounding_box, camera_to_check->frustum))
		ret = true;

	return ret;
}

bool Mesh::ContainsAaBox(const AABB& refBox, const Frustum & frustum) const
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
			if (!planes[p].AreOnSameSide(vCorner[i], frustum.CenterPoint())) {
				iPtIn = 0;
				--iInCount;
			}
		}
		// were all the points outside of plane p?
		if (iInCount == 0)
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

void Mesh::Recalculate_bounding_box()
{

}

void Mesh::Draw()
{
	if (material == nullptr && materialUID != 0)
	{
		Resource* res = App->fsys->ResourceFromUID(materialUID);
		if (res->type == RES_MATERIAL)
			material = res->mat.ptr;
	}

	if (/*CheckFrustumCulling(App->scene_controller->GetMainCamera()) && */!not_working)
	{

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

		if (material != nullptr && material->texture_buffer_id != 0)
		{
			glBindTexture(GL_TEXTURE_2D, 0);

		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glDisableClientState(GL_VERTEX_ARRAY);


	}
}
void Mesh::DrawBoundingBox()
{

}

Mesh::~Mesh()
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