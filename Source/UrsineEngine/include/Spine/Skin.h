#pragma once

#include "Spine/Attachment.h"

namespace ursine
{
    namespace spine
    {
        class Skeleton;

        class Skin
        {
            struct Entry
            {
                int slot_index;

                std::string name;

                Attachment *attachment;

                Entry(int slot_index, const char *name, Attachment *attachment);
                ~Entry(void);
            };

            const std::string name;

            std::vector<Entry *> entries;

        public:
            Skin(const char *name);
            ~Skin(void);

            void AddAttachment(int slot_index, const char *name, Attachment *attachment);

            Attachment *GetAttachment(int slot_index, const char *name) const;

            const char *GetAttachmentName(int slot_index, int attachment_index) const;

            void AttachAll(Skeleton *skeleton, const Skin *old_skin) const;

            const char *GetName(void) const;
        };
    }
}
