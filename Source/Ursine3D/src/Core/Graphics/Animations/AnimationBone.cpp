#include "UrsinePrecompiled.h"
#include "AnimationBone.h"

namespace ursine
{
    AnimationBone::AnimationBone(void)
        : m_name( "undefined" )
        , m_translation( SVec3( ) )
        , m_scale( SVec3( ) )
        , m_rotation( SQuat( ) )
        , m_boneID( -1 )
        , m_parent( nullptr )
    {
    }

    void AnimationBone::InitializeBone(
        const std::string& name,
        const SVec3& trans,
        const SVec3& scale,
        const SQuat& rotation,
        const unsigned boneID,
        AnimationBone* parent
    )
    {
        m_name = name;
        m_translation = trans;
        m_scale = scale;
        m_rotation = rotation;
        m_boneID = boneID;
        m_parent = parent;

        if ( m_parent != nullptr )
            m_parent->AddChild( this );
    }

    bool AnimationBone::IsRoot(void) const
    {
        return m_parent == nullptr;
    }

    void AnimationBone::AddChild(const AnimationBone* child)
    {
        m_children.push_back( child );
    }

    const std::string& AnimationBone::GetName() const
    {
        return m_name;
    }

    void AnimationBone::SetName(const std::string& name)
    {
        m_name = name;
    }

    const SVec3& AnimationBone::GetTranslation() const
    {
        return m_translation;
    }

    void AnimationBone::SetTranslation(const SVec3& trans)
    {
        m_translation = trans;
    }

    const SVec3& AnimationBone::GetScale(void) const
    {
        return m_scale;
    }

    void AnimationBone::SetScale(const SVec3& scale)
    {
        m_scale = scale;
    }

    const SQuat& AnimationBone::GetRotation(void) const
    {
        return m_rotation;
    }

    void AnimationBone::SetRotation(const SQuat& rot)
    {
        m_rotation = rot;
    }

    unsigned AnimationBone::GetBoneID(void) const
    {
        return m_boneID;
    }

    void AnimationBone::SetBoneID(const unsigned boneID)
    {
        m_boneID = boneID;
    }

    const AnimationBone* AnimationBone::GetParent(void) const
    {
        return m_parent;
    }

    const AnimationBone* AnimationBone::GetChild(const unsigned childID) const
    {
        return m_children[ childID ];
    }

    unsigned AnimationBone::GetChildCount(void) const
    {
        return m_children.size( );
    }
}