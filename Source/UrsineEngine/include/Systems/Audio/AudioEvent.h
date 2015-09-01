/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AudioEvent.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <string>
#include "AudioHelpers.h"

namespace ursine
{
    enum PlayMode
    {
        PLAY_FIRE_FORGET, // Multiple instances overlap
        PLAY_LOOPING, // Looping instance (single instance only)
        PLAY_START_RESTART // Only one instance allowed
    };

    class AudioEvent
    {
        // the emitter has the rights to change things
        // such as volume, pitch ect.
        friend class AudioEmitter;

    private:
        PlayMode _mode;
        fmod_s::EventDescription **_description;
        fmod_s::EventInstance *_instance;
        bool _playing;
        float _volume;
        float _pitch;

        void playFireForget(); // FIRE_FORGET
        void playLoop(); // LOOPING
        void playStartRestart(); // START_RESTART
        void copy(const AudioEvent &other);
        void play();
        void play(PlayMode play_mode);
        void stop(bool fade_out);
        void update(float dt);
    public:
        AudioEvent();
        ~AudioEvent();
        AudioEvent(const AudioEvent &other);
        AudioEvent &operator=(const AudioEvent &rhs);
    };
}