/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** U3DContentData.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceData.h"

#include "UModelData.h"
#include "URigData.h"
#include "UAnimationData.h"

namespace ursine
{
    namespace resources
    {
        class U3DContentData : public ResourceData
        {
            RESOURCE_DATA;

        public:

        private:
            void Write(pipeline::ResourceWriter &output) override;

            meta::Type GetReaderType(void) override;

            URigData       m_rigData;
            UModelData     m_modelData;
            UAnimationData m_animationData;

        } Meta(Register);
    }
}
