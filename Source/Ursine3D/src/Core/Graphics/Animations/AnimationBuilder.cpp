#include "UrsinePrecompiled.h"
#include "AnimationBuilder.h"
#include "ModelInfo.h"

namespace ursine
{
    std::vector<Animation>                          AnimationBuilder::m_animationData;
    std::vector<AnimationRig>                       AnimationBuilder::m_animationRigData;
    std::unordered_map<std::string, Animation*>     AnimationBuilder::m_name2Animation;
    std::unordered_map<unsigned, Animation*>        AnimationBuilder::m_index2Animation;
    std::unordered_map<std::string, AnimationRig*>  AnimationBuilder::m_name2Rig;
    std::unordered_map<unsigned, AnimationRig*>     AnimationBuilder::m_index2Rig;

    void AnimationBuilder::InitializeStaticData(void)
    {
        m_animationData.resize( 100 );
        m_animationRigData.resize( 100 );
    }

    unsigned AnimationBuilder::addAnimation(void)
    {
        unsigned index = static_cast<unsigned>(m_animationData.size( ));
        m_animationData.push_back( Animation( ) );
        return index;
    }

    unsigned AnimationBuilder::addAnimationRig(void)
    {
        unsigned index = static_cast<unsigned>(m_animationRigData.size( ));
        m_animationRigData.push_back( AnimationRig( ) );
        return index;
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
        unsigned animIndex = addAnimation( );
        auto animation = GetAnimationByIndex( animIndex );

        // total clip count
        unsigned clipCount = info.clipCount;

        // total bone count
        unsigned boneCount = info.boneCount;

        // resize arrays to handle bone size
        animation->SetData( clipCount, boneCount );

        // set name
        animation->SetName( info.name );

        // LOAD ANIMATION
        // iterate through each RigKeyframe,
        for ( unsigned rigIndex = 0; rigIndex < clipCount; ++rigIndex )
        {
            // iterate through all keyframes in this RigKeyframe,
            for ( unsigned bone = 0; bone < boneCount; ++bone )
            {
                // get keyframe from data
                auto keyframe = info.keyframes[ rigIndex ][ bone ];

                // skip if it doesn't exist... will only happen if animation is not baked. Assert?
                if ( keyframe == nullptr )
                    continue;

                // get values
                auto trans = SVec3( keyframe->trans.x, keyframe->trans.y, keyframe->trans.z );
                auto scale = SVec3( 1, 1, 1 );
                auto rot = SQuat( keyframe->rot.x, keyframe->rot.y, keyframe->rot.z, keyframe->rot.w );

                // add keyframe to animation
                animation->AddKeyframe(
                    rigIndex,
                    bone,
                    trans,
                    scale,
                    rot,
                    keyframe->time
                );
            }
        }

        // set values, return
        m_name2Animation[ info.name ] = animation;
        m_index2Animation[ animIndex ] = animation;
        return animIndex;
    }

    int AnimationBuilder::LoadBoneData(const graphics::ufmt_loader::ModelInfo &modelData)
    {
        unsigned boneCount = modelData.mskinCount;

        // vector of vectors for storing binary tree
        std::vector<std::vector<unsigned> >hierarchy;
        hierarchy.resize( boneCount );

        // 1. Generate binary tree for bone information
        for ( unsigned x = 0; x < boneCount; ++x )
        {
            // grab current node
            auto &node = modelData.marrSkins[ x ];

            // push index into parent's vector
            hierarchy[ node.mbones.mParentIndex ].push_back( x );
        }

        // create a new rig
        int rigIndex = addAnimationRig( );

        // get a pointer to the rig
        auto rig = GetAnimationRigByIndex( rigIndex );

        rig->SetName( modelData.marrSkins->name );
        rec_LoadBoneMesh( hierarchy, 0, -1, modelData.marrSkins, rig );

        // save the data in the maps, return
        m_name2Rig[ modelData.marrSkins->name ] = rig;
        m_index2Rig[ rigIndex ] = rig;
        return rigIndex;
    }

    void AnimationBuilder::rec_LoadBoneMesh(
        std::vector<std::vector<unsigned>>& hierarchy,
        unsigned currentIndex,
        unsigned parentIndex,
        graphics::ufmt_loader::SkinInfo* rigData,
        AnimationRig* rig
    )
    {
        // grab bone
        auto &bone = rigData[ currentIndex ];

        // grab bone data
        auto &boneData = bone.mbones;

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