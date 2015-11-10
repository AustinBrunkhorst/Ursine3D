#include "UrsinePrecompiled.h"
#include "Animation.h"

namespace ursine
{
    Animation::Animation(std::string& name) {}

    void Animation::AddKeyframe(const SVec3& trans, const SVec3& scale, const SQuat& rot, const float runtime)
    {
        unsigned index = m_keyframes.size();

        m_keyframes.push_back(AnimationKeyframe());
        m_keyframes[ index ].translation = trans;
        m_keyframes[ index ].scale = scale;
        m_keyframes[ index ].rotation = rot;

        m_keyframes[ index ].length = runtime;
    }

    const std::string& Animation::GetName() const
    {
        return m_name;
    }

    void Animation::SetName(const std::string& name)
    {
        m_name = name;
    }

    const std::vector<AnimationKeyframe>& Animation::GetKeyframes() const
    {
        return m_keyframes;
    }

    const AnimationKeyframe& Animation::GetKeyframe(unsigned index) const
    {
        return m_keyframes[ index ];
    }
}