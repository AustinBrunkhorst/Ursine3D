/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationRig.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once
#include <vector>
#include <string>

#include "SMat4.h"
#include "AnimationBone.h"

//this is information relating to the underlying bone structure of an animation

namespace ursine
{
    class AnimationRig
    {
    public:
        AnimationRig(const std::string &name);

        //returns boneID of newly minted bone lolol
        unsigned AddBone(
            const std::string& name,
            const SVec3& trans,
            const SVec3& scale,
            const SQuat& rotation,
            const unsigned parentID
        );

        // gettors and setters //////////////////////////////////////

        const std::string &GetName(void) const;
        void SetName(const std::string &name);

        const std::vector<SMat4> &GetOffsetMatrices(void) const;

        const std::vector<AnimationBone> &GetBoneData(void) const;
        AnimationBone &GetBone(const unsigned boneID);

        unsigned GetParentID(const unsigned childID) const;

        unsigned GetBoneCount(void) const;

    private:
        std::string m_name;

        std::vector<SMat4> m_offsetMatrices;
        std::vector<AnimationBone> m_boneData;
        std::vector<unsigned> m_hierarchyTable;
    };
}