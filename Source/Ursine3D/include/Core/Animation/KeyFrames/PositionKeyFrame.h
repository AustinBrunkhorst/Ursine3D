/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PositionKeyFrame.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "KeyFrame.h"
#include "SVec3.h"

namespace ursine
{
    class PositionKeyFrame : public KeyFrame
    {
    public:
        PositionKeyFrame(void);
        PositionKeyFrame(float time, const SVec3 &value);

        void Interpolate(KeyFrame *other, float t, ecs::EntityHandle &entity) override;
        void SetValue(ecs::EntityHandle &entity) override;

        SVec3 value;
    };
}
