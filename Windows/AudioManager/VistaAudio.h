#pragma once

#include <audiopolicy.h>
#include <mmdeviceapi.h>
#include <unknwn.h>
#include <shlwapi.h>
#include <new>
#include <Endpointvolume.h>
#include "IAudio.h"

class VistaAudio : public IAudioSessionEvents, public IAudio {

public:
    // Static method to create an instance of the object.
    static HRESULT CreateInstance(
        UINT uNotificationMessage,
        HWND hwndNotification,
        VistaAudio **ppAudioSessionVolume
    );

    // IUnknown methods.
    STDMETHODIMP QueryInterface(REFIID iid, void** ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    // IAudioSessionEvents methods.
    STDMETHODIMP OnSimpleVolumeChanged(
        float NewVolume,
        BOOL NewMute,
        LPCGUID EventContext
        ) {
        return S_OK;
    }

    // The remaining audio session events do not require any action.
    STDMETHODIMP OnDisplayNameChanged(LPCWSTR,LPCGUID) {
        return S_OK;
    }

    STDMETHODIMP OnIconPathChanged(LPCWSTR,LPCGUID) {
        return S_OK;
    }

    STDMETHODIMP OnChannelVolumeChanged(DWORD,float[],DWORD,LPCGUID) {
        return S_OK;
    }

    STDMETHODIMP OnGroupingParamChanged(LPCGUID,LPCGUID) {
        return S_OK;
    }

    STDMETHODIMP OnStateChanged(AudioSessionState) {
        return S_OK;
    }

    STDMETHODIMP OnSessionDisconnected(AudioSessionDisconnectReason)  {
        return S_OK;
    }

    template <class T> void SafeRelease(T **ppT) {
        if (*ppT)
        {
            (*ppT)->Release();
            *ppT = NULL;
        }
    };

    // IAudio
    float GetVolume();
    void SetVolume(float volume);

    BOOL GetMute();
    void SetMute(BOOL mute);

    int GetChannelCount();
    float GetPeak();
    float GetPeak(int channel);

protected:
    VistaAudio(UINT uNotificationMessage, HWND hwndNotification);
    ~VistaAudio();

    HRESULT Initialize();

protected:
    LONG _cRef;                        // Reference count.
    UINT _uNotificationMessage;        // Window message to send when an audio event occurs.
    HWND _hwndNotification;            // Window to receives messages.
    BOOL _bNotificationsEnabled;       // Are audio notifications enabled?

    IAudioSessionControl    *_pAudioSession;
    IAudioEndpointVolume    *_pEndpointVolume;
    IAudioMeterInformation  *_AudioMeterInformation; 
};