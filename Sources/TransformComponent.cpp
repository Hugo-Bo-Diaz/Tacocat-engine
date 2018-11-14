#include "TransformComponent.h"

#include "Application.h"
#include "ModuleRenderer3D.h"

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
	Rotation = { rotation.GetEuler().x, rotation.GetEuler().y , rotation.GetEuler().z };
	Scale = { scaling[0], scaling[1], scaling[2] };

	if(ImGui::CollapsingHeader("Transform"));
	{
		ImGui::InputFloat3("Pos", &Position[0], 2);
		ImGui::InputFloat3("Rot", &Rotation[0], 2);
		ImGui::InputFloat3("Scale", &Scale[0], 2);
		ImGui::Separator();

		if (Position.x != position[0] || Position.y != position[1] || Position.z != position[2])
		{
			position.Set(position[0], position[1], position[2]);
		}

		if (Rotation.x != rotation.GetEuler().x || Rotation.y != rotation.GetEuler().y || Rotation.z != rotation.GetEuler().z)
		{
			Quat quaternion = quaternion.FromEulerXYZ(Rotation.x, Rotation.y, Rotation.z);
			rotation.x = quaternion.x;
			rotation.y = quaternion.y;
			rotation.z = quaternion.z;
			rotation.w = quaternion.w;
		}

		if (Scale.x != scaling[0] || Scale.y != scaling[1] || Scale.z != scaling[2])
		{
			scaling.Set(scaling[0], scaling[1], scaling[2]);
		}
	}

	ImGui::End();
}