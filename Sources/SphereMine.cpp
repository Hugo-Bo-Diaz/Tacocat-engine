#include "SphereMine.h"
#include "glmath.h"
#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"

NOTphere::NOTphere(float radius, unsigned int sector_count, unsigned int stack_count)
{
	////CALCULATE POINTS
	//float angle_interval = (M_PI/2) / tris_on_straight_angle;
	//for (int iterations_on_y = 0; iterations_on_y < tris_on_straight_angle *4; iterations_on_y ++)//for all the angles
	//{
	//	for (int iterations_on_z = 0; iterations_on_z < tris_on_straight_angle*4; iterations_on_z)
	//	{
	//		float x_to_add = cosf(iterations_on_y*angle_interval) * radius;
	//		float y_to_add = sinf(iterations_on_y*angle_interval) * radius;
	//		float z_to_add = sinf(iterations_on_z*angle_interval) * radius;
	//		vertices.push_back(x_to_add);
	//		vertices.push_back(y_to_add);
	//		vertices.push_back(z_to_add);
	//	}
	//}

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

void NOTphere::draw(float x, float y, float z)
{

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void NOTphere::move(float x, float y, float z)
{
	for (int i = 0; i < vertices.size(); i += 3)
	{
		vertices[i] += x;
		vertices[i + 1] += y;
		vertices[i + 2] += z;
	}
}