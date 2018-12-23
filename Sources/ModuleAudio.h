#ifndef AUDIO_CLASS
#define AUDIO_CLASS

#include "Module.h"
#include "Application.h"

#include "Wwise_include.h"

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