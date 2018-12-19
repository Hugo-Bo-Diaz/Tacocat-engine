#ifndef AUDIO_CLASS
#define AUDIO_CLASS

#include "Module.h"
#include "Application.h"

#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/MusicEngine/Common/AkMusicEngine.h>	  
#include <AK/SoundEngine/Common/AkMemoryMgr.h>    
#include <AK/SoundEngine/Common/AkModule.h>         
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>

#include <AK/Tools/Common/AkPlatformFuncs.h>


// Input libraries
#ifndef _DEBUG  // Profile build configuration must be loaded instead of Debug
#define AK_OPTIMIZED
#pragma comment( lib, "Wwise/lib/Release/lib/AkSoundEngine.lib")
#pragma comment( lib, "Wwise/lib/Release/lib/AkMusicEngine.lib")
#pragma comment( lib, "Wwise/lib/Release/lib/AkMemoryMgr.lib")
#pragma comment( lib, "Wwise/lib/Release/lib/AkStreamMgr.lib")
#pragma comment( lib, "Wwise/lib/Release/lib/AkRoomVerbFX.lib")
#else
#include <AK/Comm/AkCommunication.h>
#pragma comment( lib, "Wwise/lib/Debug/lib/CommunicationCentral.lib")
#pragma comment( lib, "Wwise/lib/ws2_32.lib") 
#pragma comment( lib, "Wwise/lib/Debug/lib/AkSoundEngine.lib")
#pragma comment( lib, "Wwise/lib/Debug/lib/AkMusicEngine.lib")
#pragma comment( lib, "Wwise/lib/Debug/lib/AkMemoryMgr.lib")
#pragma comment( lib, "Wwise/lib/Debug/lib/AkStreamMgr.lib")
#pragma comment( lib, "Wwise/lib/Debug/lib/AkRoomVerbFX.lib")
#endif

// Microsoft DirectX external libs
#pragma comment( lib, "Wwise/lib/dinput8.lib")
#pragma comment( lib, "Wwise/lib/dsound.lib") 
#pragma comment( lib, "Wwise/lib/dxguid.lib")  

/////////////////////////////////////////////////////////////////////////////////
// Custom alloc/free functions. These are declared as "extern" in AkMemoryMgr.h
// and MUST be defined by the game developer.
/////////////////////////////////////////////////////////////////////////////////
namespace AK
{
#ifdef WIN32
	void * AllocHook(size_t in_size)
	{
		return malloc(in_size);
	}
	void FreeHook(void * in_ptr)
	{
		free(in_ptr);
	}
	// Note: VirtualAllocHook() may be used by I/O pools of the default implementation
	// of the Stream Manager, to allow "true" unbuffered I/O (using FILE_FLAG_NO_BUFFERING
	// - refer to the Windows SDK documentation for more details). This is NOT mandatory;
	// you may implement it with a simple malloc().
	void * VirtualAllocHook(
		void * in_pMemAddress,
		size_t in_size,
		DWORD in_dwAllocationType,
		DWORD in_dwProtect
	)
	{
		return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
	}
	void VirtualFreeHook(
		void * in_pMemAddress,
		size_t in_size,
		DWORD in_dwFreeType
	)
	{
		VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
	}
#endif
}

class ModuleAudio :	public Module
{
public:
	ModuleAudio(bool start_enabled = true);
	~ModuleAudio();

	bool Init();

	void ProcessingAudio(); //Recurring calls to perform audio processing

	void TermSoundEngine(); //Terminate sound engine
};

#endif // !AUDIO_CLASS