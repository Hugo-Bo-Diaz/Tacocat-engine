#include "AudioEmitterComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleRenderer3D.h"



Component_Audio_Emitter::Component_Audio_Emitter()
{
	
}

void Component_Audio_Emitter::SetWwiseObject()
{
	Wwise_obj = parent->UID;
	name = parent->name;

	AKRESULT eResult = AK::SoundEngine::RegisterGameObj(Wwise_obj, name.c_str());
}

void Component_Audio_Emitter::Update(float dt)
{
	if (attached)
	{

	}
}
void Component_Audio_Emitter::CalculatePositionWithObjectTransform()
{
	float3 pos;
	pos = parent->GetTransformComponent()->GetGlobalPosition();

	AkSoundPosition p;
	p.SetPosition(-pos.x,pos.y,-pos.z);

	//front.X = x_front;
	//front.Y = y_front;
	//front.Z = z_front;
	//top.X = x_top;
	//top.Y = y_top;
	//top.Z = z_top;
	//float length_front = sqrt(pow(front.X, 2) + pow(front.Y, 2) + pow(front.Z, 2));
	//float length_top = sqrt(pow(top.X, 2) + pow(top.Y, 2) + pow(top.Z, 2));
	////Normalize vectors
	//front.X = front.X / length_front;
	//front.Y = front.Y / length_front;
	//front.Z = front.Z / length_front;
	//top.X = top.X / length_top;
	//top.Y = top.Y / length_top;
	//top.Z = top.Z / length_top;
	////Check if the are orthogonals
	//float dot_prod = top.X*front.X + top.Y*front.Y + top.Z*front.Z;
	//if (dot_prod >= 0.0001)
	//	LOG("Vectors are not orthogonal!");

	AKRESULT res = AK::SoundEngine::SetPosition((AkGameObjectID)Wwise_obj, position);
}

void Component_Audio_Emitter::PlayEvent(unsigned long int _event)
{

	if (AK::SoundEngine::PostEvent(_event, Wwise_obj) == AK_INVALID_PLAYING_ID)
	{
		assert(!"invalid playing id");
	}
}

void Component_Audio_Emitter::Properties()
{
	if (ImGui::CollapsingHeader("Sound Emitter"))
	{
		if (ImGui::Button("Play"))
		{
			PlayEvent(779278001U);
		}
	}
}
