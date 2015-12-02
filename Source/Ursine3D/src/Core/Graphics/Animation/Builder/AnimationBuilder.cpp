#include "UrsinePrecompiled.h"
#include "AnimationBuilder.h"
#include "ModelInfo.h"

namespace ursine
{
    std::vector<Animation>                          AnimationBuilder::m_animationData;
    std::vector<AnimationRig>                       AnimationBuilder::m_animationRigData;
    std::unordered_map<std::string, Animation*>     AnimationBuilder::m_name2Animation;
    std::unordered_map<std::string, AnimationRig*>  AnimationBuilder::m_name2Rig;
    std::vector<SMat4>                              AnimationBuilder::m_toParentTransforms;
    std::vector<SMat4>                              AnimationBuilder::m_toRootTransforms;

    unsigned AnimationBuilder::m_rigCount;
    unsigned AnimationBuilder::m_animationCount;

	void AnimationBuilder::GenerateAnimationData(const AnimationState& animState, const AnimationRig* rig, std::vector<SMat4>& outputMatPal)
	{
		// get the current time
		float time = animState.GetTimePosition();

		// clamp time between 0 and 1
		time = time < 0 ? 0 : time;
		time = time > 1 ? 1 : time;

		// get the currently running animation
		auto currentAnimation = animState.GetAnimation();

		// get the total keyframes for this animation
		unsigned frameCount = currentAnimation->GetRigKeyFrameCount();

		// get num of bones in this rig
		unsigned boneCount = rig->GetBoneCount();

		// make sure the rig bones match animation bones
		UAssert(boneCount == currentAnimation->GetDesiredBoneCount(), "Attempted to use invalid rig to calculate animations!");

		// determine the 2 current keyframes to use
		// we assume that all frames exist, and that they were baked across all total keyframes
		for (int x = 0; x < frameCount - 1; ++x)
		{
			// get the two current keyframes
			const std::vector<AnimationKeyframe> &f1 = currentAnimation->GetKeyframes(x);
			const std::vector<AnimationKeyframe> &f2 = currentAnimation->GetKeyframes(x + 1);

			// check if the current keyframe set holds the time value between them
			//if (f1[0].length <= time && time < f2[0].length)
			{
				// if it did, interpolate the two keyframes, save values, break out
				interpolateRigKeyFrames(
					f1,
					f2,
					time,
					boneCount,
					m_toParentTransforms
					);

				// kick out, we're done
				break;
			}
		}

		// root bone has no transform, therefor is just defaulted to the first interpolated matrix
		m_toRootTransforms[0] = m_toParentTransforms[0];

		// now we need to go through the bone hierarchy 
		auto &boneData = rig->GetBoneData();

		// iterate through each bone
		for (unsigned x = 1; x < boneCount; ++x)
		{
			// get the toParent transform
			SMat4 &toParent = m_toParentTransforms[x];

			// get the parent to root
			const SMat4 &parentToRoot = m_toRootTransforms[boneData[x].GetParentID()];

			// calculate root transform
			m_toRootTransforms[x] = toParent * parentToRoot;
		}

		// multiply by bone offset transform to get final transform
		auto &offsetMatrices = rig->GetOffsetMatrices();
		for (unsigned x = 0; x < boneCount; ++x)
		{
			outputMatPal[x] = (offsetMatrices[x] * m_toRootTransforms[x]);
            outputMatPal[ x ].Transpose( );
		}
	}

    void AnimationBuilder::InitializeStaticData(void)
    {
        m_animationData.resize( 128 );
        m_animationRigData.resize( 128 );
        m_toParentTransforms.resize( 128 );
        m_toRootTransforms.resize( 128 );
        m_rigCount = 0;
        m_animationCount = 0;
    }

    unsigned AnimationBuilder::addAnimation(void)
    {
        return m_animationCount++;
    }

    unsigned AnimationBuilder::addAnimationRig(void)
    {
        return m_rigCount++;
    }

    Animation *AnimationBuilder::GetAnimationByIndex(const unsigned index)
    {
        return &m_animationData[ index ];
    }

    Animation* AnimationBuilder::GetAnimationByName(const std::string& name)
    {
        return m_name2Animation[ name ];
    }

    AnimationRig *AnimationBuilder::GetAnimationRigByIndex(const unsigned index)
    {
        return &m_animationRigData[ index ];
    }

    AnimationRig* AnimationBuilder::GetAnimationRigByName(const std::string& name)
    {
        return m_name2Rig[ name ];
    }

    int AnimationBuilder::LoadAnimation(const graphics::ufmt_loader::AnimInfo &info)
    {
		unsigned animIndex = addAnimation();
		auto animation = GetAnimationByIndex(animIndex);

		// total clip count
		unsigned clipCount = info.clipCount;

		// total bone count
		unsigned boneCount = info.boneCount;

		// total keyframe Count
		// since we are using unified animation keyframes,
		// every keyIndices contain same number of keyframes
		// but need to be fixed
		unsigned keyCount = info.keyIndices[0][0]; 

		// resize arrays to handle bone size
		animation->SetData(keyCount, boneCount);

		// set name
		animation->SetName(info.name);

		// LOAD ANIMATION
		// iterate through all keyframes in this RigKeyframe, iterate through each RigKeyframe,
		for (unsigned rigIndex = 0; rigIndex < keyCount; ++rigIndex)
		{
			for (unsigned boneIndex = 0; boneIndex < boneCount; ++boneIndex)
			{
				// get keyframe from data
				// need to be fixed too(remove clip or modify it)
				auto keyframe = info.keyframes[0][boneIndex][rigIndex];

				// get values
				auto trans = SVec3(keyframe.trans.x, keyframe.trans.y, keyframe.trans.z);
				auto scale = SVec3(1, 1, 1);
				auto rot = SQuat(keyframe.rot.x, keyframe.rot.y, keyframe.rot.z, keyframe.rot.w);

				// add keyframe to animation
				animation->AddKeyframe(
					rigIndex,
					boneIndex,
					trans,
					scale,
					rot,
					keyframe.time
					);
			}
		}

		// set values, return
		m_name2Animation[info.name] = animation;
		return animIndex;
    }

    int AnimationBuilder::LoadBoneData(const graphics::ufmt_loader::ModelInfo &modelData)
    {
        unsigned boneCount = modelData.mboneCount;

        // vector of vectors for storing binary tree
        std::vector<std::vector<unsigned> >hierarchy;
        hierarchy.resize( boneCount );

        // 1. Generate binary tree for bone information
        for ( unsigned x = 0; x < boneCount; ++x )
        {
            // grab current node
            auto &node = modelData.marrBones[ x ];

            if ( node.mParentIndex == -1 )
                continue;

            // push index into parent's vector
            hierarchy[ node.mParentIndex ].push_back( x );
        }

        // create a new rig
        int rigIndex = addAnimationRig( );

        // get a pointer to the rig
        auto rig = GetAnimationRigByIndex( rigIndex );

        rig->SetName( modelData.marrBones->name );
        rig->InitializeRig( boneCount );
        rec_LoadBoneMesh( hierarchy, 0, -1, modelData.marrBones, rig );

        // save the data in the maps, return
        m_name2Rig[ modelData.marrBones->name ] = rig;
        return rigIndex;
    }

	void AnimationBuilder::interpolateRigKeyFrames(
		const std::vector<AnimationKeyframe>& frame1,
		const std::vector<AnimationKeyframe>& frame2,
		const float time,
		const unsigned boneCount,
		std::vector<SMat4> &finalTransform
		)
	{
		// get the percentage between current frame and next frame
		float lerpPercent = (time - frame1[0].length) / (frame2[0].length - frame1[0].length);

		// for each one, interpolate between frame1[x] and frame2[x] with time, save result in finalTransform[x]
		for (unsigned x = 0; x < boneCount; ++x)
		{
			//// get the current guy
			//SMat4 &current = finalTransform[x];
			//
			//// position
			//SVec3 p = frame1[x].translation * lerpPercent + (1.0f - lerpPercent) * frame2[x].translation;
			//
			//// scale
			//SVec3 s = frame1[x].scale * lerpPercent + (1.0f - lerpPercent) * frame2[x].scale;
			//
			//// rotation
			//SQuat q = frame1[x].rotation.Slerp(frame2[x].rotation, lerpPercent);
			//
			//// construct matrix for this matrix
			//current = SMat4(p, q, s);
			// get the current guy

			SMat4 &current = finalTransform[x];

			// position
			SVec3 p = frame1[x].translation;

			// scale
			SVec3 s = frame1[x].scale;

			// rotation
			SQuat q = frame1[x].rotation;

			// construct matrix for this matrix
			current = SMat4(p, q, s);
		}
	}

    void AnimationBuilder::rec_LoadBoneMesh(
        std::vector<std::vector<unsigned>>& hierarchy,
        unsigned currentIndex,
        unsigned parentIndex,
        graphics::ufmt_loader::BoneInfo* rigData,
        AnimationRig* rig
    )
    {
        // grab bone
        auto &bone = rigData[ currentIndex ];

        // grab bone data
        auto &boneData = bone;

        // get bone transform ///////////////////////////////////////
        auto boneTrans = SVec3(
            boneData.boneSpacePosition.x,
            boneData.boneSpacePosition.y,
            boneData.boneSpacePosition.z
        );

        auto boneScale = SVec3( 1, 1, 1 );

        auto boneRot = SQuat(
            boneData.boneSpaceRotation.x,
            boneData.boneSpaceRotation.y,
            boneData.boneSpaceRotation.z,
            boneData.boneSpaceRotation.w
        );


        // get bind transform ///////////////////////////////////////
        auto bindTrans = SVec3( 
            boneData.bindPosition.x, 
            boneData.bindPosition.y, 
            boneData.bindPosition.z 
        );

        auto bindScale = SVec3( 1, 1, 1 );

        auto bindRot = SQuat( 
            boneData.bindRotation.x, 
            boneData.bindRotation.y, 
            boneData.bindRotation.z, 
            boneData.bindRotation.w 
        );

        // add as a bone to the current mesh
        rig->AddBone(
            boneData.name,
            boneTrans,
            boneScale,
            boneRot,
            bindTrans,
            bindScale,
            bindRot,
            parentIndex
        );

        // iterate through all existing children
        for ( auto &x : hierarchy[ currentIndex ] )
        {
            rec_LoadBoneMesh( hierarchy, x, currentIndex, rigData, rig );
        }
    }
}