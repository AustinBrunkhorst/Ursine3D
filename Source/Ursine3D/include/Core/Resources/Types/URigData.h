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

namespace ursine
{
    namespace resources
    {
        class URigData : public ResourceData
        {
            RESOURCE_DATA;

        public:

            // bone { local vqs, offset matrix, parent index, children? }

            // uint root bone

            // vector<bone> (array of all bones in this rig [depth first? bredth?])
            

            // map<name, index> (store all bone names and their index into the map)

        private:
            void Write(pipeline::ResourceWriter &output) override;

            meta::Type GetReaderType(void) override;

        } Meta(Register);
    }
}
