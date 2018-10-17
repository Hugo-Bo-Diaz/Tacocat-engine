#include "MaterialComponent.h"

#include "Application.h"
#include "ModuleRenderer3D.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

Component_Material::Component_Material()
{
	type = MATERIAL;
}

//void Component_Mesh::generate_buffer()
//{
//
//
//}

Component_Material::~Component_Material()
{

	glDeleteTextures(1, &texture_buffer_id);

}