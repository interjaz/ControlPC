#include "StdAfx.h"
#include "VistaAudio.h"

VistaAudio::VistaAudio(
    UINT uNotificationMessage,
    HWND hwndNotification
    )
    : _cRef(1),
      _uNotificationMessage(uNotificationMessage),
      _hwndNotification(hwndNotification),
      _bNotificationsEnabled(FALSE),
      _pAudioSession(NULL),
      _pEndpointVolume(NULL),
      _AudioMeterInformation(NULL) {
}

VistaAudio::~VistaAudio() {
    SafeRelease(&_pAudioSession);
    SafeRelease(&_pEndpointVolume);
    SafeRelease(&_AudioMeterInformation);
};

HRESULT VistaAudio::CreateInstance(
    UINT uNotificationMessage,
    HWND hwndNotification,
    VistaAudio **ppVistaAudio
    ) {

    VistaAudio *pVistaAudio = new (std::nothrow)
        VistaAudio(uNotificationMessage, hwndNotification);

    if (pVistaAudio == NULL) {
        return E_OUTOFMEMORY;
    }

    HRESULT hr = pVistaAudio->Initialize();
    if (SUCCEEDED(hr)) {
        *ppVistaAudio = pVistaAudio;
    } else {
        pVistaAudio->Release();
    }

    return hr;
}

HRESULT VistaAudio::Initialize() {
    HRESULT hr = S_OK;

    IMMDeviceEnumerator *pDeviceEnumerator = NULL;
    IMMDevice *pDevice = NULL;

    hr = CoCreateInstance(
         __uuidof(MMDeviceEnumerator), NULL,
         CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator),
         (void**)&pDeviceEnumerator);

    if (FAILED(hr)) {
        goto done;
    }
    
    // Get the default audio endpoint that the SAR will use.
    hr = pDeviceEnumerator->GetDefaultAudioEndpoint(
        eRender,
        eConsole,   // The SAR uses 'eConsole' by default.
        &pDevice
        );
    
    if (FAILED(hr)) {
        goto done;
    }

    // Get volume control
    hr = pDevice->Activate(
        __uuidof(IAudioEndpointVolume),
        CLSCTX_ALL,
        NULL,
        (void**) &_pEndpointVolume
        );

    if (FAILED(hr)) {
        goto done;
    }

    // Get volume observer

    hr = pDevice->Activate(
        __uuidof(IAudioMeterInformation),
        CLSCTX_ALL,
        NULL,
        (void**) &_AudioMeterInformation
        );

    if (FAILED(hr)) {
        goto done;
    }

done:
    SafeRelease(&pDeviceEnumerator);
    SafeRelease(&pDevice);
    return hr;
}

STDMETHODIMP VistaAudio::QueryInterface(REFIID riid, void **ppv) {
    static const QITAB qit[] =
    {
        QITABENT(VistaAudio, IAudioSessionEvents),
        { 0 },
    };
    // return QISearch(this, qit, riid, ppv);
    return NULL;
}

STDMETHODIMP_(ULONG) VistaAudio::AddRef() {
    return InterlockedIncrement(&_cRef);
}

STDMETHODIMP_(ULONG) VistaAudio::Release() {
    LONG cRef = InterlockedDecrement( &_cRef );
    if (cRef == 0)
    {
        delete this;
    }
    return cRef;
}

float VistaAudio::GetVolume() {
    float volume = 0;
    if (_pEndpointVolume != NULL) {
        _pEndpointVolume->GetMasterVolumeLevelScalar(&volume);
    }

    return volume;
}

void VistaAudio::SetVolume(float volume) {
    if (_pEndpointVolume != NULL) {
        
        _pEndpointVolume->SetMasterVolumeLevelScalar(
            volume,
            &GUID_NULL
            );
    }
}

BOOL VistaAudio::GetMute() {
    BOOL mute = FALSE;
    if (_pEndpointVolume != NULL) {        
        _pEndpointVolume->GetMute(&mute);
    }

    return mute;
}
void VistaAudio::SetMute(BOOL mute) {
    if (_pEndpointVolume != NULL) {

        _pEndpointVolume->SetMute(
            mute,
            &GUID_NULL  // Event context.
            );
    }
}

int VistaAudio::GetChannelCount() {
    UINT channels = 0;

    if(_AudioMeterInformation != NULL) {
        _AudioMeterInformation->GetMeteringChannelCount(&channels);
    }

    return (int)channels;
}

float VistaAudio::GetPeak() {
    float peak = 0;

    if(_AudioMeterInformation != NULL) {
        _AudioMeterInformation->GetPeakValue(&peak);
    }

    return peak;
}