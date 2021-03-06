#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "SphereMine.h"
#include "CylinderMine.h"
#include "CubeMine.h"
#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"

//#include <gl/GL.h>
//#include <gl/GLU.h>

//#include "mmgr\mmgr.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")


ModuleRenderer3D::ModuleRenderer3D( bool start_enabled) : Module(start_enabled)
{
	name = "Render3D";
	listentoevents = true;
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	CONSOLE_LOG("Creating 3D Renderer context");
	bool ret = true;

	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if (context == NULL)
	{
		CONSOLE_LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	GLenum error = glewInit();
	if (error != GL_NO_ERROR)
	{
		App->UI->console->AddLog("Error initializing glew! Error: %s\n", gluErrorString(error));
		ret = false;
	}

	if(ret == true)
	{
		//Use Vsync
		if (App->confg_vsync)
			SDL_GL_SetSwapInterval(0);
		else
			SDL_GL_SetSwapInterval(1);

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			App->UI->console->AddLog("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			App->UI->console->AddLog("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		glClearColor(0.5f, 0.5f, 0.5f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			App->UI->console->AddLog("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		lights[0].Active(true);
		if (conf_depth_test)
			glEnable(GL_DEPTH_TEST);
		if (conf_cull_face)
			glEnable(GL_CULL_FACE);
		if (conf_lighting)
			glEnable(GL_LIGHTING);
		if (conf_draw == 0)
			glEnable(GL_COLOR_MATERIAL);
		else if (conf_draw == 1)
			glEnable(GL_TEXTURE_2D);
	}

	// Projection matrix for
	OnResize(App->window->width, App->window->height);
	return ret;
}

bool ModuleRenderer3D::Start()
{
	for (int i = 0; i < 128; i++) {
		for (int j = 0; j < 128; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texture_buffer);
	glBindTexture(GL_TEXTURE_2D, texture_buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 128,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

	//delete[] checkImage;

	return true;

}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity(); 

	glMatrixMode(GL_MODELVIEW);
	//glLoadMatrixf(App->camera->GetViewMatrix());
	glLoadMatrixf(App->scene_controller->current_scene->spookamera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->scene_controller->current_scene->spookamera->Position.x, App->scene_controller->current_scene->spookamera->Position.y, App->scene_controller->current_scene->spookamera->Position.z);
	//lights[0].SetPos(App->scene_controller->current_scene->spookamera->Position.x, App->scene_controller->current_scene->spookamera->Position.y, App->camera->Position.z);


	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();


	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	//App->scene_controller->Draw();
	glBindTexture(GL_TEXTURE_2D,0);
	glColor3f(0.25f, 0.25f, 0.25f);
	glLineWidth(1.0f);
	glBegin(GL_LINES);
	float d = 200.0f;

	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();

	if (conf_draw == 0)
	{
		glColor3f(1, 1, 1);
		FillDraw();
	}

	else if (conf_draw == 1)
	{		
		glColor3f(1, 1, 1);
		LineDraw();
	}
	else
	{
		FillDraw();
		glLineWidth(2.0f);
		BoundingBoxDraw();
		LineDraw();
		App->mesh_loader->DrawSceneBoundingBox();
	}
	
	App->UI->Draw();

	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	App->UI->console->AddLog("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (App->scene_controller->current_scene)
		ProjectionMatrix = App->scene_controller->GetMainCamera()->frustum.ProjectionMatrix();
		/*App->scene_controller->current_scene->spookamera->projection_matrix_todelete(60.0f, (float)width / (float)height, 0.125f, 512.0f).Transposed();*/
		//perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix[0][0]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::enable_flag_depth_test()
{
	glEnable(GL_DEPTH_TEST);
}
void ModuleRenderer3D::disable_flag_depth_test()
{
	glDisable(GL_DEPTH_TEST);
}

void ModuleRenderer3D::enable_flag_cull_face()
{
	glEnable(GL_CULL_FACE);
}
void ModuleRenderer3D::disable_flag_cull_face()
{
	glDisable(GL_CULL_FACE);
}

void ModuleRenderer3D::enable_flag_lighting()
{
	glEnable(GL_LIGHTING);
}
void ModuleRenderer3D::disable_flag_lighting()
{
	glDisable(GL_LIGHTING);
}

void ModuleRenderer3D::enable_flag_color_material()
{
	glEnable(GL_COLOR_MATERIAL);
}
void ModuleRenderer3D::disable_flag_color_material()
{
	glDisable(GL_COLOR_MATERIAL);
}

void ModuleRenderer3D::enable_flag_texture_2D()
{
	glEnable(GL_TEXTURE_2D);
	draw_checkers = false;
}
void ModuleRenderer3D::disable_flag_texture_2D()
{
	glDisable(GL_TEXTURE_2D);
	draw_checkers = false;
}

void ModuleRenderer3D::enable_flag_draw_checkers()
{
	glEnable(GL_TEXTURE_2D);
	draw_checkers = true;
}

NOTphere* ModuleRenderer3D::AddSphere(float radius, double rings, double stacks, float x, float y, float z)
{
	NOTphere* prim = new NOTphere(radius,rings,stacks);
	
	prim->move(x, y, z);
	primitive_vector.push_back(prim);
	return prim;
}

NOTcube* ModuleRenderer3D::AddCube(float widthx, float height, float widthz, float x, float y, float z)
{
	NOTcube* prim = new NOTcube(widthx, height, widthz);

	prim->move(x, y, z);
	primitive_vector.push_back(prim);

	return prim;
}

NOTlinder* ModuleRenderer3D::AddCylinder(float radius, float sector, float stacks, float height, float x, float y, float z)
{
	NOTlinder* prim = new NOTlinder(radius, sector, stacks, height);

	prim->move(x, y, z);
	primitive_vector.push_back(prim);

	return prim;
}

void ModuleRenderer3D::AddElement(NOTprimitive* p)
{
	primitive_vector.push_back(p);
}

void ModuleRenderer3D::FillDraw()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnableClientState(GL_VERTEX_ARRAY);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	for (std::vector<NOTprimitive*>::iterator it = primitive_vector.begin(); it != primitive_vector.end(); it++)
	{
		(*it)->draw();
	}

	//for (std::vector<NOTmesh*>::iterator it = mesh_vector.begin(); it != mesh_vector.end(); it++)
	//{
	//	(*it)->draw();
	//}

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleRenderer3D::LineDraw()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnableClientState(GL_VERTEX_ARRAY);

	for (std::vector<NOTprimitive*>::iterator it = primitive_vector.begin(); it != primitive_vector.end(); it++)
	{
		(*it)->draw();
	}
	//for (std::vector<NOTmesh*>::iterator it = mesh_vector.begin(); it != mesh_vector.end(); it++)
	//{
	//	(*it)->draw();
	//}
	glDisableClientState(GL_VERTEX_ARRAY);

}

void ModuleRenderer3D::BoundingBoxDraw()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
	glColor3f(0.0f, 0.75f, 0.0f);
	glLineWidth(2.0f);

	glColor3f(1.0f, 1.0f, 1.0f);

}

void ModuleRenderer3D::Configuration()
{
	if (!justloaded)
	{
	prev_conf_depth_test = conf_depth_test;
	prev_conf_cull_face = conf_cull_face;
	prev_conf_lighting = conf_lighting;
	prev_conf_texture = conf_texture;
	}
	else
	{
		justloaded = false;
	}

	if (ImGui::CollapsingHeader("OpenGL Settings"))
	{
		ImGui::Text("Draw Mode");
		ImGui::RadioButton("Fill", &conf_draw, 0); ImGui::SameLine();
		ImGui::RadioButton("Wireframe", &conf_draw, 1); ImGui::SameLine();
		ImGui::RadioButton("Debug", &conf_draw, 2);
		ImGui::Separator();
		ImGui::Text("Draw settings");
		ImGui::Checkbox("depth_test", &conf_depth_test); ImGui::SameLine(150);
		ImGui::Checkbox("cull_face", &conf_cull_face);
		ImGui::Checkbox("lighting", &conf_lighting);
		ImGui::RadioButton("Flat color", &conf_texture, 0); ImGui::SameLine();
		ImGui::RadioButton("Texture", &conf_texture, 1); ImGui::SameLine();
		ImGui::RadioButton("Checker", &conf_texture, 2);	
	}

	if (prev_conf_depth_test != conf_depth_test)
	{
		if (conf_depth_test)
		{
			enable_flag_depth_test();
		}
		else
		{
			disable_flag_depth_test();
		}
	}

	if (prev_conf_cull_face != conf_cull_face)
	{
		if (conf_cull_face)
		{
			enable_flag_cull_face();
		}
		else
		{
			disable_flag_cull_face();
		}
	}

	if (prev_conf_lighting != conf_lighting)
	{
		if (conf_lighting)
		{
			enable_flag_lighting();
		}
		else
		{
			disable_flag_lighting();
		}
	}

	if (prev_conf_texture != conf_texture)
	{
		disable_flag_color_material();
		disable_flag_texture_2D();

		if (conf_texture == 0)
		{
			enable_flag_color_material();
		}
		else if (conf_texture == 1)
		{
			enable_flag_texture_2D();
		}
		else
		{
			enable_flag_draw_checkers();
		}
	}
}

void ModuleRenderer3D::Properties()//imgui type function to output the meshes properties
{
	//vec3 pos, rot, scale;

	//for (std::vector<NOTmesh*>::iterator it = mesh_vector.begin(); it != mesh_vector.end(); it++)
	//{
	//	pos.Set((*it)->position.x, (*it)->position.y, (*it)->position.z);
	//	rot.Set((*it)->rotation.x * RADTODEG, (*it)->rotation.y * RADTODEG, (*it)->rotation.z* RADTODEG);
	//	scale.Set((*it)->scaling.x, (*it)->scaling.y, (*it)->scaling.z);

	//	ImGui::Text("Name: %s", (*it)->name);
	//	ImGui::Text("Num. vertices: %u", (*it)->num_index);
	//	ImGui::Text("");
	//	ImGui::InputFloat3("Pos", &pos, 2);
	//	ImGui::InputFloat3("Rot", &rot, 2);
	//	ImGui::InputFloat3("Scale", &scale, 2);
	//	ImGui::Separator();
	//}
}

void ModuleRenderer3D::TexProperties()//same as the function above
{
	ImGui::Text("Texture id: %u", App->tex_loader->texture_id);
	ImGui::Text("Used in %u meshes", App->tex_loader->num_meshes);
	ImGui::Text("Width: %u", App->tex_loader->tex_width); ImGui::SameLine();
	ImGui::Text("Height: %u", App->tex_loader->tex_height);
}

void ModuleRenderer3D::CleanPrimitives()
{
	for (std::vector<NOTprimitive*>::iterator it = primitive_vector.begin(); it != primitive_vector.end(); it++)
	{
		delete(*it);
	}
	primitive_vector.clear();
}

void ModuleRenderer3D::Save(rapidjson::Document* d, rapidjson::Value* v)
{
	rapidjson::Document::AllocatorType& all = d->GetAllocator();

	rapidjson::Value module_obj(rapidjson::kObjectType);

	module_obj.AddMember("draw_mode",conf_draw, all);
	module_obj.AddMember("cull_face", conf_cull_face, all);
	module_obj.AddMember("depth_test", conf_depth_test, all);
	module_obj.AddMember("lighting", conf_lighting, all);
	module_obj.AddMember("texture", conf_texture, all);

	v->AddMember(rapidjson::GenericStringRef<char>::GenericStringRef(name.c_str()), module_obj, all);

}

void ModuleRenderer3D::Load(rapidjson::Value& v)
{
	rapidjson::Value& conf = v[name.data()];

	conf_cull_face = conf["cull_face"].GetBool();
	conf_depth_test = conf["depth_test"].GetBool();
	conf_draw = conf["draw_mode"].GetInt();
	conf_lighting = conf["lighting"].GetBool();
	conf_texture = conf["texture"].GetInt();
}

void ModuleRenderer3D::RecieveEvent(Event& ev)
{
	if (ev.type == Event::window_resize)
	{
		OnResize(ev.point2d.x, ev.point2d.y);
	}
}
