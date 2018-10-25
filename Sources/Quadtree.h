#pragma once

#include "MathGeoLib/MathGeoLib.h"
#include <vector>
#include "GameObject.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"


class NOTQuadtree
{
public:
	NOTQuadtree() {};
	NOTQuadtree(float3 min_point,float3 max_point);

	//children
	NOTQuadtree * upleft = nullptr;
	NOTQuadtree * upright = nullptr;
	NOTQuadtree * downleft = nullptr;
	NOTQuadtree * downright = nullptr;

	int max_elements_on_Quad = 1;

	void Recalculate_tree();
	void draw();

	std::vector<GameObject*> objects;

	AABB box;
};

inline NOTQuadtree::NOTQuadtree(float3 min_point, float3 max_point)
{
	box.minPoint = min_point;
	box.maxPoint = max_point;
}

inline void NOTQuadtree::Recalculate_tree()
{
	if (objects.size() > max_elements_on_Quad)//are there too many elements in here?
	{
		upleft = new NOTQuadtree(	box.minPoint, 
									float3((box.minPoint.x + (box.maxPoint.x - box.minPoint.x)/2),	box.maxPoint.y, box.minPoint.z + (box.maxPoint.z - box.minPoint.z)/2)//middle
								);
		
		upright = new NOTQuadtree(	float3((box.minPoint.x + (box.maxPoint.x - box.minPoint.x)/2),	box.minPoint.y, box.minPoint.z),//up
									float3(box.maxPoint.x,											box.maxPoint.y, box.minPoint.z + (box.maxPoint.z - box.minPoint.z)/2)//right
								);
		
		downleft = new NOTQuadtree(	float3(box.minPoint.x,											box.minPoint.y, box.minPoint.z + (box.maxPoint.z - box.minPoint.z)/2), //left
									float3((box.minPoint.x + (box.maxPoint.x - box.minPoint.x)/2),	box.maxPoint.y, box.maxPoint.z)//down
								);
		
		downright = new NOTQuadtree(float3((box.minPoint.x + (box.maxPoint.x - box.minPoint.x)/2),	box.minPoint.y, box.minPoint.z + (box.maxPoint.z - box.minPoint.z)/2),//middle
									box.maxPoint
								);

		//now that we have the 4 quads we have to separate the objects into them

		for (std::vector<GameObject*>::iterator it = objects.begin(); it != objects.end(); it++)
		{
			//where does this go??
			if		(upleft->box.Contains(float3((*it)->GetBoundingBox().CenterPoint().x, upleft->box.CenterPoint().y, (*it)->GetBoundingBox().CenterPoint().z)))
			{
				upleft->objects.push_back(*it);
			}
			else if (upright->box.Contains(float3((*it)->GetBoundingBox().CenterPoint().x, upright->box.CenterPoint().y, (*it)->GetBoundingBox().CenterPoint().z)))
			{
				upright->objects.push_back(*it);
			}
			else if (downleft->box.Contains(float3((*it)->GetBoundingBox().CenterPoint().x, downleft->box.CenterPoint().y, (*it)->GetBoundingBox().CenterPoint().z)))
			{
				downleft->objects.push_back(*it);
			}
			else if (downright->box.Contains(float3((*it)->GetBoundingBox().CenterPoint().x, downright->box.CenterPoint().y, (*it)->GetBoundingBox().CenterPoint().z)))
			{
				downright->objects.push_back(*it);
			}
		}
		objects.clear();
	}

	if (upleft != nullptr)
		upleft->Recalculate_tree();
	if (upright != nullptr)
		upright->Recalculate_tree();
	if (downleft != nullptr)
		downleft->Recalculate_tree();
	if (downright != nullptr)
		downright->Recalculate_tree();

}

inline void NOTQuadtree::draw()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	float3 vert[8];
	box.GetCornerPoints(vert);

	glBegin(GL_LINES);
	
	glVertex3fv((GLfloat*)&vert[1]);
	glVertex3fv((GLfloat*)&vert[5]);
	glVertex3fv((GLfloat*)&vert[7]);
	glVertex3fv((GLfloat*)&vert[3]);

	glVertex3fv((GLfloat*)&vert[3]);
	glVertex3fv((GLfloat*)&vert[1]);

	glVertex3fv((GLfloat*)&vert[4]);
	glVertex3fv((GLfloat*)&vert[0]);
	glVertex3fv((GLfloat*)&vert[2]);
	glVertex3fv((GLfloat*)&vert[6]);

	glVertex3fv((GLfloat*)&vert[6]);
	glVertex3fv((GLfloat*)&vert[4]);

	glVertex3fv((GLfloat*)&vert[5]);
	glVertex3fv((GLfloat*)&vert[4]);
	glVertex3fv((GLfloat*)&vert[6]);
	glVertex3fv((GLfloat*)&vert[7]);

	glVertex3fv((GLfloat*)&vert[7]);
	glVertex3fv((GLfloat*)&vert[5]);

	glVertex3fv((GLfloat*)&vert[0]);
	glVertex3fv((GLfloat*)&vert[1]);
	glVertex3fv((GLfloat*)&vert[3]);
	glVertex3fv((GLfloat*)&vert[2]);

	glVertex3fv((GLfloat*)&vert[2]);
	glVertex3fv((GLfloat*)&vert[6]);

	glVertex3fv((GLfloat*)&vert[3]);
	glVertex3fv((GLfloat*)&vert[7]);
	glVertex3fv((GLfloat*)&vert[6]);
	glVertex3fv((GLfloat*)&vert[2]);

	glVertex3fv((GLfloat*)&vert[2]);
	glVertex3fv((GLfloat*)&vert[0]);

	glVertex3fv((GLfloat*)&vert[0]);
	glVertex3fv((GLfloat*)&vert[4]);
	glVertex3fv((GLfloat*)&vert[5]);
	glVertex3fv((GLfloat*)&vert[1]);

	glEnd();

	if (upleft != nullptr)
		upleft->draw();
	if (upright != nullptr)
		upright->draw();
	if (downleft != nullptr)
		downleft->draw();
	if (downright != nullptr)
		downright->draw();
}
