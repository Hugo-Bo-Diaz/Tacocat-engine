#include "ArrowMine.h"
#include "glmath.h"
#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"

NOTarrow::NOTarrow(float x1, float y1, float z1, float x2, float y2, float z2)
{
	vertices.push_back(x1);
	vertices.push_back(y1);
	vertices.push_back(z1);
	vertices.push_back(x2);
	vertices.push_back(y2);
	vertices.push_back(z2);

	indices.push_back(1);
	indices.push_back(2);

	glGenBuffers(1, (GLuint*) &(buffer_id));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*indices.size(), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void NOTarrow::draw()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
	glVertexPointer(2, GL_FLOAT, 0, &vertices[0]);
	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}