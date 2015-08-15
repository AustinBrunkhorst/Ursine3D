#include "UrsinePrecompiled.h"

namespace Ursine
{
    namespace ECS
    {
        AudioEmitters::AudioEmitters(void)
            : BaseComponent()
        {

        }

        AudioEmitters::AudioEmitters(const std::vector<std::string> &event_names)
            : BaseComponent()
        {
            for (auto &name : event_names)
            {
                _emitters.emplace(std::make_pair(name, name));
            }
        }

        AudioEmitter *AudioEmitters::GetEvent(const std::string &event_name)
        {
            auto event = _emitters.find(event_name);

            if (event == _emitters.end())
                return nullptr;

            return &event->second;
        }

        bool AudioEmitters::HasEvent(const std::string &event_name)
        {
            auto event = _emitters.find(event_name);

            return event != _emitters.end();
        }

        AudioEmitter *AudioEmitters::AddEvent(const std::string &event_name)
        {
            _emitters.emplace(std::make_pair(event_name, event_name));

            return &_emitters[event_name];
        }

        void AudioEmitters::RemoveEvent(const std::string &event_name)
        {
            _emitters.erase(event_name);
        }
    }
}
