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
    template <typename T>
    static void evaluateKeyFrames(std::vector<T> &keys, ecs::EntityHandle &bone, float time)
    {
        if (keys.size( ))
        {
            auto keySize = static_cast<uint>( keys.size( ) );

            if (keySize == 1)
            {
                keys[ 0 ].SetValue( bone );
            }

            uint index = -1;

            for (uint i = 0; i < keySize; ++i)
            {
                auto &key = keys[ i ];

                if (key.time <= time)
                {
                    index = i;
                }

                if (key.time > time)
                {
                    break;
                }
            }

            if (index != -1)
            {
                if (index == keySize - 1)
                {
                    keys.back( ).SetValue( bone );
                }
                else
                {
                    auto *first = &keys.at( index );
                    auto *next = &keys.at( index + 1 );

                    float t = (time - first->time) / (next->time - first->time);

                    first->Interpolate( next, t, bone );
                }
            }
        }
    }

    AnimationLane::AnimationLane(void)
        : preState( Type::Default )
        , postState( Type::Default )
        , boneIndex( -1 ) { }

    void AnimationLane::Evaluate(ecs::EntityHandle bone, float time)
    {
        if (bone == nullptr)
            return;

        // TODO: Implement the different types
        // TODO: Optimize finding the two keys

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

        evaluateKeyFrames( positionKeys, bone, time );
        evaluateKeyFrames( rotationKeys, bone, time );
        evaluateKeyFrames( scaleKeys, bone, time );
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
