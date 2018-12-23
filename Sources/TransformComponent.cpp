#include "TransformComponent.h"

#include "Application.h"
#include "ModuleRenderer3D.h"

#include <limits>

Component_Transform::Component_Transform()
{
	type = TRANSFORM;

}


float3 Component_Transform::GetGlobalPosition()
{
	float3 pos;
	float3 not;
	float3x4 needed;

	transform_global.Decompose(pos,needed,not);
	return pos;
}

void Component_Transform::Calculate_Global_Matrix(float4x4 globalmatrixoftheobject)
{
	transform_global = globalmatrixoftheobject * transform_local;
}

void Component_Transform::Caluculate_Local_Matrix()
{

	float3 p, s;
	p.x = position.x;
	p.y = position.y;
	p.z = position.z;

	s.x = scaling.x;
	s.y = scaling.y;
	s.z = scaling.z;


	transform_local = float4x4::FromTRS(p, rotation, s);
}



Component_Transform::~Component_Transform()
{

}

void Component_Transform::Save_Component(rapidjson::Document* d, rapidjson::Value* v)
{
	rapidjson::Document::AllocatorType& all = d->GetAllocator();

	rapidjson::Value module_obj(rapidjson::kObjectType);

	rapidjson::Value row1(rapidjson::kObjectType);

	row1.AddMember("0", transform_local[0][0], all);
	row1.AddMember("1", transform_local[1][0], all);
	row1.AddMember("2", transform_local[2][0], all);
	row1.AddMember("3", transform_local[3][0], all);
	module_obj.AddMember("row1", row1, all);

	rapidjson::Value row2(rapidjson::kObjectType);

	row2.AddMember("0", transform_local[0][1], all);
	row2.AddMember("1", transform_local[1][1], all);
	row2.AddMember("2", transform_local[2][1], all);
	row2.AddMember("3", transform_local[3][1], all);
	module_obj.AddMember("row2", row2, all);

	rapidjson::Value row3(rapidjson::kObjectType);

	row3.AddMember("0", transform_local[0][2], all);
	row3.AddMember("1", transform_local[1][2], all);
	row3.AddMember("2", transform_local[2][2], all);
	row3.AddMember("3", transform_local[3][2], all);
	module_obj.AddMember("row3", row3, all);

	rapidjson::Value row4(rapidjson::kObjectType);

	row4.AddMember("0", transform_local[0][3], all);
	row4.AddMember("1", transform_local[1][3], all);
	row4.AddMember("2", transform_local[2][3], all);
	row4.AddMember("3", transform_local[3][3], all);
	module_obj.AddMember("row4", row4, all);

	v->AddMember("TRANSFORM", module_obj, all);
}

void Component_Transform::Load_Component(rapidjson::Value::ConstMemberIterator v)
{

}

void Component_Transform::Properties()
{
	float3 Position, Rotation, Scale;

	Position = { position[0], position[1], position[2] };
	//Rotation = { RadToDeg(rotation.GetEuler().x), RadToDeg(rotation.GetEuler().y), RadToDeg(rotation.GetEuler().z)};
	Rotation = rotation.ToEulerXYZ();
	Scale = { scaling[0], scaling[1], scaling[2] };

	if(ImGui::CollapsingHeader("Transform"))
	{
		ImGui::DragFloat3("Pos", &Position[0], 0.2f, -100.0f, 100.0f);
		ImGui::DragFloat3("Rot", &Rotation[0], 0.01f);
		//ImGui::InputFloat3("Rot",&Rotation[0]);
		//Rotation.x += 0.1;
		ImGui::DragFloat3("Scale", &Scale[0], 0.2f, -100.0f, 100.0f);
		ImGui::Separator();

		if (Position.x != position[0] || Position.y != position[1] || Position.z != position[2])
		{
			position.Set(Position[0], Position[1], Position[2]);

		}

		//if (Rotation.x != rotation.GetEuler().x || Rotation.y != rotation.GetEuler().y || Rotation.z != rotation.GetEuler().z)
		if (Rotation.x != rotation.ToEulerXYZ().x || Rotation.y != rotation.ToEulerXYZ().y || Rotation.z != rotation.ToEulerXYZ().z)
		{
			//aiVector3D v;
			//v.x = Rotation.x;
			//v.y = Rotation.y;
			//v.z = Rotation.z;

			//rotation.Rotate(v);

			//Quat quaternion;

			//quaternion.x = rotation.x;
			//quaternion.y = rotation.y;
			//quaternion.z = rotation.z;
			//quaternion.w = rotation.w;
			rotation = rotation.FromEulerXYZ(Rotation.x, Rotation.y, Rotation.z);
			//rotation = rotation.RotateX(Rotation.x- rotation.ToEulerXYZ().x);
			//rotation = rotation.RotateY(Rotation.y- rotation.ToEulerXYZ().y);
			//rotation = rotation.RotateZ(Rotation.z- rotation.ToEulerXYZ().z);

			//quaternion.RotateX(pi);
			//quaternion.RotateY(Rotation.y);
			//quaternion.RotateZ(Rotation.z);

			////Quat quaternion = quaternion.FromEulerXYZ(Rotation.x, Rotation.y, Rotation.z);

			//rotation.x = quaternion.x;
			//rotation.y = quaternion.y;
			//rotation.z = quaternion.z;
			//rotation.w = quaternion.w;
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


		transform_local = float4x4::FromTRS(p, rotation, s);
	}
}

void Component_Transform::Rotate(float angle_x, float angle_y, float angle_z)
{
	//rotation.Rotate
}