#include "stdafx.h"
#include "AudioManager.h"

using namespace System;
using namespace System::Threading;

AudioManager::AudioManager() {

    System::OperatingSystem ^osInfo = System::Environment::OSVersion;

    _IsVista = osInfo->Version->Major >= 6;

    if(_IsVista) {
        VistaAudio* va;
        VistaAudio::CreateInstance(0,NULL, &va);
        _Audio = va;
    }
}

AudioManager::~AudioManager() {

}

void AudioManager::SetVolume(float volume) {

    if(_IsVista) {
        ((VistaAudio*)_Audio)->SetVolume(volume);
    } else {

    }
}

float AudioManager::GetVolume() {

    if(_IsVista) {
        return ((VistaAudio*)_Audio)->GetVolume();
    } else {
        return 0;
    }
}

void AudioManager::SetMute(bool mute) {

    if(_IsVista) {
        ((VistaAudio*)_Audio)->SetMute(mute);
    } else {

    }
}

BOOL AudioManager::GetMute() {

    if(_IsVista) {
        return ((VistaAudio*)_Audio)->GetMute();
    } else {
        return FALSE;
    }
}

int AudioManager::GetChannelCount() {

    if(_IsVista) {
        return ((VistaAudio*)_Audio)->GetChannelCount();
    } else {
        return 0;
    }
}

float AudioManager::GetPeak() {

    if(_IsVista) {
        return ((VistaAudio*)_Audio)->GetPeak();
    } else {
        return 0;
    }
}