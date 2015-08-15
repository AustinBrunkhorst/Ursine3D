/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AudioEmittersComponent.h
** - Component containing a collection of audio emitters
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////
#ifndef AUDIO_EMITTERS_COMPONENT_H
#define AUDIO_EMITTERS_COMPONENT_H
////////////////////////////////////////////////////////////////////////////////

#include "AudioEmitter.h"

namespace Ursine
{
    namespace ECS
    {
        class AudioEmitters : public Component
        {
            //SerializerAllow(AudioEmitters, _emitters);
            std::unordered_map<std::string, AudioEmitter> _emitters;

        public:
            AudioEmitters(void);

            AudioEmitters(const std::vector<std::string> &event_names);

            AudioEmitter *GetEvent(const std::string &event_name);

            bool HasEvent(const std::string &event_name);

            AudioEmitter *AddEvent(const std::string &event_name);
            
            void RemoveEvent(const std::string &event_name);
        };
    }
}

////////////////////////////////////////////////////////////////////////////////
#endif // !AUDIO_EMITTERS_COMPONENT_H
////////////////////////////////////////////////////////////////////////////////

#ifdef REGISTER_COMPONENTS
#ifndef AUDIO_EMITTERS_REGISTERED
#define AUDIO_EMITTERS_REGISTERED
namespace Ursine
{
    namespace ECS
    {
        RegisterComponent(AudioEmitters);
        //RegisterComponentData(AudioEmitters, _emitters);
    }
}
#endif
#endif
