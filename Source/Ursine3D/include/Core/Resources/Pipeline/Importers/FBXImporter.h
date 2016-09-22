/* ----------------------------------------------------------------------------
** Team Bear King
** � 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** FBXImporter.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceImporter.h"

#include "U3DContentData.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class FBXImporter : public ResourceImporter
            {
                RESOURCE_IMPORTER;

            public:
                FBXImporter(void);
                ~FBXImporter(void);

            private:
                ResourceData::Handle Import(ResourceImportContext &context) override;

                bool importFBX(const char *fileName, U3DContentData *content);

                void buildModel(FbxScene *scene, UModelData &modelData);

                void getVertsAndIndices(FbxMesh *mesh, UMeshData *meshData);

                void getNormalBinormalAndTangents(FbxMesh *mesh, UMeshData *meshData);

            } Meta(
                Enable,
                DisplayName( "FBXImporter" ),
                ResourceImporterConfig(
                    ExtensionList{ "fbx" },
                    typeof( ursine::rp::PassThroughProcessor )
                )
            );
        }
    }
}
