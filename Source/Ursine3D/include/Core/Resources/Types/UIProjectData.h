/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UIProjectData.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
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
        class UIProjectData : public ResourceData
        {
            RESOURCE_DATA(UIProjectData);

        public:
            typedef std::unordered_map<
                fs::path, 
                BinaryData::Handle, 
                fs::PathHasher
            > ResourceTable;

            UIProjectData(ResourceTable &&table);

            BinaryData::Handle GetEntry(const fs::path &entryPath) const;
        private:
            ResourceTable m_table;

            void Write(pipeline::ResourceWriter &output) override;

            meta::Type GetReaderType(void) override;
        } Meta(Register);
    }
}