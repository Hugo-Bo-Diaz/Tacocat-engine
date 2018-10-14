#pragma once
#include "Module.h"
#include "Globals.h"
//#include "glmath.h"//TOCHANGE
#include "Light.h"
#include <vector>

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"

#define MAX_LIGHTS 8

class NOTphere;
class NOTlinder;
class NOTcube;
class NOTprimitive;
class NOTmesh;

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

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;

	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

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
	void enable_flag_draw_checkers();

	bool draw_checkers = false;

	bool conf_depth_test = true;
	bool conf_cull_face = true;
	bool conf_lighting = true;
	int conf_draw = -1;
	int conf_texture = -1;

	bool prev_conf_depth_test = false;
	bool prev_conf_cull_face = false;
	bool prev_conf_lighting = false;
	int prev_conf_texture = -1;

	bool justloaded = true;
	void Configuration();
	void Properties();

	NOTphere* AddSphere(float radius, double rings, double stacks,				 float x = 0, float y = 0, float z = 0);
	NOTcube* AddCube(float widthx, float height, float widthz ,					 float x = 0, float y = 0, float z = 0);
	NOTlinder* AddCylinder(float radius, float sector, float stacks, float height, float x = 0, float y = 0, float z = 0);
	void AddElement(NOTprimitive*);
	void AddMesh(NOTmesh*);
	void CleanPrimitives();

	std::vector<NOTprimitive*> primitive_vector;
	std::vector<NOTmesh*> mesh_vector;

	GLuint texture_buffer = 0;
	GLubyte checkImage[128][128][4];

private:
	void FillDraw();
	void LineDraw();
	void BoundingBoxDraw();
};