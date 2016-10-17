/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** URigData.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceData.h"
#include "Bone.h"

namespace ursine
{
    namespace resources
    {
        class URigData : public ResourceData
        {
            RESOURCE_DATA(URigData);

        public:
            std::string name;

            // depth first hierarchy
            std::vector<Bone> bones;

            // Named map of all bones
            std::unordered_map<std::string, uint> boneMap;

            void Write(pipeline::ResourceWriter &output) override;
            meta::Type GetReaderType(void) override;

        } Meta(Register);
    }
}
