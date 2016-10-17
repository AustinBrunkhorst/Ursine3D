/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** KeyFrame.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    class KeyFrame
    {
    public:
        KeyFrame(float time);

        // Interpolate FROM "this" TO "other" with t [0.0f, 1.0f]
        virtual void Interpolate(KeyFrame *other, float t, ecs::EntityHandle &entity) = 0;

        virtual void SetValue(ecs::EntityHandle &entity) = 0;

        // Time in seconds
        float time;
    };
}
