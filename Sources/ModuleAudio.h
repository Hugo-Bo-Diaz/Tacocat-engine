#ifndef AUDIO_CLASS
#define AUDIO_CLASS

#include "Module.h"
#include <list>

#include "Wwise_include.h"

class ModuleAudio :	public Module
{
public:
	ModuleAudio(bool start_enabled = true);
	~ModuleAudio();

	bool Init();

	AkBankID LoadBank(const char* name);

	update_status Update(float dt);

	std::list<AkBankID> banks;

	void ProcessingAudio(); //Recurring calls to perform audio processing

	void TermSoundEngine(); //Terminate sound engine

	bool CleanUp();

};

#endif // !AUDIO_CLASS