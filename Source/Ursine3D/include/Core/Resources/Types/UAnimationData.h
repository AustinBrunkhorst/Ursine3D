/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UAnimationData.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceData.h"
#include "AnimationConfig.h"
#include "AnimationLane.h"
#include "RigComponent.h"

namespace ursine
{
    namespace resources
    {
        class UAnimationData : public ResourceData
        {
            RESOURCE_DATA(UAnimationData);

        public:
            UAnimationData(void);

            void Evaluate(ecs::Rig *rig, float time);

            // each animation lane affects one bone
            std::vector<AnimationLane> lanes;

            std::string name;

            // Duration in seconds
            float duration;

            void Write(pipeline::ResourceWriter &output) override;
            meta::Type GetReaderType(void) override;

        } Meta(Register);
    }
}
