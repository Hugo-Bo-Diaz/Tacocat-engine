#include "ModuleAudio.h"
#include "Application.h"

#include <AK/SoundEngine/Common/AkMemoryMgr.h>                  // Memory Manager
#include <AK/SoundEngine/Common/AkModule.h>                     // Default memory and stream managers
#include <AK/SoundEngine/Common/IAkStreamMgr.h>                 // Streaming Manager
#include <AK/Tools/Common/AkPlatformFuncs.h>                    // Thread defines
#include <AK/SoundEngine/Win32/AkFilePackageLowLevelIOBlocking.h> // Sample low-level I/O implementation

#ifndef AK_OPTIMIZED
	#include <AK/Comm/AkCommunication.h>
#endif // AK_OPTIMIZED

CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

ModuleAudio::ModuleAudio(bool start_enabled)
{
}


ModuleAudio::~ModuleAudio()
{
	TermSoundEngine();
}

bool ModuleAudio::Init()
{
	//
	// Create and initialize an instance of the default memory manager. Note
	// that you can override the default memory manager with your own. Refer
	// to the SDK documentation for more information.
	//

	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 20;

	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{
		App->UI->console->AddLog("Could not create the memory manager.");
		assert(!"Could not create the memory manager.");
		return false;
	}

	// Initializing the Streaming Manager
	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);

	// Customize the Stream Manager settings here
	if (!AK::StreamMgr::Create(stmSettings))
	{
		assert(!"Could not create the Streaming Manager");
		return false;
	}
	// -------------------
	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);
	if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		assert(!"Could not create the streaming device and Low-Level I/O system");
		return false;
	}

	//
	// Create the Sound Engine
	// Using default initialization parameters
	//

	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		assert(!"Could not initialize the Sound Engine.");
		return false;
	}

	// In case we need interactive music ----------------------------------------------------------------------

	//
	// Initialize the music engine
	// Using default initialization parameters
	//

	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	if (AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		assert(!"Could not initialize the Music Engine.");
		return false;
	}

	// ----------------------------------------------------------------------------------------------------------

	// Initialize communication with wwise -(DO NOT USE IN RELEASE)----------------------------------------------

#ifndef AK_OPTIMIZED
	//
	// Initialize communications (not in release build!)
	//
	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);
	if (AK::Comm::Init(commSettings) != AK_Success)
	{
		assert(!"Could not initialize communication.");
		return false;
	}
#endif // AK_OPTIMIZED

	// -----------------------------------------------------------------------------------------------------------
	
	LoadBank("Sound/Init.bnk");
	LoadBank("Sound/audio_demo.bnk");

	return true;
}

AkBankID ModuleAudio::LoadBank(const char * name)
{
	AkBankID bankID;
	AKRESULT eResult = AK::SoundEngine::LoadBank(name, AK_DEFAULT_POOL_ID, bankID);
	if (eResult == AK_WrongBankVersion)
	{
		assert(!"WrongBankVersion!");
		return false;
	}
	else if (eResult != AK_Success)
	{
		assert(!"Could not load soundbank!");
		return false;
	}
	return bankID;
}

update_status ModuleAudio::Update(float dt)
{
	AK::SoundEngine::RenderAudio();
	return UPDATE_CONTINUE;
}

void ModuleAudio::ProcessingAudio()
{
	// Process bank requests, events, positions, RTPC, etc.
	AK::SoundEngine::RenderAudio();
}

void ModuleAudio::TermSoundEngine()
{
#ifndef AK_OPTIMIZED
	//
	// Terminate Communication Services
	//
	AK::Comm::Term();
#endif // AK_OPTIMIZED

	//
	// Terminate the music engine
	//

	AK::MusicEngine::Term();

	//
	// Terminate the sound engine
	//

	AK::SoundEngine::Term();

	// Terminate the streaming device and streaming manager

	// CAkFilePackageLowLevelIOBlocking::Term() destroys its associated streaming device 
	// that lives in the Stream Manager, and unregisters itself as the File Location Resolver.
	g_lowLevelIO.Term();

	if (AK::IAkStreamMgr::Get())
		AK::IAkStreamMgr::Get()->Destroy();

	// Terminate the Memory Manager
	AK::MemoryMgr::Term();

}
