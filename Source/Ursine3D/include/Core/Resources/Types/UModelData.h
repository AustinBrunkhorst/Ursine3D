/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UModelData.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceData.h"
#include "UMeshData.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            // TODO: [J] Remove this shit
            class Content3DImporter;
        }

        class UModelData : public ResourceData
        {
            RESOURCE_DATA;

            // TODO: [J] Remove this dependency (needed for Write function)
            friend class pipeline::Content3DImporter;
            
            friend class UModelReader;

        public:

            typedef std::shared_ptr<UModelData> Handle;

            void AddMesh(const UMeshData::Handle &mesh);
            const UMeshData::Handle &GetMesh(uint index) const;

            uint GetNumMeshes(void) const;

            void SetName(const std::string &name);
            const std::string &GetName(void) const;

            graphics::GfxHND GetGraphicsHandle(void) const;

        private:
            void Write(pipeline::ResourceWriter &output) override;

            meta::Type GetReaderType(void) override;

            std::vector<UMeshData::Handle> m_meshes;

            std::string m_name;

            graphics::GfxHND m_id;

        } Meta(Register);
    }
}
