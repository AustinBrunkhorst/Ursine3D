#pragma once

#include "ResourceData.h"

namespace ursine
{
    namespace resources
    {
        class UIResourceData : public ResourceData
        {
            RESOURCE_DATA;

        public:
            typedef std::unordered_map<
                fs::path, 
                BinaryData::Handle, 
                fs::PathHasher
            > ResourceTable;

            UIResourceData(ResourceTable &&table);

            BinaryData::Handle GetEntry(const fs::path &entryPath) const;
        private:
            ResourceTable m_table;

            void Write(pipeline::ResourceWriter &output) override;

            meta::Type GetReaderType(void) override;
        } Meta(Register);
    }
}