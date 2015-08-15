#include "UrsinePrecompiled.h"

#include "Spine/spine.h"

namespace Ursine
{
    namespace Spine
    {
        Skin::Entry::Entry(int slotIndex, const char *name, Attachment *attachment)
            : slot_index(slotIndex)
            , attachment(attachment)
            , name(name)
        {

        }

        Skin::Entry::~Entry(void)
        {
            delete attachment;
        }

        Skin::Skin(const char *name)
            : name(name)
        {

        }

        Skin::~Skin(void)
        {
            for (auto entry : entries)
                delete entry;
        }

        void Skin::AddAttachment(int slot_index, const char *name, Attachment *attachment)
        {
            entries.push_back(new Entry(slot_index, name, attachment));
        }

        Attachment *Skin::GetAttachment(int slot_index, const char *name) const
        {
            for (auto entry : entries)
            {
                if (entry->slot_index == slot_index && strcmp(entry->name.c_str(), name) == 0)
                    return entry->attachment;
            }

            return nullptr;
        }

        const char *Skin::GetAttachmentName(int slot_index, int attachment_index) const
        {
            int i = 0;

            for (auto entry : entries)
            {
                if (entry->slot_index == slot_index)
                {
                    if (i == attachment_index)
                        return entry->name.c_str();

                    i++;
                }
            }

            return nullptr;
        }

        void Skin::AttachAll(Skeleton *skeleton, const Skin *old_skin) const
        {
            for (auto entry : old_skin->entries)
            {
                Slot *slot = skeleton->GetSlots()[entry->slot_index];

                if (slot->attachment == entry->attachment)
                {
                    Attachment *attachment = GetAttachment(entry->slot_index, entry->name.c_str());

                    if (attachment)
                        slot->SetAttachment(attachment);
                }
            }
        }

        const char *Skin::GetName(void) const
        {
            return name.c_str();
        }
    }
}
