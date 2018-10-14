#include "Primitive.h"
#include "glmath.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"

void NOTprimitive::draw()
{

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


}

void NOTprimitive::move(float x, float y, float z)
{
	for (int i = 0; i < vertices.size(); i += 3)
	{
		vertices[i] += x;
		vertices[i + 1] += y;
		vertices[i + 2] += z;
	}
}

NOTprimitive::~NOTprimitive()
{

	vertices.clear();
	indices.clear();
	glDeleteBuffers(1,&buffer_id);

}