#pragma once

#include "ResourceData.h"

namespace ursine
{
    namespace resources
    {
        class UIScreenData : public ResourceData
        {
            RESOURCE_DATA;

        public:
            UIScreenData(const Json &data);
            UIScreenData(const GUID &project, const fs::path &path);

            const GUID &GetProject(void) const;
            const fs::path &GetPath(void) const;
            const std::string &GetName(void) const;

            fs::path GetQualifiedPath(void) const;

            void Write(pipeline::ResourceWriter &output) override;

        private:
            GUID m_project;
            fs::path m_path;
            std::string m_name;

            meta::Type GetReaderType(void) override;
        } Meta( Register );
    }
}