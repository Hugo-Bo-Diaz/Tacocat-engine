#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>


#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")

ModuleRenderer3D::ModuleRenderer3D( bool start_enabled) : Module(start_enabled)
{
	name = "Render3D";
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

	if(ret == true)
	{
		//Use Vsync
		//if(VSYNC &&  < 0)//TODO
		//CONSOLE_LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
		SDL_GL_SetSwapInterval(0);
		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			CONSOLE_LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			CONSOLE_LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		/*glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);*/

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			CONSOLE_LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
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
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	// Projection matrix for
	OnResize(App->window->width, App->window->height);

	glGenBuffers(1, (GLuint*) &(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*36 * 3, vertices, GL_STATIC_DRAW);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity(); 

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();


	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	App->scene_controller->Draw();


	//glLineWidth(1.0f);
	//glBegin(GL_LINES);
	//float d = 200.0f;

	//for (float i = -d; i <= d; i += 1.0f)
	//{
	//	glVertex3f(i, 0.0f, -d);
	//	glVertex3f(i, 0.0f, d);
	//	glVertex3f(-d, 0.0f, i);
	//	glVertex3f(d, 0.0f, i);
	//}

	//glEnd();

	//draw a line

	glLineWidth(2.0f);
	glBegin(GL_LINES);  
	glVertex3f(0.f, 0.f, 0.f);  
	glVertex3f(0.f, 10.f, 0.f); 
	glEnd();
	glLineWidth(1.0f);

	//glVertex3f(0.5f, 0.5f, 0.5f);

	/* Quad vertices
	glVertex3f(0.5f, -0.5f, -0.5f); // A
	glVertex3f(0.5f, 0.5f, -0.5f); // B
	glVertex3f(0.5f, -0.5f, 0.5f); // C
	glVertex3f(0.5f, 0.5f, 0.5f); // D
	glVertex3f(-0.5f, -0.5f, -0.5f); // E
	glVertex3f(-0.5f, 0.5f, -0.5f); // F
	glVertex3f(-0.5f, -0.5f, 0.5f); // G
	glVertex3f(-0.5f, 0.5f, 0.5f); // H
	*/

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// ... draw other buffers
	glDrawArrays(GL_TRIANGLES, 0, 36 * 3);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	////draw a quad
	//glLineWidth(2.0f);
	//glBegin(GL_TRIANGLES);
	////Front face
	//	//pt 1
	//glVertex3f(0.5f, -0.5f, -0.5f); // A
	//glVertex3f(0.5f, 0.5f, -0.5f); // B
	//glVertex3f(0.5f, 0.5f, 0.5f); // D
	//	//pt 2
	//glVertex3f(0.5f, 0.5f, 0.5f); // D
	//glVertex3f(0.5f, -0.5f, 0.5f); // C
	//glVertex3f(0.5f, -0.5f, -0.5f); // A
	////Right face in truth it's the top
	//	//pt 1
	//glVertex3f(0.5f, 0.5f, -0.5f); // B
	//glVertex3f(-0.5f, 0.5f, -0.5f); // F
	//glVertex3f(-0.5f, 0.5f, 0.5f); // H
	//	//pt 2
	//glVertex3f(-0.5f, 0.5f, 0.5f); // H
	//glVertex3f(0.5f, 0.5f, 0.5f); // D
	//glVertex3f(0.5f, 0.5f, -0.5f); // B
	////Back face
	//	//pt 1
	//glVertex3f(-0.5f, 0.5f, -0.5f); // F
	//glVertex3f(-0.5f, -0.5f, -0.5f); // E
	//glVertex3f(-0.5f, -0.5f, 0.5f); // G
	//	//pt 2
	//glVertex3f(-0.5f, -0.5f, 0.5f); // G
	//glVertex3f(-0.5f, 0.5f, 0.5f); // H
	//glVertex3f(-0.5f, 0.5f, -0.5f); // F
	////Left face acshually its bottom
	//	//pt 1
	//glVertex3f(-0.5f, -0.5f, -0.5f); // E
	//glVertex3f(0.5f, -0.5f, -0.5f); // A
	//glVertex3f(0.5f, -0.5f, 0.5f); // C
	//	//pt 2
	//glVertex3f(0.5f, -0.5f, 0.5f); // C
	//glVertex3f(-0.5f, -0.5f, 0.5f); // G
	//glVertex3f(-0.5f, -0.5f, -0.5f); // E
	////Top ???
	//	//pt 1
	//glVertex3f(0.5f, -0.5f, 0.5f); // C
	//glVertex3f(0.5f, 0.5f, 0.5f); // D
	//glVertex3f(-0.5f, 0.5f, 0.5f); // H

	//glVertex3f(-0.5f, 0.5f, 0.5f); // H
	//glVertex3f(-0.5f, -0.5f, 0.5f); // G
	//glVertex3f(0.5f, -0.5f, 0.5f); // C
	////Bottom ???
	//	//pt 1
	//glVertex3f(-0.5f, -0.5f, -0.5f); // E
	//glVertex3f(-0.5f, 0.5f, -0.5f); // F
	//glVertex3f(0.5f, 0.5f, -0.5f); // B

	//glVertex3f(0.5f, 0.5f, -0.5f); // B
	//glVertex3f(0.5f, -0.5f, -0.5f); // A
	//glVertex3f(-0.5f, -0.5f, -0.5f); // E

	//glEnd();


	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	CONSOLE_LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	//glViewport(0, 0, width, height);

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//ProjectionMatrix.Perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	////glLoadMatrixf(float4x4_to_float(ProjectionMatrix));
	////float ret[16] =
	////{	ProjectionMatrix[0][0], ProjectionMatrix[0][1], ProjectionMatrix[0][2], ProjectionMatrix[0][3],
	////	ProjectionMatrix[1][0], ProjectionMatrix[1][1], ProjectionMatrix[1][2], ProjectionMatrix[1][3],
	////	ProjectionMatrix[2][0], ProjectionMatrix[2][1], ProjectionMatrix[2][2], ProjectionMatrix[2][3],
	////	ProjectionMatrix[3][0], ProjectionMatrix[3][1], ProjectionMatrix[3][2], ProjectionMatrix[3][3] };

	//float ret[16] =
	//{ ProjectionMatrix[0][0], ProjectionMatrix[1][0], ProjectionMatrix[2][0], ProjectionMatrix[3][0],
	//	ProjectionMatrix[0][1], ProjectionMatrix[1][1], ProjectionMatrix[2][1], ProjectionMatrix[3][1],
	//	ProjectionMatrix[0][2], ProjectionMatrix[1][2], ProjectionMatrix[2][2], ProjectionMatrix[3][2],
	//	ProjectionMatrix[0][3], ProjectionMatrix[1][3], ProjectionMatrix[2][3], ProjectionMatrix[3][3] };

	//glLoadMatrixf(ret);


	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

float* ModuleRenderer3D::float4x4_to_float(float4x4 to_change)
{
	float ret[16] = {to_change[0][0],to_change[0][1], to_change[0][2], to_change[0][3], 
					to_change[1][0], to_change[1][1], to_change[1][2], to_change[1][3], 
					to_change[2][0], to_change[2][1], to_change[2][2], to_change[2][3], 
					to_change[3][0], to_change[3][1], to_change[3][2], to_change[3][3]};
	return ret;
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
}
void ModuleRenderer3D::disable_flag_texture_2D()
{
	glDisable(GL_TEXTURE_2D);
}