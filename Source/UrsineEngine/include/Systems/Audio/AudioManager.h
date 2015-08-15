//----------------------------------------
//  File: SoundManager.h
//  Purpose: Contains the SoundManager interface.
//  Author: Jordan Ellis
//  Date: 5/2/2014
// ----------------------------------------------
#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "AudioEvent.h"
#include "System.h"

namespace Ursine
{
    class AudioManager : public System
    {
        friend class AudioEmitter;

        float _master_volume;

        // master volume before the window lost focus last
        float _last_master_volume;

        // The GUID file
        const char *_fGUID;
        // The FMOD system pointer
        FMOD_S::System *_system;
        FMOD::System *_low_system;
        // The hash map containing all playable events
        std::unordered_map<std::string, FMOD_S::EventDescription*> _event_desc_map;
        std::vector<AudioEmitter*> _loading_queue;

        // These are used for direct playback of events without needing an emitter
        std::unordered_map<std::string, AudioEmitter *> _global_events;

        void registerEvent(AudioEmitter *emitter);
        void unregisterEvent(AudioEmitter *emitter);

        void onFocusChanged(EVENT_HANDLER(WindowManager));

    public:
        AudioManager(const char *guid_file = "GUIDs.txt");
        ~AudioManager();

        void Update(void) override;
        void StopAll(bool fade_out);

        float GetMasterVolume(void) const;
        void SetMasterVolume(float volume);

        void PlayGlobalEvent(const std::string &event_name, 
                             int loading_priority = 1, 
                             PlayMode mode = PLAY_START_RESTART);

        void StopGlobalEvent(const std::string &event_name, bool allow_fade_out = true);

        bool IsGlobalEventPlaying(const std::string &event_name);

        // get the GUID containing file's name
        inline const char *GetfGUID() const;

        // Debug output
        friend std::ostream &operator<<(std::ostream &stream, const AudioManager &sound_manager);
    };

    extern AudioManager *gAudioManager;
}

#include "AudioManager.hpp"