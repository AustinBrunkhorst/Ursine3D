#include "UrsinePrecompiled.h"
#include "AnimationBuilder.h"

namespace ursine
{
    std::vector<Animation> AnimationBuilder::m_animationData;
    std::vector<AnimationRig> AnimationBuilder::m_animationRigData;

    void AnimationBuilder::InitializeStaticData()
    {
        m_animationData.resize(100);
        m_animationRigData.resize(100);
    }

    unsigned AnimationBuilder::AddAnimation()
    {
        unsigned index = m_animationData.size();
        m_animationData.push_back(Animation());
        return index;
    }

    unsigned AnimationBuilder::AddAnimationRig()
    {
        unsigned index = m_animationRigData.size();
        m_animationRigData.push_back(AnimationRig());
        return index;
    }

    Animation *AnimationBuilder::GetAnimation(const unsigned index)
    {
        return &m_animationData[ index ];
    }

    AnimationRig *AnimationBuilder::GetAnimationRig(const unsigned index)
    {
        return &m_animationRigData[ index ];
    }
}