#pragma once

#include "SVec3.h"
#include "SQuat.h"

namespace ursine
{
    // yup
    struct Keyframe
    {
        float length;
        SVec3 translation;
        SVec3 scale;
        SQuat rotation;
    };

    // struct for one total frame of animation, top to bottom
    struct KeyframeSet
    {
        std::vector<Keyframe> keyframes;
    };

    // struct for an animation as a whole
    struct Animation
    {
        std::string name;
        std::vector<KeyframeSet> keyframeSets;
    };
}