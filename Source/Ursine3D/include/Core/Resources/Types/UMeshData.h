/* ----------------------------------------------------------------------------
** Team Bear King
** � 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UMeshData.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
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
        // TODO: [J] This shouldn't be here?
        // I don't think we need a resource reader/writer
        class UMeshData : public ResourceData
        {
            RESOURCE_DATA;

        public:

        private:
            void Write(pipeline::ResourceWriter &output) override;

            meta::Type GetReaderType(void) override;

            // raw mesh data for physics stuff
            std::vector<Vec3> m_rawVertData;
            std::vector<UINT> m_rawIndexData;
            std::vector<Vec3> m_rawNormData;
            std::vector<Vec3> m_rawNBinormData;
            std::vector<Vec3> m_rawTangentmData;

            // raw binary data
            std::vector<AnimationVertex> m_rawModelData;

        } Meta(Register);
    }
}
