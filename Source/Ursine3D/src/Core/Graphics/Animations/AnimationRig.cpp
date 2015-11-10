#include "UrsinePrecompiled.h"
#include "AnimationRig.h"

namespace ursine
{
    AnimationRig::AnimationRig(const std::string& name)
        : m_name(name)
    {
        
    }

    unsigned AnimationRig::AddBone(
        const std::string& name, 
        const SVec3& trans, 
        const SVec3& scale, 
        const SQuat& rotation, 
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
        m_boneData[ newID ].InitializeBone(name, trans, scale, rotation, newID, parent);
        
        // calculate offset matrix
        SMat4 offsetMatrix = SMat4(trans) * SMat4(scale.X(), scale.Y(), scale.Z()) * SMat4(rotation);
        m_offsetMatrices.push_back(offsetMatrix);
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