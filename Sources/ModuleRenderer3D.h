#pragma once
#include "Module.h"
#include "Globals.h"
//#include "glmath.h"//TOCHANGE
#include "Light.h"
#include <vector>

#define MAX_LIGHTS 8

class NOTphere;
class NOTlinder;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D( bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

public:

	//float* float4x4_to_float(float4x4 to_change);

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	//math::float3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	
	uint my_id = 0;
	uint my_id2 = 0;
	std::vector<float> vertices;
	std::vector<float> vertices2;
	std::vector<uint> vertex_order;

/*
	//0.5f, -0.5f, -0.5f, // A
	//	0.5f, 0.5f, -0.5f, // B
	//	0.5f, 0.5f, 0.5f, // D

	//	0.5f, 0.5f, 0.5f, // D
	//	0.5f, -0.5f, 0.5f, // C
	//	0.5f, -0.5f, -0.5f, // A

	//	0.5f, 0.5f, -0.5f, // B
	//	-0.5f, 0.5f, -0.5f, // F
	//	-0.5f, 0.5f, 0.5f, // H

	//	-0.5f, 0.5f, 0.5f, // H
	//	0.5f, 0.5f, 0.5f, // D
	//	0.5f, 0.5f, -0.5f, // B

	//	-0.5f, 0.5f, -0.5f, // F
	//	-0.5f, -0.5f, -0.5f, // E
	//	-0.5f, -0.5f, 0.5f, // G

	//	-0.5f, -0.5f, 0.5f, // G
	//	-0.5f, 0.5f, 0.5f, // H
	//	-0.5f, 0.5f, -0.5f, // F

	//	-0.5f, -0.5f, -0.5f, // E
	//	0.5f, -0.5f, -0.5f, // A
	//	0.5f, -0.5f, 0.5f, // C

	//	0.5f, -0.5f, 0.5f, // C
	//	-0.5f, -0.5f, 0.5f, // G
	//	-0.5f, -0.5f, -0.5f, // E

	//	0.5f, -0.5f, 0.5f, // C
	//	0.5f, 0.5f, 0.5f, // D
	//	-0.5f, 0.5f, 0.5f, // H

	//	-0.5f, 0.5f, 0.5f, // H
	//	-0.5f, -0.5f, 0.5f, // G
	//	0.5f, -0.5f, 0.5f, // C

	//	-0.5f, -0.5f, -0.5f, // E
	//	-0.5f, 0.5f, -0.5f, // F
	//	0.5f, 0.5f, -0.5f, // B

	//	0.5f, 0.5f, -0.5f,// B
	//	0.5f, -0.5f, -0.5f, // A
	//	-0.5f, -0.5f, -0.5f // E
	*/



	//mat3x3 NormalMatrix;
	//mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	void enable_flag_depth_test();
	void disable_flag_depth_test();

	void enable_flag_cull_face();
	void disable_flag_cull_face();
	
	void enable_flag_lighting();
	void disable_flag_lighting();

	void enable_flag_color_material();
	void disable_flag_color_material();

	void enable_flag_texture_2D();
	void disable_flag_texture_2D();

	bool conf_depth_test = false;
	bool conf_cull_face = false;
	bool conf_lighting = false;
	bool conf_color_material = false;
	bool conf_texture_2D = false;
	bool conf_wireframe = false;

	void Configuration();

	NOTphere* sph;
	NOTlinder* lin;

	//void DrawQuad(	std::vector<float> point1,
	//				std::vector<float> point2,
	//				std::vector<float> point3,
	//				std::vector<float> point4);

		//DOESN'T ACTUALLY DRAW QUADS
};