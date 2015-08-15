#pragma once

#include "AttachmentConfig.h"

namespace Ursine
{
    namespace Spine
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
