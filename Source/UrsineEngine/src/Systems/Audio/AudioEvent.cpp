/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AudioEvent.cpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "AudioEvent.h"

namespace ursine
{
    AudioEvent::AudioEvent()
        : _mode(PLAY_FIRE_FORGET)
        , _playing(false)
        , _instance(nullptr)
        , _description(nullptr)
        , _volume(1.0f)
        , _pitch(1.0f) {}

    AudioEvent::~AudioEvent() 
    {
        
    }

    AudioEvent::AudioEvent(const AudioEvent &other)
    {
        copy(other);
    }

    AudioEvent &AudioEvent::operator=(const AudioEvent &rhs)
    {
        copy(rhs);
        return (*this);
    }

    void AudioEvent::copy(const AudioEvent &other)
    {
        _mode = other._mode;
        _description = other._description;
        _instance = nullptr;
        _playing = false;
        _pitch = other._pitch;
        _volume = other._volume;
    }

    void AudioEvent::play(PlayMode play_mode)
    {
        _mode = play_mode;
        play();
    }

    void AudioEvent::play()
    {
        switch (_mode)
        {
            case PLAY_FIRE_FORGET:
                playFireForget();
                break;
            case PLAY_LOOPING:
                playLoop();
                break;
            case PLAY_START_RESTART:
                playStartRestart();
                break;
        }

        _playing = true;
    }

    void AudioEvent::stop(bool fade_out)
    {
        if (_instance)
        {
            FMOD_STUDIO_STOP_MODE stop_mode = fade_out ? FMOD_STUDIO_STOP_ALLOWFADEOUT :
                FMOD_STUDIO_STOP_IMMEDIATE;
            FMODERRCHK(_instance->stop(stop_mode));

            FMODERRCHK(_instance->release());

            _instance = nullptr;
            _playing = false;
        }
    }

    void AudioEvent::update(float dt)
    {
        if (_instance)
        {
            FMOD_STUDIO_PLAYBACK_STATE current_state;

            FMODERRCHK(_instance->getPlaybackState(&current_state));

            if (current_state == FMOD_STUDIO_PLAYBACK_IDLE ||
                current_state == FMOD_STUDIO_PLAYBACK_STOPPED)
            {
                if (_mode == PLAY_LOOPING)
                {
                    playLoop();
                    _playing = true;
                }
                else
                {
                    FMODERRCHK(_instance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT));
                    FMODERRCHK(_instance->release());
                    _instance = nullptr;
                    _playing = false;
                }
            }
        }
        else
            _playing = false;
    }

    void AudioEvent::playFireForget()
    {
        FMOD_S::EventInstance *temp_instance;

        FMODERRCHK((*_description)->createInstance(&temp_instance));

        float master_volume = gAudioManager->GetMasterVolume();

        FMODERRCHK(temp_instance->setVolume(_volume * master_volume));
        
        FMODERRCHK(temp_instance->setPitch(_pitch));
        
        // copy over the 3d attributes if needed
        bool is_3d;
        FMOD_3D_ATTRIBUTES attributes;
        FMODERRCHK((*_description)->is3D(&is_3d));
        if (is_3d)
        {
            FMODERRCHK(_instance->get3DAttributes(&attributes));
            FMODERRCHK(temp_instance->set3DAttributes(&attributes));
        }

        // copy over the parameter values
        int num_params = 0;
        FMODERRCHK(_instance->getParameterCount(&num_params));

        // trim this down for 3D.  This resolves
        // stupid FMOD errors.  fuck dat shit mane.
        URSINE_TODO("Investigate this shit");
        if (is_3d && num_params >= 2)
            num_params -= 2;

        for (int i = 0; i < num_params; ++i)
        {
            FMOD_S::ParameterInstance *param, *temp_param;
            FMODERRCHK(_instance->getParameterByIndex(i, &param));
            FMODERRCHK(temp_instance->getParameterByIndex(i, &temp_param));
            
            float value;
            FMODERRCHK(param->getValue(&value));
            FMODERRCHK(temp_param->setValue(value));
        }

        FMODERRCHK(temp_instance->start());

        // releasing because FMOD won't delete it internally
        // until it finishes playing
        FMODERRCHK(temp_instance->release());

        _playing = false;
    }

    void AudioEvent::playLoop()
    {
        if (_playing)
            return ;

        if (!_instance)
            FMODERRCHK((*_description)->createInstance(&_instance));

        float master_volume = gAudioManager->GetMasterVolume();

        FMODERRCHK(_instance->setVolume(_volume * master_volume));

        FMODERRCHK(_instance->setPitch(_pitch));

        FMODERRCHK(_instance->start());
    }

    void AudioEvent::playStartRestart()
    {
        if (!_instance)
            FMODERRCHK((*_description)->createInstance(&_instance));

        float master_volume = gAudioManager->GetMasterVolume();

        FMODERRCHK(_instance->setVolume(_volume * master_volume));

        FMODERRCHK(_instance->setPitch(_pitch));

        FMODERRCHK(_instance->start());
    }
}