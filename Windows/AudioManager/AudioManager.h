
#include "VistaAudio.h"
#include "IAudio.h"
#include "VistaAudio.h"
#include <Windows.h>

using namespace System;
using namespace System::Threading;

public ref class AudioManager {

private:

    VistaAudio* _Audio;
    BOOL _IsVista;


public:
    AudioManager();

    void SetVolume(float volume);
    float GetVolume();
    void SetMute(bool mute);
    BOOL GetMute();
    int GetChannelCount();
    float GetPeak();

    ~AudioManager();

};