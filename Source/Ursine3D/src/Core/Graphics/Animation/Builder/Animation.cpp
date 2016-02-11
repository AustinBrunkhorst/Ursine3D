/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Animation.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "Animation.h"

namespace ursine
{
	Animation::Animation(void)
		: m_rigKeyframeCount(0)
		, m_boneCount(0)
		, m_name("")
	{
	}

	Animation::Animation(const std::string &name)
		: m_rigKeyframeCount(0)
		, m_boneCount(0)
		, m_name(name)
	{
	}

	void Animation::AddKeyframe(
		const unsigned rigKeyIndex,
		const unsigned boneIndex,
		const SVec3& trans,
		const SVec3& scale,
		const SQuat& rot,
		const float runtime
		)
	{
		unsigned index = static_cast<unsigned>(m_keyframes.size());

		m_keyframes[rigKeyIndex][boneIndex].translation = trans;
		m_keyframes[rigKeyIndex][boneIndex].scale = scale;
		m_keyframes[rigKeyIndex][boneIndex].rotation = rot;
		m_keyframes[rigKeyIndex][boneIndex].length = runtime;
	}

	void Animation::SetData(
		const unsigned rigKeyFrameCount,
		const unsigned boneCount)
	{
		m_boneCount = boneCount;
		m_rigKeyframeCount = rigKeyFrameCount;
		m_keyframes.resize(rigKeyFrameCount);
		for (auto &x : m_keyframes)
			x.resize(boneCount);
	}

	const std::string& Animation::GetName(void) const
	{
		return m_name;
	}

	void Animation::SetName(const std::string& name)
	{
		m_name = name;
	}

	const std::vector<AnimationKeyframe>& Animation::GetKeyframes(const unsigned rigKeyframe) const
	{
		return m_keyframes[rigKeyframe];
	}

	const AnimationKeyframe& Animation::GetKeyframe(const unsigned rigKeyFrame, const unsigned boneIndex) const
	{
		return m_keyframes[rigKeyFrame][boneIndex];
	}

	unsigned Animation::GetRigKeyFrameCount() const
	{
		return m_rigKeyframeCount;
	}

	unsigned Animation::GetDesiredBoneCount() const
	{
		return m_boneCount;
	}
}