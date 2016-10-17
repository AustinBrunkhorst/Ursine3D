/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationLane.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "AnimationLane.h"

namespace ursine
{
    AnimationLane::AnimationLane(void)
        : preState( Type::Default )
        , postState( Type::Default )
        , boneIndex( -1 )
    {
        keyFrames.push_back(
            reinterpret_cast<std::vector<KeyFrame>*>( &positionKeys )
        );

        keyFrames.push_back(
            reinterpret_cast<std::vector<KeyFrame>*>( &rotationKeys )
        );

        keyFrames.push_back(
            reinterpret_cast<std::vector<KeyFrame>*>( &scaleKeys )
        );
    }

    void AnimationLane::Evaluate(ecs::EntityHandle bone, float time)
    {
        for (auto keys : keyFrames)
        {
            // TODO: Implement the different types

            // find the two keys we want
            //if (time < firstFrame)
            //{
            //    // do the thing
            //    return;
            //}

            //if (time > lastFrame)
            //{
            //    // do the thing
            //    return;
            //}
        }
    }

    void AnimationLane::Write(resources::pipeline::ResourceWriter &output)
    {
        output.Write( preState );
        output.Write( postState );
        output.Write( boneIndex );

        output.Write( positionKeys.size( ) );

        for (auto &key : positionKeys)
        {
            output.Write( key.time );
            output.Write( key.value );
        }

        output.Write( rotationKeys.size( ) );

        for (auto &key : rotationKeys)
        {
            output.Write( key.time );
            output.Write( key.value );
        }

        output.Write( scaleKeys.size( ) );

        for (auto &key : scaleKeys)
        {
            output.Write( key.time );
            output.Write( key.value );
        }
    }

    void AnimationLane::Read(resources::ResourceReader &reader)
    {
        reader.Read( preState );
        reader.Read( postState );
        reader.Read( boneIndex );

        size_t count;

        reader.Read( count );

        positionKeys.resize( count );

        for (size_t i = 0; i < count; ++i)
        {
            reader.Read( positionKeys[ i ].time );
            reader.Read( positionKeys[ i ].value );
        }

        reader.Read( count );

        rotationKeys.resize( count );

        for (size_t i = 0; i < count; ++i)
        {
            reader.Read( rotationKeys[ i ].time );
            reader.Read( rotationKeys[ i ].value );
        }

        reader.Read( count );

        scaleKeys.resize( count );

        for (size_t i = 0; i < count; ++i)
        {
            reader.Read( scaleKeys[ i ].time );
            reader.Read( scaleKeys[ i ].value );
        }
    }
}
