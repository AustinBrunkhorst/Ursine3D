#pragma once

#include "Spine/AttachmentLoader.h"
#include "Spine/Atlas.h"

namespace ursine
{
    namespace spine
    {
        class AtlasAttachmentLoader : public AttachmentLoader
        {
            Atlas *atlas;

        public:
            AtlasAttachmentLoader(Atlas *atlas);
            ~AtlasAttachmentLoader(void);

            Attachment *NewAttachment(Skin *skin, AttachmentType type, const char *name, const char *path) override;
        };
    }
}
