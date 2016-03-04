#pragma once

#include "ResourceData.h"

#include "WwiseForward.h"

namespace ursine
{
    namespace resources
    {
        class AudioData : public ResourceData
        {
            RESOURCE_DATA;

        public:
            typedef std::vector<std::string> EventList;

            AudioData(
                const EventList &events,
                void *initBytes,
                size_t initByteSize, 
                void *bankBytes,
                size_t bankSize
            );

            ~AudioData(void);

            const EventList &GetEvents(void) const;

            void *GetInitBytes(void) const;
            size_t GetInitByteSize(void) const;
            void *GetBankBytes(void) const;
            size_t GetBankSize(void) const;

            AkBankID GetInitID(void) const;
            AkBankID GetBankID(void) const;
        private:
            EventList m_events;

            void *m_initBytes;
            size_t m_initByteSize;
            void *m_bankBytes;
            size_t m_bankSize;

            AkBankID m_initID;
            AkBankID m_bankID;

            void Write(pipeline::ResourceWriter &output) override;

            meta::Type GetReaderType(void) override;
        } Meta(Register);
    }
}