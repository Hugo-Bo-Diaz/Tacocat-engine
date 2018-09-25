#include "Globals.h"
#include "Application.h"
#include "ModulePhysics3D.h"
#include "PhysBody3D.h"
#include "Primitive.h"

#ifdef _DEBUG
	#pragma comment (lib, "Bullet/libx86/BulletDynamics_debug.lib")
	#pragma comment (lib, "Bullet/libx86/BulletCollision_debug.lib")
	#pragma comment (lib, "Bullet/libx86/LinearMath_debug.lib")
#else
	#pragma comment (lib, "Bullet/libx86/BulletDynamics.lib")
	#pragma comment (lib, "Bullet/libx86/BulletCollision.lib")
	#pragma comment (lib, "Bullet/libx86/LinearMath.lib")
#endif

ModulePhysics3D::ModulePhysics3D( bool start_enabled) : Module(start_enabled)
{
	debug = true;

	name = "Physics3D";

	collision_conf = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_conf);
	broad_phase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	//debug_draw = new DebugDrawer();
}

// Destructor
ModulePhysics3D::~ModulePhysics3D()
{
	//delete debug_draw;
	delete solver;
	delete broad_phase;
	delete dispatcher;
	delete collision_conf;
}

// Render not available yet----------------------------------
bool ModulePhysics3D::Init()
{
	CONSOLE_LOG("Creating 3D Physics simulation");
	bool ret = true;

	return ret;
}

// ---------------------------------------------------------
bool ModulePhysics3D::Start()
{
	CONSOLE_LOG("Creating Physics environment");

	world = new btDiscreteDynamicsWorld(dispatcher, broad_phase, solver, collision_conf);
	//world->setDebugDrawer(debug_draw);
	world->setGravity(GRAVITY);
	vehicle_raycaster = new btDefaultVehicleRaycaster(world);

	// Big plane as ground    //change to a sensor that sets the player to start and restarts map
	{
		btCollisionShape* colShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);

		btDefaultMotionState* myMotionState = new btDefaultMotionState();
		btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, myMotionState, colShape);

		btRigidBody* body = new btRigidBody(rbInfo);
		world->addRigidBody(body);
	}

	return true;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PreUpdate(float dt)
{
	world->stepSimulation(dt, 15);

	int numManifolds = world->getDispatcher()->getNumManifolds();
	for(int i = 0; i<numManifolds; i++)
	{
		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
		btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());

		int numContacts = contactManifold->getNumContacts();
		if(numContacts > 0)
		{
			PhysBody3D* pbodyA = (PhysBody3D*)obA->getUserPointer();
			PhysBody3D* pbodyB = (PhysBody3D*)obB->getUserPointer();

			if(pbodyA && pbodyB)
			{
				for (std::list<Module*>::iterator item = pbodyA->collision_listeners.begin(); item != pbodyA->collision_listeners.end(); item++)
				{
					(*item)->OnCollision(pbodyA, pbodyB);
				}
				for (std::list<Module*>::iterator item_1 = pbodyB->collision_listeners.begin(); item_1 != pbodyB->collision_listeners.end(); item_1++)
				{
					(*item_1)->OnCollision(pbodyB, pbodyA);
				}

			}
		}
	}

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::Update(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(debug == true)
	{
		world->debugDrawWorld();

		// Render vehicles


	/*	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			Sphere s(1);
			s.SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);
			float force = 30.0f;
			AddBody(s)->Push(-(App->camera->Z.x * force), -(App->camera->Z.y * force), -(App->camera->Z.z * force));
		}*/
	}

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics3D::CleanUp()
{
	CONSOLE_LOG("Destroying 3D Physics simulation");

	// Remove from the world all collision bodies
	for(int i = world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = world->getCollisionObjectArray()[i];
		world->removeCollisionObject(obj);
	}

	for(std::list<btTypedConstraint*>::iterator it = constraints.begin(); it != constraints.end(); it++)
	{
		world->removeConstraint(*it);
		delete(*it);
	}
	
	constraints.clear();

	for (std::list<btDefaultMotionState*>::iterator it = motions.begin(); it != motions.end(); it++)
	{
		delete(*it);
	}
	motions.clear();


	for (std::list<btCollisionShape*>::iterator it = shapes.begin(); it != shapes.end(); it++)
	{
		delete(*it);
	}

	shapes.clear();

	for (std::list<PhysBody3D*>::iterator it = bodies.begin(); it != bodies.end(); it++)
	{
		delete(*it);
	}
	bodies.clear();

	delete vehicle_raycaster;
	delete world;

	return true;
}

// ---------------------------------------------------------
//PhysBody3D* ModulePhysics3D::AddBody(const Sphere& sphere, float mass)
//{
//	btCollisionShape* colShape = new btSphereShape(sphere.radius);
//	shapes.push_back(colShape);
//
//	btTransform startTransform;
//	startTransform.setFromOpenGLMatrix(&sphere.transform);
//
//	btVector3 localInertia(0, 0, 0);
//	if(mass != 0.f)
//		colShape->calculateLocalInertia(mass, localInertia);
//
//	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
//	motions.push_back(myMotionState);
//	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
//
//	btRigidBody* body = new btRigidBody(rbInfo);
//	PhysBody3D* pbody = new PhysBody3D(body);
//
//	world->addRigidBody(body);
//	bodies.push_back(pbody);
//
//	return pbody;
//}
//
//
//// ---------------------------------------------------------
//PhysBody3D* ModulePhysics3D::AddBody(const Cube& cube, float mass)
//{
//	btCollisionShape* colShape = new btBoxShape(btVector3(cube.size.x*0.5f, cube.size.y*0.5f, cube.size.z*0.5f));
//	shapes.push_back(colShape);
//
//	btTransform startTransform;
//	startTransform.setFromOpenGLMatrix(&cube.transform);
//
//	btVector3 localInertia(0, 0, 0);
//	if(mass != 0.f)
//		colShape->calculateLocalInertia(mass, localInertia);
//
//	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
//	motions.push_back(myMotionState);
//	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
//
//	btRigidBody* body = new btRigidBody(rbInfo);
//	PhysBody3D* pbody = new PhysBody3D(body);
//
//	world->addRigidBody(body);
//	bodies.push_back(pbody);
//
//	return pbody;
//}
//
//// ---------------------------------------------------------
//PhysBody3D* ModulePhysics3D::AddBody(const Cylinder& cylinder, float mass)
//{
//	btCollisionShape* colShape = new btCylinderShapeX(btVector3(cylinder.height*0.5f, cylinder.radius, 0.0f));
//	shapes.push_back(colShape);
//
//	btTransform startTransform;
//	startTransform.setFromOpenGLMatrix(&cylinder.transform);
//
//	btVector3 localInertia(0, 0, 0);
//	if(mass != 0.f)
//		colShape->calculateLocalInertia(mass, localInertia);
//
//	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
//	motions.push_back(myMotionState);
//	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
//
//	btRigidBody* body = new btRigidBody(rbInfo);
//	PhysBody3D* pbody = new PhysBody3D(body);
//
//	world->addRigidBody(body);
//	bodies.push_back(pbody);
//
//	return pbody;
//}

// ---------------------------------------------------------
void ModulePhysics3D::AddConstraintP2P(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB)
{
	btTypedConstraint* p2p = new btPoint2PointConstraint(
		*(bodyA.body), 
		*(bodyB.body), 
		btVector3(anchorA.x, anchorA.y, anchorA.z), 
		btVector3(anchorB.x, anchorB.y, anchorB.z));
	world->addConstraint(p2p);
	constraints.push_back(p2p);
	p2p->setDbgDrawSize(2.0f);
}

void ModulePhysics3D::AddConstraintHinge(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB, const vec3& axisA, const vec3& axisB, bool disable_collision)
{
	btHingeConstraint* hinge = new btHingeConstraint(
		*(bodyA.body), 
		*(bodyB.body), 
		btVector3(anchorA.x, anchorA.y, anchorA.z),
		btVector3(anchorB.x, anchorB.y, anchorB.z),
		btVector3(axisA.x, axisA.y, axisA.z), 
		btVector3(axisB.x, axisB.y, axisB.z));

	world->addConstraint(hinge, disable_collision);
	constraints.push_back(hinge);
	hinge->setDbgDrawSize(2.0f);
}

// =============================================
//void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
//{
//	//line.origin.Set(from.getX(), from.getY(), from.getZ());
//	//line.destination.Set(to.getX(), to.getY(), to.getZ());
//	//line.color.Set(color.getX(), color.getY(), color.getZ());
//	//line.Render();
//}
//
//void DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
//{
//	//point.transform.translate(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
//	//point.color.Set(color.getX(), color.getY(), color.getZ());
//	//point.Render();
//}
//
//void DebugDrawer::reportErrorWarning(const char* warningString)
//{
//	CONSOLE_LOG("Bullet warning: %s", warningString);
//}
//
//void DebugDrawer::draw3dText(const btVector3& location, const char* textString)
//{
//	CONSOLE_LOG("Bullet draw text: %s", textString);
//}
//
//void DebugDrawer::setDebugMode(int debugMode)
//{
//	mode = (DebugDrawModes) debugMode;
//}
//
//int	 DebugDrawer::getDebugMode() const
//{
//	return mode;
//}
