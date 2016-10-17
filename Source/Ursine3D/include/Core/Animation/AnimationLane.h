/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationLane.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "KeyFrame.h"

#include "PositionKeyFrame.h"
#include "RotationKeyFrame.h"
#include "ScaleKeyFrame.h"
#include "ResourceReader.h"

namespace ursine
{
    class AnimationLane
    {
    public:
        AnimationLane(void);

        void Evaluate(ecs::EntityHandle bone, float time);

        void Write(resources::pipeline::ResourceWriter &output);
        void Read(resources::ResourceReader &reader);

        enum class Type
        {
            // The value from the default bone transformation is taken
            Default,
            // The nearest key value is used without interpolation
            Constant,
            // The value of the nearest two keys is linearly extrapolated for the current time value
            Linear,
            // The animation is repeated
            // If the animation key goes from N to M, and current time is T,
            // use the value at (T - N) % |M - N|
            Repeat
        };

        // Defines how the animation behaves before the first key is encountered
        Type preState;

        // Defines how the animation behaves after the last key was processed.
        Type postState;

        uint boneIndex;

        std::vector<std::vector<KeyFrame>*> keyFrames;

        std::vector<PositionKeyFrame> positionKeys;
        std::vector<RotationKeyFrame> rotationKeys;
        std::vector<ScaleKeyFrame>    scaleKeys;
    };
}
