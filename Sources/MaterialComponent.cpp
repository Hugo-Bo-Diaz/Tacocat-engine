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

void Component_Material::Save_Component(rapidjson::Document* d, rapidjson::Value* v)
{
	rapidjson::Document::AllocatorType& all = d->GetAllocator();

	rapidjson::Value module_obj(rapidjson::kObjectType);

	module_obj.AddMember("tex_width", tex_width, all);
	module_obj.AddMember("tex_height", tex_height, all);

	v->AddMember("MATERIAL", module_obj, all);
}

void Component_Material::Load_Component(rapidjson::Value& v)
{

}