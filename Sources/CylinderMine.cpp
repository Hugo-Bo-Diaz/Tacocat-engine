#include "CylinderMine.h"
#include "glmath.h"
#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"

NOTlinder::NOTlinder(float radius, unsigned int sector_count,unsigned int stack_count, unsigned int height)
{

	float x, y, z;                              // vertex position

	float sectorStep = 2 * M_PI / sector_count;
	float stackStep = M_PI / stack_count;
	float sectorAngle, stackAngle;
	float height_gained_per_level = height / stack_count;

	//all the vertices
	for (int i = 0; i <= stack_count; ++i)
	{
		stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		y = height_gained_per_level * i;              // r * sin(u)
												  // add (sectorCount+1) vertices per stack
												  // the first and last vertices have same position and normal, but different tex coods
		for (int j = 0; j <= sector_count; ++j)
		{
			sectorAngle = j * sectorStep;

			// vertex position (x, y, z)
			z = radius * cosf(sectorAngle);             // r * cos(u) * cos(v)
			x = radius * sinf(sectorAngle);             // r * cos(u) * sin(v)
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
	}

	int k1, k2;
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(radius);

	vertices.push_back(0);
	vertices.push_back(height-2);
	vertices.push_back(radius);

	//base
	for (int i = 0; i < sector_count; i++)
	{
		indices.push_back(i);
		indices.push_back(((int)vertices.size() / 3) - 2);
		indices.push_back(i + 1);
	}

	//middle
	for (int i = 0; i < stack_count; ++i)
	{
		k1 = i * (sector_count + 1);     // beginning of current stack
		k2 = k1 + sector_count + 1;      // beginning of next stack

		for (int j = 0; j < sector_count; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding 1st and last stacks
			if (i >= 0)
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k1);

			}

			if (i != stack_count)
			{
				indices.push_back(k2 + 1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}
		}
	}

	//other base
	int last_vertex = (vertices.size() / 3) - sector_count - 3;
	for (int i = 0; i < sector_count; i++)
	{
		indices.push_back(((int)vertices.size() / 3) - 1);
		indices.push_back(i + last_vertex);
		indices.push_back(i + last_vertex + 1);
	}

	glGenBuffers(1, (GLuint*) &(buffer_id));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*indices.size(), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void NOTlinder::draw(float x, float y, float z)
{

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void NOTlinder::move(float x, float y, float z)
{
	for (int i = 0; i < vertices.size(); i += 3)
	{
		vertices[i] += x;
		vertices[i + 1] += y;
		vertices[i + 2] += z;
	}
}