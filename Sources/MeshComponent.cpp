#include "MeshComponent.h"

#include "Application.h"
#include "ModuleRenderer3D.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

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

void Component_Mesh::draw()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);


	if (App->renderer3D->draw_checkers)
	{
		glBindTexture(GL_TEXTURE_2D, App->renderer3D->texture_buffer);
		glTexCoordPointer(2, GL_FLOAT, 0, &tex_coords[0]);
	}
	else if (texture != 0)
	{
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexCoordPointer(2, GL_FLOAT, 0, &tex_coords[0]);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glVertexPointer(3, GL_FLOAT, 0, &vertex[0]);
	glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, NULL);

	if (texture != 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);

	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

void Component_Mesh::generate_buffer()
{


}

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
	if (texture != 0)
		glDeleteTextures(1, &texture);

}