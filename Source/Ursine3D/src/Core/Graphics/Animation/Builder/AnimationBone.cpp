/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationBone.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "AnimationBone.h"

namespace ursine
{
    AnimationBone::AnimationBone(void)
        : m_name("undefined")
        /*, m_translation(SVec3())
        , m_scale(SVec3())
        , m_rotation(SQuat())*/
        , m_boneID(-1)
        , m_parentID(-1)
        , m_parent(nullptr) { }

    void AnimationBone::InitializeBone(
        const std::string &name,
        const SVec3 &trans,
        const SVec3 &scale,
        const SQuat &rotation,
        const unsigned boneID,
        const unsigned parentID,
        AnimationBone *parent
        )
    {
        m_name = name;
        m_boneID = boneID;
        m_parentID = parentID;
        m_parent = parent;

        if (m_parent != nullptr)
            m_parent->AddChild(this);
    }

    bool AnimationBone::IsRoot(void) const
    {
        return m_parent == nullptr;
    }

    void AnimationBone::AddChild(AnimationBone *child)
    {
        m_children.push_back(child);
    }

    const std::string &AnimationBone::GetName(void) const
    {
        return m_name;
    }

    void AnimationBone::SetName(const std::string &name)
    {
        m_name = name;
    }

    const SVec3 &AnimationBone::GetTranslation(void) const
    {
        return m_transform->GetLocalPosition( );
    }

    void AnimationBone::SetTranslation(const SVec3 &trans)
    {
        m_transform->SetLocalPosition( trans );
    }

    const SVec3 &AnimationBone::GetScale(void) const
    {
        return m_transform->GetLocalScale( );
    }

    void AnimationBone::SetScale(const SVec3 &scale)
    {
        m_transform->SetLocalScale( scale );
    }

    const SQuat &AnimationBone::GetRotation(void) const
    {
        return m_transform->GetLocalRotation( );
    }

    void AnimationBone::SetRotation(const SQuat &rot)
    {
        m_transform->SetLocalRotation( rot );
    }

    unsigned AnimationBone::GetBoneID(void) const
    {
        return m_boneID;
    }

    unsigned AnimationBone::GetParentID() const
    {
        return m_parentID;
    }

    AnimationBone *AnimationBone::GetParent(void) const
    {
        return m_parent;
    }

    AnimationBone *AnimationBone::GetChild(unsigned childID) const
    {
        return m_children[childID];
    }

    unsigned AnimationBone::GetChildCount(void) const
    {
        return static_cast<unsigned>(m_children.size());
    }

    const std::vector<AnimationBone *> &AnimationBone::GetChildren(void) const
    {
        return m_children;
    }
}
