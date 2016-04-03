/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationBuilder.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/
#include "UrsinePrecompiled.h"
#include "AnimationBuilder.h"
#include "ModelInfo.h"

namespace ursine
{
    std::vector<Animation>                          AnimationBuilder::m_animationData;
    std::vector<AnimationRig>                       AnimationBuilder::m_animationRigData;
    std::unordered_map<std::string, Animation*>     AnimationBuilder::m_name2Animation;
    std::unordered_map<std::string, AnimationRig*>  AnimationBuilder::m_name2Rig;

    std::vector<SVec3>                              AnimationBuilder::m_toParentTransl;
    std::vector<SQuat>                              AnimationBuilder::m_toParentRot;
    std::vector<SVec3>                              AnimationBuilder::m_toParentScl;

    std::vector<SVec3>                              AnimationBuilder::m_toFutParentTransl;
    std::vector<SQuat>                              AnimationBuilder::m_toFutParentRot;
    std::vector<SVec3>                              AnimationBuilder::m_toFutParentScl;

    unsigned AnimationBuilder::m_rigCount;
    unsigned AnimationBuilder::m_animationCount;

    void AnimationBuilder::GenerateAnimationData(
        const AnimationState *currentState,
        const AnimationState *futureState,
        AnimationRig* rig,
        std::vector<SMat4> &outputBoneMatrix,
        std::vector<SMat4> &outputBones,
        const float &transFactor
    )
    {
        // get num of bones in this rig
        unsigned boneCount = rig->GetBoneCount();

        // interpolate current state and animation
        interpolateStateAndAnimation(boneCount, rig, currentState, m_toParentTransl, m_toParentRot, m_toParentScl);

        bool IsfutureAnimation = false;

        if (futureState)
        {
            if (futureState->GetAnimation())
            {
                // interpolate future state and animation
                interpolateStateAndAnimation(boneCount, rig, futureState, m_toFutParentTransl, m_toFutParentRot, m_toFutParentScl);
                IsfutureAnimation = true;
            }
        }

        // root bone has no transform, therefor is just defaulted to the first interpolated matrix
        float trans = transFactor;

        if(!IsfutureAnimation)
        {
            SVec3 t = m_toParentTransl[0];
            SQuat r = m_toParentRot[0];
            SVec3 s = m_toParentScl[0];

            outputBones[0] = SMat4(t, r, s);
        }
        else
        {
            SVec3 t = m_toParentTransl[0] * (1.0f - trans) + m_toFutParentTransl[0] * trans;
            SQuat r = m_toParentRot[0].Slerp(m_toFutParentRot[0], trans);
            SVec3 s = m_toParentScl[0] * (1.0f - trans) + m_toFutParentScl[0] * trans;

            outputBones[0] = SMat4(t, r, s);
        }

        // now we need to go through the bone hierarchy 
        auto &boneData = rig->GetBoneData();

        // iterate through each bone
        for (unsigned x = 1; x < boneCount; ++x)
        {
            SMat4 toParent;

            // get the toParent transform
            if (!IsfutureAnimation)
            {
                SVec3 t = m_toParentTransl[x];
                SQuat r = m_toParentRot[x];
                SVec3 s = m_toParentScl[x];

                toParent = SMat4(t, r, s);
            }
            else
            {
                SVec3 t = m_toParentTransl[x] * (1.0f - trans) + m_toFutParentTransl[x] * trans;
                SQuat r = m_toParentRot[x].Slerp(m_toFutParentRot[x], trans);
                SVec3 s = m_toParentScl[x] * (1.0f - trans) + m_toFutParentScl[x] * trans;

                toParent = SMat4(t, r, s);
            }

            // get the parent to root
            const SMat4 &parentToRoot = outputBones[boneData[x].GetParentID()];

            // calculate root transform
            outputBones[x] = parentToRoot * toParent;
        }

        // multiply by bone offset transform to get final transform
        auto &offsetMatrices = rig->GetOffsetMatrices();

        for (unsigned x = 0; x < boneCount; ++x)
        {
            outputBoneMatrix[x] = (outputBones[x] * offsetMatrices[x]);
        }
    }

    void AnimationBuilder::InitializeStaticData(void)
    {
        m_animationData.resize(128);
        m_animationRigData.resize(128);

        m_toParentTransl.resize(128);
        for (auto &iter : m_toParentTransl)
            iter = SVec3(0, 0, 0);

        m_toParentRot.resize(128);
        for (auto &iter : m_toParentRot)
            iter = SQuat(0.f, 0.f, 0.f, 1.f);

        m_toParentScl.resize(128);
        for (auto &iter : m_toParentScl)
            iter = SVec3(1, 1, 1);

        m_toFutParentTransl.resize(128);
        for (auto &iter : m_toFutParentTransl)
            iter = SVec3(0, 0, 0);

        m_toFutParentRot.resize(128);
        for (auto &iter : m_toFutParentRot)
            iter = SQuat(0.f, 0.f, 0.f, 1.f);

        m_toFutParentScl.resize(128);
        for (auto &iter : m_toFutParentScl)
            iter = SVec3(0, 0, 0);

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
        if (index >= m_animationData.size())
            return nullptr;
        return &m_animationData[index];
    }

    Animation* AnimationBuilder::GetAnimationByName(const std::string& name)
    {
        if (m_name2Animation.end() == m_name2Animation.find(name))
            return nullptr;
        else
            return m_name2Animation[name];
    }

    AnimationRig *AnimationBuilder::GetAnimationRigByIndex(const unsigned index)
    {
        if (index >= m_animationRigData.size())
            return nullptr;
        return &m_animationRigData[index];
    }

    AnimationRig* AnimationBuilder::GetAnimationRigByName(const std::string& name)
    {
        if (m_name2Rig.end() == m_name2Rig.find(name))
            return nullptr;
        else
            return m_name2Rig[name];
    }

    int AnimationBuilder::LoadAnimation(const graphics::ufmt_loader::AnimInfo &info)
    {
        unsigned animIndex = addAnimation();
        auto animation = GetAnimationByIndex(animIndex);

        // total clip count
        unsigned clipCount = info.animDataArr[0].clipCount;

        // total bone count
        unsigned boneCount = info.animDataArr[0].boneCount;

        // total keyframe Count
        // since we are using unified animation keyframes,
        // every keyIndices contain same number of keyframes
        // but need to be fixed
        unsigned keyCount = info.animDataArr[0].keyIndices[0][0];

        // resize arrays to handle bone size
        animation->SetData(keyCount, boneCount);

        // set name - let's use model name
        animation->SetName(info.name);

        // LOAD ANIMATION
        // iterate through all keyframes in this RigKeyframe, iterate through each RigKeyframe,
        for (unsigned rigIndex = 0; rigIndex < keyCount; ++rigIndex)
        {
            for (unsigned boneIndex = 0; boneIndex < boneCount; ++boneIndex)
            {
                // get keyframe from data
                // need to be fixed too(remove clip or modify it)
                auto keyframe = info.animDataArr[0].keyframes[0][boneIndex][rigIndex];

                // get values
                auto trans = SVec3(
                    keyframe.trans.x, 
                    keyframe.trans.y, 
                    keyframe.trans.z
                );

                auto scale = SVec3(
                    keyframe.scl.x,
                    keyframe.scl.y,
                    keyframe.scl.z
                );

                auto rot = SQuat(
                    keyframe.rot.x, 
                    keyframe.rot.y, 
                    keyframe.rot.z, 
                    keyframe.rot.w
                );

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

    int AnimationBuilder::LoadBoneData(const graphics::ufmt_loader::ModelInfo &modelData, const std::string &name)
    {
        unsigned boneCount = modelData.mboneCount;

        // vector of vectors for storing binary tree
        std::vector<std::vector<unsigned> >hierarchy;
        hierarchy.resize(boneCount);

        // 1. Generate binary tree for bone information
        for (unsigned x = 0; x < boneCount; ++x)
        {
            // grab current node
            auto &node = modelData.mBoneInfoVec[x];
            if (-1 == node.mParentIndex)
                continue;

            // push index into parent's vector
            hierarchy[node.mParentIndex].push_back(x);
        }

        // create a new rig
        int rigIndex = addAnimationRig();

        // get a pointer to the rig
        auto rig = GetAnimationRigByIndex(rigIndex);

        rig->SetName(modelData.name);
        rig->InitializeRig(boneCount);
        rec_LoadBoneMesh(hierarchy, 0, -1, modelData.mBoneInfoVec, rig);

        // save the data in the maps, return
        m_name2Rig[name] = rig;
        return rigIndex;
    }

    float AnimationBuilder::linearInterpolation(float &coeff1, float &coeff2, float transRate)
    {
        // for example, coeff1 = 1, coeff2 = 0, then calculation should be
        // ( (1.0f - 0.0f) - trasRate) * frame1 + transRate * frame2
        return (coeff1 - coeff2) - transRate;
    }

    float AnimationBuilder::cubicInterpolation(float coeff[4], float transRate)
    {
        return coeff[1] + 0.5f * transRate * (
            coeff[2] - coeff[0] + transRate * (
                2.f * coeff[0] - 5.f * coeff[1] + 4.f * coeff[2] - coeff[3] + transRate * (
                    3.f * (coeff[1] - coeff[2]) + coeff[3] - coeff[0]
                    )
                )
            );
    }

    float AnimationBuilder::bicubicInterpolation(float coeff[4][4], float transRate_x, float transRate_y)
    {
        float arr[4];

        arr[0] = cubicInterpolation(coeff[0], transRate_y);
        arr[1] = cubicInterpolation(coeff[1], transRate_y);
        arr[2] = cubicInterpolation(coeff[2], transRate_y);
        arr[3] = cubicInterpolation(coeff[3], transRate_y);

        return cubicInterpolation(arr, transRate_x);
    }

    void AnimationBuilder::interpolateRigKeyFrames(
        const std::vector<AnimationKeyframe>& frame1,
        const std::vector<AnimationKeyframe>& frame2,
        const float time,
        const unsigned boneCount,
        std::vector<SVec3> &transl,
        std::vector<SQuat> &rot,
        std::vector<SVec3> &scl,
        AnimationRig *rig
        )
    {
        // get the percentage between current frame and next frame
        auto denom = frame2[0].length - frame1[0].length;
        float lerpPercent = denom == 0.0f ? 1.0f : (time - frame1[0].length) / denom;

        // for each one, interpolate between frame1[x] and frame2[x] with time, save result in finalTransform[x]
        for (unsigned x = 0; x < boneCount; ++x)
        {            
            // position
            SVec3 p = (1.0f - lerpPercent) * frame1[x].translation + frame2[x].translation * lerpPercent;
            
            // scale
            SVec3 s = (1.0f - lerpPercent) * frame1[x].scale + frame2[x].scale * lerpPercent;
            
            // rotation
            SQuat q = frame1[x].rotation.Slerp(frame2[x].rotation, lerpPercent);
            
            auto bone = rig->GetBone( x );

            bone->SetTranslation( p );
            bone->SetRotation( q );
            bone->SetScale( s );

            transl[x] = p;
            rot[x] = q;
            scl[x] = s;
        }
    }

    void AnimationBuilder::interpolateStateAndAnimation(
        const int &boneCount,
        AnimationRig* rig,
        const AnimationState *state,
        std::vector<SVec3> &transl,
        std::vector<SQuat> &rot,
        std::vector<SVec3> &scl
        )
    {
        if (!state)
            return;
        
        // get the currently running animation
        auto animation = state->GetAnimation();

        if (!animation)
            return;

        // get the current time
        float time = (float)state->GetTimePosition();

        // get the total keyframes for this animation
        unsigned frameCount = animation->GetRigKeyFrameCount();
        
        // make sure the rig bones match animation bones
        if (boneCount != animation->GetDesiredBoneCount())
            return;

        bool bFound = false; 

        // determine the 2 current keyframes to use
        // we assume that all frames exist, and that they were baked across all total keyframes
        for (unsigned x = 0; x < frameCount - 1; ++x)
        {
            // get the two current keyframes
            auto &f1 = animation->GetKeyframes(x);
            auto &f2 = animation->GetKeyframes(x + 1);

            // check if the current keyframe set holds the time value between them
            if (f1[0].length <= time && time < f2[0].length)
            {
                // if it did, interpolate the two keyframes, save values, break out
                interpolateRigKeyFrames(
                    f1,
                    f2,
                    time,
                    boneCount,
                    transl, rot, scl,
                    rig
                    );

                bFound = true;

                // kick out, we're done
                break;
            }
        }

        if (!bFound)
        {
            auto &f1 = animation->GetKeyframes(frameCount - 1);
            interpolateRigKeyFrames(
                f1,
                f1,
                time,
                boneCount,
                transl, rot, scl,
                rig
                );
        }
    }

    void AnimationBuilder::rec_LoadBoneMesh(
        std::vector<std::vector<unsigned>>& hierarchy,
        unsigned currentIndex,
        unsigned parentIndex,
        const std::vector<graphics::ufmt_loader::BoneInfo>& rigData,
        AnimationRig* rig
    )
    {
        // grab bone
        auto &bone = rigData[currentIndex];

        // grab bone data
        auto &boneData = bone;

        // get bone transform ///////////////////////////////////////
        auto boneTrans = SVec3(
            boneData.boneSpacePosition.x,
            boneData.boneSpacePosition.y,
            boneData.boneSpacePosition.z
        );

        auto boneScale = SVec3(
            boneData.boneSpaceScaling.x,
            boneData.boneSpaceScaling.y,
            boneData.boneSpaceScaling.z
        );

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

        auto bindScale = SVec3(
            boneData.bindScaling.x,
            boneData.bindScaling.y,
            boneData.bindScaling.z
        );

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
        for (auto &x : hierarchy[currentIndex])
        {
            rec_LoadBoneMesh( 
                hierarchy, 
                x, 
                currentIndex, 
                rigData, 
                rig 
            );
        }
    }
}