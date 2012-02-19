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
      _pEndpointVolume(NULL)
{
}

VistaAudio::~VistaAudio()
{
    EnableNotifications(FALSE);

    SafeRelease(&_pAudioSession);
    SafeRelease(&_pEndpointVolume);
};


//  Creates an instance of the CVistaAudio object.

/* static */
HRESULT VistaAudio::CreateInstance(
    UINT uNotificationMessage,
    HWND hwndNotification,
    VistaAudio **ppVistaAudio
    )
{

    VistaAudio *pVistaAudio = new (std::nothrow)
        VistaAudio(uNotificationMessage, hwndNotification);

    if (pVistaAudio == NULL)
    {
        return E_OUTOFMEMORY;
    }

    HRESULT hr = pVistaAudio->Initialize();
    if (SUCCEEDED(hr))
    {
        *ppVistaAudio = pVistaAudio;
    }
    else
    {
        pVistaAudio->Release();
    }

    return hr;
}

HRESULT VistaAudio::Initialize() {
    HRESULT hr = S_OK;

    IMMDeviceEnumerator *pDeviceEnumerator = NULL;
    IMMDevice *pDevice = NULL;
    IAudioSessionManager *pAudioSessionManager = NULL;

    // Get the enumerator for the audio endpoint devices.
    /*hr = CoCreateInstance(
        __uuidof(MMDeviceEnumerator),
        NULL,
        CLSCTX_ALL,
        IID_PPV_ARGS(&pDeviceEnumerator)
        );*/

    hr = CoCreateInstance(
         __uuidof(MMDeviceEnumerator), NULL,
         CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator),
         (void**)&pDeviceEnumerator);

    if (FAILED(hr))
    {
        goto done;
    }
    
        // Get the default audio endpoint that the SAR will use.
    hr = pDeviceEnumerator->GetDefaultAudioEndpoint(
        eRender,
        eConsole,   // The SAR uses 'eConsole' by default.
        &pDevice
        );
    
    if (FAILED(hr))
    {
        goto done;
    }

    // Get the session manager for this device.
    hr = pDevice->Activate(
        __uuidof(IAudioEndpointVolume),
        CLSCTX_ALL,
        NULL,
        (void**) &_pEndpointVolume
        );

    if (FAILED(hr)) {
        goto done;
    }

    float pflVolume;
        if (_pEndpointVolume  == NULL)
    {

    }
    else
    {
        _pEndpointVolume->GetMasterVolumeLevelScalar(&pflVolume);
    }


done:
    SafeRelease(&pDeviceEnumerator);
    SafeRelease(&pDevice);
    SafeRelease(&pAudioSessionManager);
    return hr;
}

STDMETHODIMP VistaAudio::QueryInterface(REFIID riid, void **ppv)
{
    static const QITAB qit[] =
    {
        QITABENT(VistaAudio, IAudioSessionEvents),
        { 0 },
    };
    // return QISearch(this, qit, riid, ppv);
    return NULL;
}

STDMETHODIMP_(ULONG) VistaAudio::AddRef()
{
    return InterlockedIncrement(&_cRef);
}

STDMETHODIMP_(ULONG) VistaAudio::Release()
{
    LONG cRef = InterlockedDecrement( &_cRef );
    if (cRef == 0)
    {
        delete this;
    }
    return cRef;
}


// Enables or disables notifications from the audio session. For example, the
// application is notified if the user mutes the audio through the system
// volume-control program (Sndvol).

HRESULT VistaAudio::EnableNotifications(BOOL bEnable)
{
    HRESULT hr = S_OK;

    if (_hwndNotification == NULL || _pAudioSession == NULL)
    {
        return E_FAIL;
    }

    if (_bNotificationsEnabled == bEnable)
    {
        // No change.
        return S_OK;
    }

    if (bEnable)
    {
        hr = _pAudioSession->RegisterAudioSessionNotification(this);
    }
    else
    {
        hr = _pAudioSession->UnregisterAudioSessionNotification(this);
    }

    if (SUCCEEDED(hr))
    {
        _bNotificationsEnabled = bEnable;
    }

    return hr;
}


// Gets the session volume level.

HRESULT VistaAudio::GetVolume(float *pflVolume)
{
    if (_pEndpointVolume == NULL)
    {
        return E_FAIL;
    }
    else
    {
        return _pEndpointVolume->GetMasterVolumeLevelScalar(pflVolume);
    }
}

//  Sets the session volume level.
//
//  flVolume: Ranges from 0 (silent) to 1 (full volume)

HRESULT VistaAudio::SetVolume(float flVolume)
{
    if (_pEndpointVolume == NULL)
    {
        return E_FAIL;
    }
    else
    {
        return _pEndpointVolume->SetMasterVolumeLevelScalar(
            flVolume,
            &GUID_NULL  // Event context.
            );
    }
}


//  Gets the muting state of the session.

HRESULT VistaAudio::GetMute(BOOL *pbMute)
{
    if (_pEndpointVolume == NULL)
    {
        return E_FAIL;
    }
    else
    {
        return _pEndpointVolume->GetMute(pbMute);
    }
}

//  Mutes or unmutes the session audio.

HRESULT VistaAudio::SetMute(BOOL bMute)
{
    if (_pEndpointVolume == NULL)
    {
        return E_FAIL;
    }
    else
    {
        return _pEndpointVolume->SetMute(
            bMute,
            &GUID_NULL  // Event context.
            );
    }
}

//  Sets the display name for the session audio.

HRESULT VistaAudio::SetDisplayName(const WCHAR *wszName)
{
    if (_pAudioSession == NULL)
    {
        return E_FAIL;
    }
    else
    {
        return _pAudioSession->SetDisplayName(wszName, NULL);
    }
}


//  Called when the session volume level or muting state changes.
//  (Implements IAudioSessionEvents::OnSimpleVolumeChanged.)

HRESULT VistaAudio::OnSimpleVolumeChanged(
    float NewVolume,
    BOOL NewMute,
    LPCGUID EventContext
    )
{

    return S_OK;
}