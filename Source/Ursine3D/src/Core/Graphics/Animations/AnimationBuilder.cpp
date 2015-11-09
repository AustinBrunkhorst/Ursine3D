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

    Animation* AnimationBuilder::AddAnimation()
    {
        
    }

    AnimationRig* AnimationBuilder::AddAnimationRig()
    {
        
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