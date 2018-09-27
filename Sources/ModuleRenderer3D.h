#pragma once
#include "Module.h"
#include "Globals.h"
//#include "glmath.h"//TOCHANGE
#include "Light.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D( bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

public:

	float* float4x4_to_float(float4x4 to_change);

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	math::float3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	uint my_id = 0;
	float vertices[] = {
		0.5f, -0.5f, -0.5f, // A
		0.5f, 0.5f, -0.5f, // B
		0.5f, 0.5f, 0.5f, // D

		0.5f, 0.5f, 0.5f, // D
		0.5f, -0.5f, 0.5f, // C
		0.5f, -0.5f, -0.5f, // A

		0.5f, 0.5f, -0.5f, // B
		-0.5f, 0.5f, -0.5f, // F
		-0.5f, 0.5f, 0.5f, // H

		-0.5f, 0.5f, 0.5f, // H
		0.5f, 0.5f, 0.5f, // D
		0.5f, 0.5f, -0.5f, // B

		-0.5f, 0.5f, -0.5f, // F
		-0.5f, -0.5f, -0.5f, // E
		-0.5f, -0.5f, 0.5f, // G

		-0.5f, -0.5f, 0.5f, // G
		-0.5f, 0.5f, 0.5f, // H
		-0.5f, 0.5f, -0.5f, // F

		-0.5f, -0.5f, -0.5f, // E
		0.5f, -0.5f, -0.5f, // A
		0.5f, -0.5f, 0.5f, // C

		0.5f, -0.5f, 0.5f, // C
		-0.5f, -0.5f, 0.5f, // G
		-0.5f, -0.5f, -0.5f, // E

		0.5f, -0.5f, 0.5f, // C
		0.5f, 0.5f, 0.5f, // D
		-0.5f, 0.5f, 0.5f, // H

		-0.5f, 0.5f, 0.5f, // H
		-0.5f, -0.5f, 0.5f, // G
		0.5f, -0.5f, 0.5f, // C

		-0.5f, -0.5f, -0.5f, // E
		-0.5f, 0.5f, -0.5f, // F
		0.5f, 0.5f, -0.5f, // B

		0.5f, 0.5f, -0.5f,// B
		0.5f, -0.5f, -0.5f, // A
		-0.5f, -0.5f, -0.5f // E
	};
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

};