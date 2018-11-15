#pragma once
#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"
#include "CameraComponent.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Material.h"

class Mesh
{

public:
	
	uint Resource_UID;

	uint buffer_id;

	float* tex_coords = nullptr;

	uint num_index = 0;
	uint* index = nullptr;

	//uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;
	uint num_normals = 0;
	float* normals = nullptr;

	AABB bounding_box;

	bool not_working = false;

	Material* material;
	void SetMaterial(Material* mat) { material = mat; }

	bool CheckFrustumCulling(Component_Camera * camera_to_check);
	bool ContainsAaBox(const AABB & refBox, const Frustum& frustum) const;

	void Draw();

	~Mesh();

};


inline bool Mesh::CheckFrustumCulling(Component_Camera * camera_to_check)
{
	bool ret = false;

	if (ContainsAaBox(bounding_box, camera_to_check->frustum))
		ret = true;

	return ret;
}

inline bool Mesh::ContainsAaBox(const AABB& refBox, const Frustum & frustum) const
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

inline void Mesh::Draw()
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
		else if (material != nullptr && material->buffer_id != 0)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindTexture(GL_TEXTURE_2D, material->buffer_id);
			glTexCoordPointer(2, GL_FLOAT, 0, &tex_coords[0]);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		glVertexPointer(3, GL_FLOAT, 0, &vertex[0]);
		glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, NULL);

		if (material->buffer_id != 0)
		{
			glBindTexture(GL_TEXTURE_2D, 0);

		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glDisableClientState(GL_VERTEX_ARRAY);


	}
}
inline Mesh::~Mesh()
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