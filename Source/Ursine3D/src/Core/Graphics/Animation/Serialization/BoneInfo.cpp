/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BoneInfo.cpp
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "UrsinePrecompiled.h"
#include "BoneInfo.h"

namespace ursine
{
    namespace graphics
    {
        namespace ufmt_loader
        {
            BoneInfo::BoneInfo(void)
                : mParentIndex( -1 )
                , bindPosition( 0, 0, 0 )
                , bindRotation( 0, 0, 0, 1 )
                , bindScaling( 1, 1, 1 )
                , boneSpacePosition( 0, 0, 0 )
                , boneSpaceRotation( 0, 0, 0, 1 )
                , boneSpaceScaling( 1, 1, 1 ) { }

            BoneInfo::~BoneInfo(void)
            {
                ReleaseData( );
            }

            void BoneInfo::ReleaseData(void)
            {
                name = "";
                mParentIndex = 0;

                bindPosition = pseudodx::XMFLOAT3( 0.f, 0.f, 0.f );
                bindRotation = pseudodx::XMFLOAT4( 0.f, 0.f, 0.f, 1.f );
                bindScaling = pseudodx::XMFLOAT3( 1.f, 1.f, 1.f );

                boneSpacePosition = pseudodx::XMFLOAT3( 0.f, 0.f, 0.f );
                boneSpaceRotation = pseudodx::XMFLOAT4( 0.f, 0.f, 0.f, 1.f );
                boneSpaceScaling = pseudodx::XMFLOAT3( 1.f, 1.f, 1.f );
            }

            void BoneInfo::Read(resources::ResourceReader &input)
            {
                input.ReadString( name );

                input.Read( mParentIndex );

                input.Read( bindPosition );
                input.Read( bindRotation );
                input.Read( bindScaling );

                input.Read( boneSpacePosition );
                input.Read( boneSpaceRotation );
                input.Read( boneSpaceScaling );
            }

            void BoneInfo::Write(resources::pipeline::ResourceWriter &output)
            {
                output.WriteString( name );

                output.Write( mParentIndex );

                output.Write( bindPosition );
                output.Write( bindRotation );
                output.Write( bindScaling );
                              
                output.Write( boneSpacePosition );
                output.Write( boneSpaceRotation );
                output.Write( boneSpaceScaling );
            }
        };
    };
};
