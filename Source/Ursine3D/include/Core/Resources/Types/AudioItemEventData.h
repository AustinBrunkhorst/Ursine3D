/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** AudioItemEventData.h
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceData.h"

namespace ursine
{
    namespace resources
    {
        class AudioItemEventData : public ResourceData
        {
            RESOURCE_DATA;

        public:
            AudioItemEventData(const Json &data);
            AudioItemEventData(const GUID &project, const std::string &event);

            const GUID &GetProject(void) const;
            const std::string &GetEvent(void) const;

            void Write(pipeline::ResourceWriter &output) override;

        private:
            GUID m_project;
            std::string m_event;

            meta::Type GetReaderType(void) override;

        } Meta(Register);
    }
}