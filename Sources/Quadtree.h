#ifndef QUADTREE_CLASS
#define QUADTREE_CLASS

#include "GameObject.h"
#include "MathGeoLib/MathGeoLib.h"

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
		x = 0.0f;
		z = 0.0f;
		width = 0.0f;
		height = 0.0f;
	};
	Spooktree(sector type, float x, float y, float width, float height) : type(type), x(x), z(z), width(width), height(height) {};
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
		NWS = new Spooktree(NW, x, z, width / 2, height / 2);
		NES = new Spooktree(NE, x, z + (width / 2), height / 2, width / 2);
		SWS = new Spooktree(SW, x + (height / 2), z, height / 2, width / 2);
		SES = new Spooktree(SE, x + (height / 2), z + (height / 2), height / 2, width / 2);

		for (int aux = 0; aux < GameObjects.size(); aux++)
		{
			//if (GameObjects[aux]->GetBoundingBox() >= aux) //dunno how to get the size of the bounding box
			//{
			//	NWS->children.push_back(GameObjects[aux]);
			//}

			//same for the other ones
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
			if (children.size >= max)
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

private:
	std::list<Spooktree*> sectors; //It should have 0 or 4 sectors
	std::vector<GameObject*> children;

	sector type;

	AABB container;
	float x, z, width, height;

	Spooktree* NWS;
	Spooktree* NES;
	Spooktree* SWS;
	Spooktree* SES;
};

#endif // !QUADTREE_CLASS