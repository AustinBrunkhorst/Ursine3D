/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** Content3DImporter.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceImporter.h"
#include "ResourceTypeReader.h"
#include "PassThroughProcessor.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            // TODO: [J] Remove these.  It's needed because the Import function must not return nullptr
            ///////////////////////////////////////////////////////////////////////////////////////////
            class Content3DReader : public ResourceTypeReader
            {
                RESOURCE_TYPE_READER;

                ResourceData::Handle Read(ResourceReader &input) override;
            } Meta(
                Enable,
                DisplayName( "Content3DReader" )
            );

            class Content3DData : public ResourceData
            {
                RESOURCE_DATA;

                void Write(pipeline::ResourceWriter &output) override;
                meta::Type GetReaderType(void) override;

            } Meta(Register);

            ///////////////////////////////////////////////////////////////////////////////////////////

            class Content3DImporter : public ResourceImporter
            {
                RESOURCE_IMPORTER;

            public:
                Content3DImporter(void);
                ~Content3DImporter(void);

            private:
                ResourceData::Handle Import(ResourceImportContext &context) override;

            } Meta(
                Enable,
                DisplayName( "Content3DImporter" ),
                ResourceImporterConfig(
                    ExtensionList{ "fbx" },
                    typeof( ursine::rp::PassThroughProcessor )
                )
            );
        }
    }
}
