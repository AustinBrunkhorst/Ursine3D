//----------------------------------------
//  File: SoundManager.cpp
//  Purpose: Contains the SoundManager interface implementation.
//  Author: Jordan Ellis
//  Date: 5/2/2014
// ----------------------------------------------

#include "UrsinePrecompiled.h"

#include "AudioManager.h"
#include "AudioEmitter.h"
#include "Utils.h"

namespace ursine
{
    static void parse_Banks(fmod_s::System *system, std::ifstream &file);
    static void load_Bank(fmod_s::System *system, std::string &bank_s);

    static int emitterSort(const void *emitter1, const void *emitter2);

    AudioManager *gAudioManager = nullptr;

    AudioManager::AudioManager(const char *guid_file) 
        : _master_volume(1.0f)
        , _last_master_volume(1.0f)
        , _fGUID(guid_file)
        , _system(nullptr)
    {
        gAudioManager = this;

        // Initialize the system
        FMODERRCHK(fmod_s::System::create(&_system));
        FMODERRCHK(_system->initialize(512, 0, 0, nullptr));
        FMODERRCHK(_system->getLowLevelSystem(&_low_system));

        auto path = MediaPath(_fGUID, utils::TYPE_GUIDs);

        // open file, and parse to populate the event_desc_map
        std::ifstream infile(path.c_str());

        UAssert(infile.is_open(), "Error opening GUID file.\n%s", path.c_str());

        parse_Banks(_system, infile);

        gWindowManager->Listener( this )
            .On( WM_FOCUS_CHANGED, &AudioManager::onFocusChanged );
    }

    AudioManager::~AudioManager(void)
    {
        gWindowManager->Listener(this)
            .Off( WM_FOCUS_CHANGED, &AudioManager::onFocusChanged );

        for (auto itr : _event_desc_map)
        {
            FMODERRCHK(itr.second->releaseAllInstances());
        }

        for (auto itr : _global_events)
        {
            delete itr.second;
        }

        gAudioManager = nullptr;
    }

    void AudioManager::registerEvent(AudioEmitter *emitter)
    {
        auto iter = _event_desc_map.find(emitter->_event_name);

        // if the event description is already loaded
        if (iter != _event_desc_map.end())
        {
            // giving the emitter the description pointer for further initilization
            emitter->initEvent(&iter->second);
        }
        else
        {
            // add this event to the loading queue
            _loading_queue.push_back(emitter);
            // sort the queue based on the emitters weight
            //qsort(&_loading_queue[0], _loading_queue.size(), sizeof(AudioEmitter*), emitterSort);
        }
    }

    void AudioManager::unregisterEvent(AudioEmitter *emitter)
    {
        for (uint i = 0, n = _loading_queue.size(); i < n; ++i)
        {
            if (_loading_queue[i] == emitter)
            {
                _loading_queue.erase(_loading_queue.begin() + i);

                --i;
                --n;
            }
        }
    }

    void AudioManager::onFocusChanged(EVENT_HANDLER(WindowManager))
    {
        EVENT_ATTRS(WindowManager, WindowFocusArgs);

        if (args->focused)
        {
            SetMasterVolume( _last_master_volume );
        }
        else
        {
            _last_master_volume = _master_volume;

            SetMasterVolume( 0.0f );
        }
    }

    void AudioManager::Update(void)
    {
        // if there are emitters in the loading queue
        if (_loading_queue.size())
        {
            // load events in the queue
            for (AudioEmitter *emitter : _loading_queue)
            {
                fmod_s::EventDescription *description;

                // check for the event description already being loaded
                if (_event_desc_map.find(emitter->_event_name) == _event_desc_map.end())
                {
                    fmod_s::ID id;

                    // load event description from FMOD
                    FMODERRCHK(_system->lookupID(emitter->_event_name.c_str(), &id));
                    FMODERRCHK(_system->getEvent(&id, FMOD_STUDIO_LOAD_BEGIN_NOW, &description));

                    // store description in event map
                    _event_desc_map[emitter->_event_name] = description;
                }

                // send address of description to emiter for further initialization
                emitter->initEvent(&_event_desc_map[emitter->_event_name]);
            }

            // flush the queue
            _loading_queue.clear();
        }

        // Update the underlying system
        FMODERRCHK(_system->update());
        FMODERRCHK(_low_system->update());
    }

    void AudioManager::StopAll(bool fade_out)
    {
        fmod_s::EventInstance *buffer[125];

        for (auto itr : _event_desc_map)
        {
            int count;

            FMODERRCHK(itr.second->getInstanceList(buffer, 125, &count));

            for (int i = 0; i < count; ++i)
                FMODERRCHK(buffer[i]->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT));
        }
    }

    float AudioManager::GetMasterVolume(void) const
    {
        return _master_volume;
    }

    void AudioManager::SetMasterVolume(float volume)
    {
        fmod_s::EventInstance *buffer[125];

        for (auto itr : _event_desc_map)
        {
            int count;

            FMODERRCHK(itr.second->getInstanceList(buffer, 125, &count));

            for (int i = 0; i < count; ++i)
                FMODERRCHK(buffer[i]->setVolume(volume));
        }

        _master_volume = volume;

        FMODERRCHK(_system->update());
    }

    void AudioManager::PlayGlobalEvent(const std::string &event_name, int loading_priority, PlayMode mode)
    {
        auto event = _global_events.find(event_name);

        if (event == _global_events.end())
        {
            _global_events[event_name] = 
                new AudioEmitter(event_name, loading_priority, true, mode);
        }
        else
        {
            event->second->Play(mode);
        }
    }

    void AudioManager::StopGlobalEvent(const std::string &event_name, bool allow_fade_out)
    {
        auto event = _global_events.find(event_name);

        if (event != _global_events.end())
        {
            event->second->Stop(allow_fade_out);
        }
    }

    bool AudioManager::IsGlobalEventPlaying(const std::string &event_name)
    {
        auto event = _global_events.find(event_name);

        if (event != _global_events.end())
        {
            return event->second->IsPlaying();
        }

        return false;
    }

    std::ostream &operator<<(std::ostream &stream, const AudioManager &sound_manager)
    {
        // get some bit of memory usage information from FMOD
        // if Max Allocated is to too large, increase the memory pool size
        int current_allocated, max_allocated;

        FMOD::Memory_GetStats(&current_allocated, &max_allocated, false);

        stream << "FMOD Current Allocated: " << current_allocated << std::endl;
        stream << "FMOD Max Allocated: " << max_allocated << std::endl;

        return stream;
    }

    static void parse_Banks(fmod_s::System *system, std::ifstream &file)
    {
        // string for extraction
        std::string str;

        // must start at the beginning of the file
        file.clear();
        file.seekg(0, file.beg);

        // using getline to retain whitespaces
        while (getline(file, str))
        {
            // length of the prefix "bank:/"
            size_t prefix = 6;
            size_t position = str.find("bank:/");

            if (position != std::string::npos)
            {
                // finding the substring
                size_t startPos = position + prefix;
                size_t endPos = str.length() - startPos;
                // loading the bank (substring)
                load_Bank(system, str.substr(startPos, endPos).append(".bank"));
            }
        }

        // must load the unlisted master string bank
        load_Bank(system, str = "Master Bank.strings.bank");
    }

    static void load_Bank(fmod_s::System *system, std::string &bank_s)
    {
        fmod_s::Bank *bank = NULL;
        FMODERRCHK(system->loadBankFile(MediaPath(bank_s, utils::TYPE_AUDIO).c_str(),
            FMOD_STUDIO_LOAD_BANK_NORMAL, &bank));
    }

    int emitterSort(const void *emitter1, const void *emitter2)
    {
        const AudioEmitter *e1 = static_cast<const AudioEmitter*>(emitter1);
        const AudioEmitter *e2 = static_cast<const AudioEmitter*>(emitter2);

        if (e1->GetWeight() < e2->GetWeight())
            return -1;
        else if (e1->GetWeight() == e2->GetWeight())
            return 0;
        else
            return 1;
    }
}