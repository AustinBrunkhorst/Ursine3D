#include "UrsinePrecompiled.h"
#include "AnimationRig.h"

namespace ursine
{
    AnimationRig::AnimationRig(void)
    {
    }

    AnimationRig::AnimationRig(const std::string& name)
        : m_name(name)
    {
        
    }

    unsigned AnimationRig::AddBone(
        const std::string& name,
        const SVec3& boneTrans,
        const SVec3& boneScale,
        const SQuat& boneRotation,
        const SVec3& bindTrans,
        const SVec3& bindScale,
        const SQuat& bindRotation,
        const unsigned parentID
    )
    {
        // we need a new bone! first, save the new boneID, add to hierarchy table
        unsigned newID = m_boneData.size();
        m_hierarchyTable.push_back(parentID);

        // check if this is the root node
        AnimationBone *parent;
        if (parentID == -1)
            parent = nullptr;
        else
            parent = &m_boneData[ parentID ];

        // add bone to table, initialize this bone with proper data
        m_boneData.push_back(AnimationBone());
        m_boneData[ newID ].InitializeBone(name, boneTrans, boneScale, boneRotation, newID, parent);
        
        // calculate offset matrix
        SMat4 offsetMatrix = SMat4(bindTrans) * SMat4(bindScale.X(), bindScale.Y(), bindScale.Z()) * SMat4(bindRotation);
        m_offsetMatrices.push_back(offsetMatrix);

        return newID;
    }

    const std::string& AnimationRig::GetName() const
    {
        return m_name;
    }

    void AnimationRig::SetName(const std::string& name)
    {
        m_name = name;
    }

    const std::vector<SMat4>& AnimationRig::GetOffsetMatrices() const
    {
        return m_offsetMatrices;
    }

    const std::vector<AnimationBone>& AnimationRig::GetBoneData() const
    {
        return m_boneData;
    }

    AnimationBone& AnimationRig::GetBone(const unsigned boneID)
    {
        return m_boneData[ boneID ];
    }

    unsigned AnimationRig::GetParentID(const unsigned childID) const
    {
        return m_hierarchyTable[ childID ];
    }

    unsigned AnimationRig::GetBoneCount() const
    {
        return m_boneData.size();
    }
}