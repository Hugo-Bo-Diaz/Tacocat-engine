#ifndef QUADTREE_CLASS
#define QUADTREE_CLASS

#include "GameObject.h"
#include "MathGeoLib/MathGeoLib.h"

struct Spooktree
{
public:
	Spooktree() {};
	~Spooktree() {};

	int max = 2;

	void CalculateContainer(std::vector<GameObject*> GameObjects)
	{
		//Get min/max points from current container
		x = container.MinX();
		z = container.MinZ();
		width = container.MaxX();
		height = container.MaxZ();

		//Get min/max points from received gameobjects
		for (int aux = 0; aux < GameObjects.size(); aux++)
		{
			AABB auxcontainer = GameObjects[aux]->GetBoundingBox();

			if (auxcontainer.MinX() < x)
				x = auxcontainer.MinX();
			if (auxcontainer.MaxX() > width)
				width = container.MaxX();
			if (auxcontainer.MinZ() < z)
				z = container.MinZ();
			if (auxcontainer.MaxZ() > height)
				height = container.MaxZ();
		}

		//Resize container to new size
	};

	//Everytime a children is added resize the containers
	void ResizeContainers() 
	{
		CalculateContainer(children);
		for (std::list<Spooktree*>::iterator it = sectors.begin(); it != sectors.end(); it++)
		{
			(*it)->ResizeContainers();
		}
	};

private:
	std::list<Spooktree*> sectors; //It should have 0 or 4 sectors
	std::vector<GameObject*> children;

	AABB container;
	float x, z, width, height;
};

#endif // !QUADTREE_CLASS