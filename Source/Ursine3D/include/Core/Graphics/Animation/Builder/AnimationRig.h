/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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
        AnimationRig(void);
        AnimationRig(const std::string &name);

        /** @brief initializes this rig for loading
        *
        *  resizes some veectors so pointers aren't invalidated
        *
        *  @param boneCount number of bones in rig
        *  @return Void.
        */
        void InitializeRig(const unsigned boneCount);

        /** @brief adds a bone to this rig
        *
        *  the rig consists of a hierarchy of bones. 
        *  this method will add the bone, update the data,
        *  calculate the proper matrices and update the 
        *  parent's child list
        *
        *  @param name the name of this bone
        *  @param boneTrans the trans in bone space
        *  @param boneScale the scale in bone space
        *  @param boneRotation the rot in bone space
        *  @param bindTrans the trans in bind space
        *  @param bindScale the scale in bind space
        *  @param bindRotation the rot in bind space
        *  @param parentID the index ID of parent bone
        *  @return ID of new bone
        */
        unsigned AddBone(
            const std::string& name,
            const SVec3& boneTrans,
            const SVec3& boneScale,
            const SQuat& boneRotation,
            const SVec3& bindTrans,
            const SVec3& bindScale,
            const SQuat& bindRotation,
            unsigned parentID
        );

        // gettors and setters //////////////////////////////////////

        const std::string &GetName(void) const;
        void SetName(const std::string &name);

        const std::vector<SMat4> &GetOffsetMatrices(void) const;

        const std::vector<AnimationBone> &GetBoneData(void) const;
        AnimationBone *GetBone(unsigned boneID);

        unsigned GetParentID(unsigned childID) const;

        unsigned GetBoneCount(void) const;

        const std::vector<unsigned> &GetHierarchyTable(void) const;
    private:
        std::string m_name;

        unsigned m_boneCount;

        std::vector<SMat4> m_offsetMatrices;    //t-pose
        std::vector<AnimationBone> m_boneData;  //bone-space data
        std::vector<unsigned> m_hierarchyTable;
    };
}