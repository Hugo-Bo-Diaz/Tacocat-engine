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

void Component_Material::Save_Component(JSON_Object* root, const char* prev)
{
	std::string temp = prev;
	temp += ".type";
	json_object_dotset_number(root, temp.c_str(), (uint)type);

}

void Component_Material::Load_Component(JSON_Object* root, const char* prev)
{

}