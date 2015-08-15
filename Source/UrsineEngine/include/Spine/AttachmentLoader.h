#pragma once

#include "Spine/Attachment.h"
#include "Spine/Skin.h"

namespace Ursine
{
    namespace Spine
    {
        class AttachmentLoader
        {
        public:
            virtual ~AttachmentLoader(void) { }

            virtual Attachment *NewAttachment(Skin *skin, AttachmentType type, const char *name, const char *path) = 0;
        };
    }
}
