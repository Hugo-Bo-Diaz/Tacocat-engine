#include "TransformComponent.h"

#include "Application.h"
#include "ModuleRenderer3D.h"

#include <limits>

Component_Transform::Component_Transform()
{
	type = TRANSFORM;

}


void Component_Transform::Calculate_Global_Matrix(float4x4 globalmatrixoftheobject)
{
	transform_global = globalmatrixoftheobject * transform_local;
}

Component_Transform::~Component_Transform()
{

}

void Component_Transform::Save_Component(rapidjson::Document* d, rapidjson::Value* v)
{

}

void Component_Transform::Load_Component(rapidjson::Value& v)
{

}

void Component_Transform::Properties()
{
	float3 Position, Rotation, Scale;

	Position = { position[0], position[1], position[2] };
	Rotation = { RadToDeg(rotation.GetEuler().x), RadToDeg(rotation.GetEuler().y), RadToDeg(rotation.GetEuler().z)};
	Scale = { scaling[0], scaling[1], scaling[2] };

	if(ImGui::CollapsingHeader("Transform"))
	{
		ImGui::DragFloat3("Pos", &Position[0], 0.2f, -100.0f, 100.0f);
		ImGui::DragFloat3("Rot", &Rotation[0], 0.01f, -359.9f, 360.0f);
		ImGui::DragFloat3("Scale", &Scale[0], 0.2f, -100.0f, 100.0f);
		ImGui::Separator();

		if (Position.x != position[0] || Position.y != position[1] || Position.z != position[2])
		{
			position.Set(Position[0], Position[1], Position[2]);

		}

		if (Rotation.x != RadToDeg(rotation.GetEuler().x) || Rotation.y != RadToDeg(rotation.GetEuler().y) || Rotation.z != RadToDeg(rotation.GetEuler().z))
		{
			Quat quaternion = quaternion.FromEulerXYZ(Rotation.x, Rotation.y, Rotation.z);
			rotation.x = quaternion.x;
			rotation.y = quaternion.y;
			rotation.z = quaternion.z;
			rotation.w = quaternion.w;
		}

		if (Scale.x != scaling[0] || Scale.y != scaling[1] || Scale.z != scaling[2])
		{
			scaling.Set(Scale[0], Scale[1], Scale[2]);
		}

		float3 p, s;
		p.x = position.x;
		p.y = position.y;
		p.z = position.z;

		s.x = scaling.x;
		s.y = scaling.y;
		s.z = scaling.z;

		Quat r;
		r.x = rotation.x;
		r.y = rotation.y;
		r.z = rotation.z;
		r.w = rotation.w;

		transform_local = float4x4::FromTRS(p, r, s);
	}
}