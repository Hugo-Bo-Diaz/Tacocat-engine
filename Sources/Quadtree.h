#ifndef QUADTREE_CLASS
#define QUADTREE_CLASS

#include "GameObject.h"
#include "MathGeoLib/MathGeoLib.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"

enum sector 
{
	ROOT,
	NW,
	NE,
	SW,
	SE
};

struct Spooktree
{
public:
	Spooktree() 
	{
		minx = 0.0f;
		minz = 0.0f;
		maxx = 0.0f;
		maxz = 0.0f;
	};
	Spooktree(sector type, float minx, float minz, float maxx, float maxz) : type(type), minx(minx), minz(minz), maxx(maxx), maxz(maxz) {};
	~Spooktree() {};

	int max = 2;

	//// Use for kd-tree, not useful for now
	//void CalculateContainer(std::vector<GameObject*> GameObjects)
	//{
	//	//Get min/max points from current container
	//	if (type != NW)
	//		x = container.MinX();
	//	if (type != NE)
	//		z = container.MinZ();
	//	if (type != SW)
	//		width = container.MaxX();
	//	if (type != SE)
	//		height = container.MaxZ();

	//	//Get min/max points from received gameobjects
	//	for (int aux = 0; aux < GameObjects.size(); aux++)
	//	{
	//		AABB auxcontainer = GameObjects[aux]->GetBoundingBox();

	//		if (auxcontainer.MinX() < x)
	//			x = auxcontainer.MinX();
	//		if (auxcontainer.MaxX() > width)
	//			width = container.MaxX();
	//		if (auxcontainer.MinZ() < z)
	//			z = container.MinZ();
	//		if (auxcontainer.MaxZ() > height)
	//			height = container.MaxZ();
	//	}

	//	//Resize container to new size
	//};

	void Divide(std::vector<GameObject*> GameObjects)
	{
		NWS = new Spooktree(NW, minx, minz, minx + maxx, minz + maxz);
		NES = new Spooktree(NE, minx, minz + maxz, minx + maxz, maxz);
		SWS = new Spooktree(SW, minx + maxx, minz, maxx, minz + maxz);
		SES = new Spooktree(SE, minx + maxx, minz + maxz, maxx, maxz);

		float sX, sZ, mX, mZ;

		for (int aux = 0; aux < GameObjects.size(); aux++)
		{
			bool assigned = false;

			sX = GameObjects[aux]->GetBoundingBox().MinX();
			mX = GameObjects[aux]->GetBoundingBox().MaxX();
			sZ = GameObjects[aux]->GetBoundingBox().MinZ();
			mZ = GameObjects[aux]->GetBoundingBox().MaxZ();

			if ((sX >= NWS->minx && mX <= NWS->maxx) || (sZ >= NWS->minz && mZ <= NWS->maxz))
			{
				NWS->children.push_back(GameObjects[aux]);
				assigned = true;
			}
			if ((sX >= NES->minx && mX <= NES->maxx) || (sZ >= NES->minz && mZ <= NES->maxz))
			{
				NWS->children.push_back(GameObjects[aux]);
				assigned = true;
			}
			if ((sX >= SWS->minx && mX <= SWS->maxx) || (sZ >= SWS->minz && mZ <= SWS->maxz))
			{
				NWS->children.push_back(GameObjects[aux]);
				assigned = true;
			}
			if ((sX >= SES->minx && mX <= SES->maxx) || (sZ >= SES->minz && mZ <= SES->maxz))
			{
				NWS->children.push_back(GameObjects[aux]);
				assigned = true;
			}

			if (assigned)
			{
				GameObjects.erase(GameObjects.begin() + aux);
			}
		}

		sectors.push_back(NWS);
		sectors.push_back(NES);
		sectors.push_back(SWS);
		sectors.push_back(SES);
	};

	//Everytime a children is added resize the containers
	void ResizeContainers() 
	{
		//CalculateContainer(children);

		for (std::list<Spooktree*>::iterator it = sectors.begin(); it != sectors.end(); it++)
		{
			if (children.size() >= max)
			{
				(*it)->Divide(children);
				(*it)->ResizeContainers();
			}
		}
	};

	//If static then add into the children vector and resize containers
	void IntoSpooktree(GameObject* newchild)
	{
		children.push_back(newchild);
		ResizeContainers();
	};

	void Draw();

private:
	std::list<Spooktree*> sectors; //It should have 0 or 4 sectors
	std::vector<GameObject*> children;

	sector type;

	AABB container;
	float minx, minz, maxx, maxz;

	Spooktree* NWS;
	Spooktree* NES;
	Spooktree* SWS;
	Spooktree* SES;
};


inline void Spooktree::Draw()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glLineWidth(2.5);
	glColor3f(1.0, 0.0, 0.0);

	glBegin(GL_LINES);
	glVertex3f(minx, 0.0, minz);
	glVertex3f(maxx, 0.0, minz);

	glVertex3f(minx, 0.0, minz);
	glVertex3f(minx, 0.0, maxz);

	glVertex3f(minx, 0.0, maxz);
	glVertex3f(maxx, 0.0, maxz);

	glVertex3f(maxx, 0.0, minz);
	glVertex3f(maxx, 0.0, maxz);
	glEnd();
}
#endif // !QUADTREE_CLASS