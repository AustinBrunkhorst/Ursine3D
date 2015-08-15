/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AudioEmitter.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "AudioManager.h"
#include <DERPFile.h>
#include <AudioDataBaseClasses.h>

namespace Ursine
{
    class AudioEmitter
    {
        friend class AudioManager;

        std::string _event_name;
        bool _play_on_creation;
        bool _mute;
        int _weight;
        AudioEvent _event;
        DERPFile *_derp;
        PlayMode _mode;
        // attributes for 3D 
        FMOD_3D_ATTRIBUTES _attributes;

        void initEvent(FMOD_S::EventDescription **description);
        void copy(const AudioEmitter &other);

    public:
        AudioEmitter(const std::string &event_name,
            int loading_priority = 1,
            bool play_on_creation = false,
            PlayMode mode = PLAY_START_RESTART);
        AudioEmitter();
        ~AudioEmitter();
        AudioEmitter(const AudioEmitter &other);
        AudioEmitter &operator=(const AudioEmitter &rhs);

        void Play();
        void Play(PlayMode mode);
        void Stop(bool allow_fade_out = true);
        void Update(float dt);

        bool IsPlaying(void);

        void SetEvent(const char *event_name);
        void SetPitch(float pitch);
        void SetVolume(float volume);
        void SetMute(bool mute);
        void SetMode(PlayMode mode);
        void SetTimeline(int timeline);
        void SetDerpFile(const char *file_name);
        // value should be a 0 - 1 value (it will be interpreted by this function)
        void SetParameter(AudioParam &param, float value);
        void Set3DParameter(const Vector3 &listener, const Vector3 &emitter);

        inline const std::string &GetEvent() const;
        inline int GetWeight() const;
        inline float GetPitch() const;
        inline float GetVolume() const;
        inline bool GetMute() const;
        inline PlayMode GetMode() const;
        inline float GetDerpVal();
    };
}

#include "AudioEmitter.hpp"