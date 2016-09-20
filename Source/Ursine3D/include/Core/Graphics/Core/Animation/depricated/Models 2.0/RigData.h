#pragma once

namespace ursine
{
    struct Bone
    {
        std::string name;
        ecs::Transform transform;
    };

    struct Rig
    {
        std::string name;

        std::vector<SMat4>      offsetMatrices;
        std::vector<Bone>       boneData;
        std::vector<unsigned>   bonehierarchy;
    };
}