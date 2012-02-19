#pragma once

class IAudio {

public:
    virtual void SetVolume(float volume) = 0;
    virtual float GetVolume() = 0;
    virtual void SetMute(BOOL mute) = 0;
    virtual BOOL GetMute() = 0;
    virtual int GetChannelCount() = 0;
    virtual float GetPeak() = 0;
};
