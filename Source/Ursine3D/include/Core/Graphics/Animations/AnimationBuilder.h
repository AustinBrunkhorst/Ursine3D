/* ---------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationBuilder.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once
#include <vector>

#include "AnimationRig.h"
#include "Animation.h"
#include "AnimationState.h"
#include "SkinInfo.h"
#include <unordered_map>

namespace ursine
{
    namespace graphics{namespace ufmt_loader{
        class AnimInfo;
        class ModelInfo;
    }}

    class AnimationBuilder
    {
    public:
        //given a Rig, State, and 2 vectors of matrices, generate matrix palette

        void GenerateAnimationData(
            const AnimationState &animState, 
            const Animation *animation, 
            std::vector<SMat4> &outputMatPal
        );

        /////////////////////////////////////////////////////////////
        // static stuff /////////////////////////////////////////////
        static void InitializeStaticData(void);
        
        // getting an animation
        static Animation *GetAnimationByIndex(const unsigned index);
        static Animation *GetAnimationByName(const std::string &name);

        // getting an animation rig
        static AnimationRig *GetAnimationRigByIndex(const unsigned index);
        static AnimationRig *GetAnimationRigByName(const std::string &name);

        /** @brief loads an animation into builder
        *
        *  this will construct the animation data, reading in ufmt,
        *  and saving it into memory
        *
        *  @param info the animation info
        *  @return index of new resource.
        */
        static int LoadAnimation(const graphics::ufmt_loader::AnimInfo &info);

        /** @brief loads a rig into builder
        *
        *  this will construct the rig data, reading in ufmt,
        *  and saving it into memory
        *
        *  @param info the rig data
        *  @return index of new resource.
        */
        static int LoadBoneData(const graphics::ufmt_loader::ModelInfo &modelData);

    private:
        // add resources
        static unsigned addAnimation(void);
        static unsigned addAnimationRig(void);

        /** @brief recursively load a bone hierarchy
        *
        *  given an existing binary tree, this method will
        *  generate the current bone, then instantiate 
        *  this bone's children
        *
        *  @param hierarchy binary tree of children
        *  @param currentIndex index of current node
        *  @param parentIndex index of the parent
        *  @param rigData pointer to the bone data
        *  @param rig pointer to the rig we are filling
        *  @return Void.
        */
        static void rec_LoadBoneMesh(
            std::vector<std::vector<unsigned>> &hierarchy,
            unsigned currentIndex,
            unsigned parentIndex,
            graphics::ufmt_loader::SkinInfo *rigData,
            AnimationRig *rig
            );

        // all the data
        static std::vector<Animation> m_animationData;
        static std::vector<AnimationRig> m_animationRigData;
        
        // lookup tables for stuff
        static std::unordered_map<std::string, Animation*> m_name2Animation;
        static std::unordered_map<unsigned, Animation*> m_index2Animation;
        static std::unordered_map<std::string, AnimationRig*> m_name2Rig;
        static std::unordered_map<unsigned, AnimationRig*> m_index2Rig;
    };
}
