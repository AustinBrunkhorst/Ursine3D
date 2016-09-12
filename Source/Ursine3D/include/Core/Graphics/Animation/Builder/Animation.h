/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Animation.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once
#include <string>
#include <vector>

#include "AnimationKeyframe.h"
#include "SVec3.h"
#include "SQuat.h"

//this class holds the keyframes

namespace ursine
{
    class Animation
    {
    public:
        Animation(void);
        Animation(const std::string &name);

        /** @brief adds a keyframe to this animation
        *
        *  given a rigKeyframe, this method will add
        *  a keyframe for a given bone.
        *
        *  @param rigKeyIndex rig keyframe to use
        *  @param boneIndex ID of the bone that uses this keyframe
        *  @param trans bone-space translation
        *  @param scale bone-space scale
        *  @param rot bone-space rotation
        *  @param runTime time that it runs
        *  @return Void.
        */
        void AddKeyframe(
            const unsigned rigKeyIndex,
            const unsigned boneIndex,
            const SVec3 &trans,
            const SVec3 &scale,
            const SQuat &rot,
            const float runTime
            );

        /** @brief sets values that initialize vectors
        *
        *  @param rigKeyFrameCount # of rig keyframes in this
        *                          animation
        *  @param boneCount # of bones this animation will use
        *  @return Void.
        */
        void SetData(
            const unsigned rigKeyFrameCount,
            const unsigned boneCount
            );

        // gettors and setters //////////////////////////////////////

        const std::string &GetName(void) const;
        void SetName(const std::string &name);

        /** @brief retrieves an entire vector of all bone keyframes
        *          for this rig keyframe
        *
        *  @param rigKeyframe the rig keyframe we are using
        *  @return the vector of keyframes
        */
        const std::vector<AnimationKeyframe> &GetKeyframes(const unsigned rigKeyframe) const;

        /** @brief retrieves a specific keyframe
        *
        *  @param rigKeyframe the rig keyframe we are using
        *  @param boneIndex the index of the desired bone
        *  @return the requested keyframe.
        */
        const AnimationKeyframe &GetKeyframe(
            const unsigned rigKeyframe,
            const unsigned boneIndex
            ) const;

        unsigned GetRigKeyFrameCount(void) const;

        unsigned GetDesiredBoneCount(void) const;

    private:
        unsigned m_rigKeyframeCount;
        unsigned m_boneCount;
        std::string m_name;

        // vector (each keyframe) of vectors (each bone's keyframes)
        // so it would be like: 
            //  keyframes[what keyframe are we on][boneIndex]
        // this should help keep things contiguous in memory when looking at a single key
        // frame
        std::vector< std::vector<AnimationKeyframe> > m_keyframes;
    };

    class InStateAnimation
    {
    public:
        //EditorField(
        //    std::string stateName,
        //    GetStateName,
        //    SetStateName
        //    );
        //
        //// how can I add multiple animations in animation state?
        //// how can I use combo box for this?
        //// Array<Animation> => no, in this case, we should edit
        //// Get animation as a resource not by name
        //EditorResourceField(
        //    ursine::resources::AnimationClipData,
        //    clip,
        //    GetClip,
        //    SetClip
        //    );
        //
        //InStateAnimation(void);
        //InStateAnimation(const std::string &name);
        //
        //// gettors and setters //////////////////////////////////////
        //
        //const std::string &GetName(void) const;
        //void SetName(const std::string &name);
        //
        //unsigned GetRigKeyFrameCount(void) const;
        //
        //unsigned GetDesiredBoneCount(void) const;

    private:
        unsigned m_rigKeyframeCount;
        unsigned m_boneCount;
        std::string m_name;

        // vector (each keyframe) of vectors (each bone's keyframes)
        // so it would be like: 
        //  keyframes[what keyframe are we on][boneIndex]
        // this should help keep things contiguous in memory when looking at a single key
        // frame
        std::vector< Animation > m_animations;
    };// Meta(Enable, EnableArrayType, DisplayName("InStateAnimation"));
}