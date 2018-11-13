#pragma once

#include "Globals.h"
#include <string>
#include "rapidjson/rapidjson.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/reader.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"
#include "Event.h"

class Application;
struct PhysBody3D;

class Module
{
private :
	bool enabled;


public:
	//Application* App;
	bool listentoevents=false;

	std::string name;

	Module(bool start_enabled = true)
	{}

	virtual ~Module()
	{}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void OnCollision(PhysBody3D* body1, PhysBody3D* body2)
	{}

	virtual void Configuration() {};

	virtual void Save(rapidjson::Document* d, rapidjson::Value* v) {};

	virtual void Load(rapidjson::Value& v) {};

	virtual void RecieveEvent(Event& ev) {};
};