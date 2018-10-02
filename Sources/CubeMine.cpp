#include "CubeMine.h"
#include "glmath.h"
#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"

NOTcube::NOTcube(float width, float height, float width2)
{
	vertices = { 
		width / 2, -height / 2, -width2 / 2, // A = 0
		width / 2, height / 2, -width2 / 2, // B = 1
		width / 2, -height / 2, width2 / 2, // C = 2
		width / 2, height / 2, width2 / 2, // D = 3
		-width / 2, -height / 2, -width2 / 2, // E = 4
		-width / 2, height / 2, -width2 / 2, // F = 5
		-width / 2, -height / 2, width2 / 2, // G = 6
		-width / 2, height / 2, width2 / 2, // H = 7
	};

	indices =
	{ 0,1,3, 3,2,0,
		1,5,7,  7,3,1,
		5,4,6,  6,7,5,
		4,0,2,  2,6,4,
		2,3,7,  7,6,2,
		4,5,1,  1,0,4 };

	glGenBuffers(1, (GLuint*) &(buffer_id));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*indices.size(), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}