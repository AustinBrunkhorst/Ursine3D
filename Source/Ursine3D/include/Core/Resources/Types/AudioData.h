/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** AudioData.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceData.h"

#include "WwiseForward.h"

namespace ursine
{
    namespace resources
    {
        class AudioData : public ResourceData
        {
            RESOURCE_DATA(AudioData);

        public:
            typedef std::vector<std::string> EventList;

            AudioData(
                const EventList &events,
                BinaryData initData,
                BinaryData bankData
            );

            AudioData(const AudioData &rhs);

            ~AudioData(void);

            const EventList &GetEvents(void) const;

            const BinaryData &GetInitData(void) const;
            const BinaryData &GetBankData(void) const;

            AkBankID GetInitID(void) const;
            AkBankID GetBankID(void) const;
        private:
            EventList m_events;

            BinaryData m_initData;
            BinaryData m_bankData;

            AkBankID m_initID;
            AkBankID m_bankID;

            void Write(pipeline::ResourceWriter &output) override;

            meta::Type GetReaderType(void) override;
        } Meta(Register);
    }
}
