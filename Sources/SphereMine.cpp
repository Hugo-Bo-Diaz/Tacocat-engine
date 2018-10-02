#include "SphereMine.h"
#include "glmath.h"
#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"

NOTphere::NOTphere(float radius, unsigned int sector_count, unsigned int stack_count)
{
	////CALCULATE POINTS

	float x, y, z, xy;                              // vertex position

	float sectorStep = 2 * M_PI / sector_count;
	float stackStep = M_PI / stack_count;
	float stackAngle, sectorAngle;

	for (int i = 0; i <= stack_count; ++i)
	{
		stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

													// add (sectorCount+1) vertices per stack
													// the first and last vertices have same position and normal, but different tex coods
		for (int j = 0; j <= sector_count; ++j)
		{
			sectorAngle = j * sectorStep;

			// vertex position (x, y, z)
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
	}

	int k1, k2;
	for (int i = 0; i < stack_count; ++i)
	{
		k1 = i * (sector_count + 1);     // beginning of current stack
		k2 = k1 + sector_count + 1;      // beginning of next stack

		for (int j = 0; j < sector_count; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding 1st and last stacks
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			if (i != (stack_count - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}
	glGenBuffers(1, (GLuint*) &(buffer_id));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*indices.size(), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
