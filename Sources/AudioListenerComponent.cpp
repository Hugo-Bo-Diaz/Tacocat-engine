#include "AudioListenerComponent.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

Component_Audio_Listener::Component_Audio_Listener()
{
}

void Component_Audio_Listener::CalculatePositionWithObjectTransform()
{
	float3 pos;
	pos = parent->GetTransformComponent()->GetGlobalPosition();

	AkSoundPosition p;
	p.SetPosition(-pos.x, pos.y, -pos.z);
	App->UI->console->AddLog("%f, %f, %f", pos.x, pos.y, pos.z);

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

	AKRESULT res = AK::SoundEngine::SetPosition(Wwise_obj, p);
}

void Component_Audio_Listener::SetWwiseObject()
{
	Wwise_obj = parent->UID;
	name = parent->name;

	AK::SoundEngine::RegisterGameObj(Wwise_obj, name.c_str());
	AK::SoundEngine::SetDefaultListeners(&Wwise_obj, 1);

	AkSoundPosition position;
	AkVector k, v;
	k.X = 1;
	k.Z = 1;
	k.Y = 1;
	v.X = 1;
	v.Y = 1;
	v.Z = 1;
	position.SetOrientation(k, v);
	position.SetPosition(0, 0, 0);
	AKRESULT res = AK::SoundEngine::SetPosition(Wwise_obj, position);
}

void Component_Audio_Listener::Update(float dt)
{
	if (attached)
	{
		CalculatePositionWithObjectTransform();
	}

	//AkAuxSendValue reverb;
	//reverb.listenerID = Wwise_obj;
	//reverb.auxBusID = AK::SoundEngine::GetIDFromString("tunnel");
	//reverb.fControlValue = 20;

	//AKRESULT res = AK::SoundEngine::SetGameObjectAuxSendValues(Wwise_obj, &reverb, 1);
}

void Component_Audio_Listener::Properties()
{
	if (ImGui::CollapsingHeader("Sound Listener"))
	{
		ImGui::Text("Currently listening to sounds on the environment at the current location");
	}
}