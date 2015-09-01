/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AudioEmitter.cpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "Application.h"
#include "AudioEmitter.h"
#include "AudioDataBaseClasses.h"


const ursine::SVec3 MAX_BOUNDS = ursine::SVec3(2400, 2400, 400);
const ursine::SVec3 FMOD_MAX_BOUNDS = ursine::SVec3(20, 20, 20);
namespace ursine
{
    static void setAttributesDefault(FMOD_3D_ATTRIBUTES &attributes)
    {
        attributes.position = { 0, 0, 0 };
        attributes.velocity = { 0, 0, 0 };
        attributes.forward = { 0, 1, 0 };
        attributes.up = { 0, 0, 1 };
    }

    AudioEmitter::AudioEmitter()
        : _play_on_creation(false)
        , _mute(false)
        , _weight(0)
        , _derp(nullptr)
    {
        setAttributesDefault(_attributes);
    }

    AudioEmitter::AudioEmitter(const std::string &event_name, int loading_priority,
                               bool play_on_creation, PlayMode mode)
        : _event_name(event_name)
        , _weight(loading_priority)
        , _play_on_creation(play_on_creation)
        , _mute(false)
        , _derp(nullptr)
        , _mode(mode)
    {
        setAttributesDefault(_attributes);

        // register with the audio manager so our event can be loaded
        gAudioManager->registerEvent(this);
    }

    AudioEmitter::~AudioEmitter() 
    {
        if (gAudioManager)
            gAudioManager->unregisterEvent(this);
    }

    AudioEmitter::AudioEmitter(const AudioEmitter &other)
    {
        copy(other);
        gAudioManager->registerEvent(this);
    }

    AudioEmitter &AudioEmitter::operator=(const AudioEmitter &rhs)
    {
        copy(rhs);
        gAudioManager->registerEvent(this);
        return (*this);
    }

    void AudioEmitter::Play()
    {
        // if the event has been loaded
        if (_event._description)
            _event.play();
    }

    void AudioEmitter::Play(PlayMode mode)
    {
        // if the event has been loaded
        if (_event._description)
            _event.play(mode);
    }

    void AudioEmitter::Stop(bool allow_fade_out)
    {
        _event.stop(allow_fade_out);
    }

    void AudioEmitter::Update(float dt)
    {
        _event.update(dt);
    }

    bool AudioEmitter::IsPlaying(void)
    {
        FMOD_STUDIO_PLAYBACK_STATE state;

        if (_event._instance)
            FMODERRCHK(_event._instance->getPlaybackState(&state));
        else
            return false;

        if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
            _event._playing = false;

        return _event._playing;
    }

    void AudioEmitter::SetEvent(char const* event_name)
    {
        _event_name = event_name;
        gAudioManager->registerEvent(this);
    }

    void AudioEmitter::SetPitch(float pitch)
    {
        _event._pitch = pitch;
    }

    void AudioEmitter::SetVolume(float volume)
    {
        _event._volume = volume;
        if (_event._playing && _event._instance)
        {
            if (gAudioManager->GetMasterVolume() > volume)
                FMODERRCHK(_event._instance->setVolume(volume));
        }
    }

    void AudioEmitter::SetMute(bool mute)
    {
        _mute = mute;
        if (_mute && _event._volume != 0.0f)
            SetVolume(0.0f);
    }

    void AudioEmitter::SetMode(PlayMode mode)
    {
        _event._mode = _mode = mode;
    }

    void AudioEmitter::SetTimeline(int timeline)
    {
        if (_event._instance)
            FMODERRCHK(_event._instance->setTimelinePosition(timeline));
    }

    void AudioEmitter::SetDerpFile(char const* file_name)
    {
        if (_derp)
            delete _derp;

        _derp = new DERPFile(file_name);
    }

    void AudioEmitter::SetParameter(AudioParam &param, float value)
    {
        if (_event._instance)
        {
            float range = param.maximum - param.minimum;
            auto new_value = param.minimum + (value * range);

            FMODERRCHK(_event._instance->setParameterValue(param.name, new_value));
        }
    }

    void AudioEmitter::Set3DParameter(SVec3 const& listener, SVec3 const& emitter)
    {
        SVec3 dist = emitter - listener;
        dist = (dist / MAX_BOUNDS) * FMOD_MAX_BOUNDS;

        _attributes.position = { dist.X(), dist.Y(), dist.Z() };

        if (_event._instance)
            FMODERRCHK(_event._instance->set3DAttributes(&_attributes));
    }

    void AudioEmitter::initEvent(fmod_s::EventDescription **description)
    {
        // Set pointer to description
        _event._description = description;

        if (_event._instance)
            FMODERRCHK(_event._instance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT));

        // Create instance from description
        FMODERRCHK((*_event._description)->createInstance(&_event._instance));

        // Set the event instances values
        FMODERRCHK(_event._instance->setVolume(_event._volume));
        FMODERRCHK(_event._instance->setPitch(_event._pitch));

        // Pass it a pointer to it's 3D data struct
        bool is_3d = false;
        FMODERRCHK((*_event._description)->is3D(&is_3d));
        if (is_3d)
            FMODERRCHK(_event._instance->set3DAttributes(&_attributes));

        _event._mode = _mode;

        // play the event
        if (_play_on_creation)
            _event.play();
    }

    void AudioEmitter::copy(const AudioEmitter &other)
    {
        _event_name = other._event_name;
        _play_on_creation = other._play_on_creation;
        _mute = other._mute;
        _weight = other._weight;
        _derp = nullptr;

        // this does not copy over the instance
        // see AudioEvent's operator= for more info
        _event = other._event;
    }
}