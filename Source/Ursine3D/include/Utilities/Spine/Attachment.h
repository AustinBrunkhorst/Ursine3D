#pragma once

#include "AttachmentConfig.h"

namespace ursine
{
    namespace spine
    {
        class Attachment
        {
        public:
            const std::string name;
            const AttachmentType type;

            Attachment(const char *name, AttachmentType type);
            virtual ~Attachment(void);
        };
    }
}
