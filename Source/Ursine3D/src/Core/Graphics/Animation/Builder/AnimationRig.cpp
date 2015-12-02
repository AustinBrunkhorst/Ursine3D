#include "UrsinePrecompiled.h"
#include "AnimationRig.h"

namespace ursine
{
    AnimationRig::AnimationRig(void)
        : m_name( "" )
        , m_boneCount( 0 )
    {
    }

    AnimationRig::AnimationRig( const std::string& name )
        : m_name( name )
        , m_boneCount( 0 )
    {
        
    }

    void AnimationRig::InitializeRig(const unsigned boneCount)
    {
        m_hierarchyTable.resize( boneCount );
        m_boneData.resize( boneCount );
        m_offsetMatrices.resize( boneCount );
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
        unsigned newID = m_boneCount++;

        // check if this is the root node
        AnimationBone *parent;
        if (parentID == -1)
            parent = nullptr;
        else
            parent = &m_boneData[ parentID ];

        // set bone in vector, initialize this bone with proper data
        m_boneData[ newID ].InitializeBone(name, bindTrans, bindScale, bindRotation, newID, parentID, parent);

        // set bone in hierarchy table
        m_hierarchyTable[ newID ] = parentID;
        
        // calculate offset matrix
        m_offsetMatrices[ newID ] = SMat4( boneTrans ) * SMat4( boneScale.X( ), boneScale.Y( ), boneScale.Z( ) ) * SMat4( boneRotation );

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
        return static_cast<unsigned>(m_boneData.size());
    }
}