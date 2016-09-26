/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ModelInfo.cpp
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "UrsinePrecompiled.h"
#include "ModelInfo.h"

namespace ursine
{
    namespace graphics
    {
        /*namespace ufmt_loader
        {
            ModelInfo::ModelInfo(void)
                : mmeshCount( 0 )
                , mmaterialCount( 0 )
                , mboneCount( 0 )
                , mmeshlvlCount( 0 )
                , mriglvlCount( 0 ) { }

            ModelInfo::~ModelInfo()
            {
                mmeshCount = 0;
                mmaterialCount = 0;
                mboneCount = 0;
                mmeshlvlCount = 0;
                mriglvlCount = 0;

                mMeshInfoVec.clear();
                mMtrlInfoVec.clear();
                mBoneInfoVec.clear();
                mMeshLvVec.clear();
                mRigLvVec.clear();
            }

            void ModelInfo::Read(resources::ResourceReader &input)
            {
                input.Read( name );

                input.Read( mmeshCount );
                input.Read( mmaterialCount );
                input.Read( mboneCount );
                input.Read( mmeshlvlCount );
                input.Read( mriglvlCount );

                unsigned i = 0;

                mMeshInfoVec.resize( mmeshCount );

                for (i = 0; i < mmeshCount; ++i)
                    mMeshInfoVec[ i ].Read( input );

                mMtrlInfoVec.resize( mmaterialCount );

                for (i = 0; i < mmaterialCount; ++i)
                    mMtrlInfoVec[ i ].Read( input );

                mBoneInfoVec.resize( mboneCount );

                for (i = 0; i < mboneCount; ++i)
                    mBoneInfoVec[ i ].Read( input );

                mMeshLvVec.resize( mmeshlvlCount );

                for (i = 0; i < mmeshlvlCount; ++i)
                {
                    auto &mesh = mMeshLvVec[ i ];

                    input.Read( mesh.meshName );

                    input.Read( mesh.meshTM );
                    input.Read( mesh.mParentIndex );
                }
                    
                mRigLvVec.resize( mriglvlCount );

                for (i = 0; i < mriglvlCount; ++i)
                {
                    auto &rig = mRigLvVec[ i ];

                    input.Read( rig.boneName );

                    input.Read( rig.mParentIndex );
                }
            }

            void ModelInfo::Write(resources::pipeline::ResourceWriter &output)
            {
                output.Write( name );

                output.Write( mmeshCount );
                output.Write( mmaterialCount );
                output.Write( mboneCount );
                output.Write( mmeshlvlCount );
                output.Write( mriglvlCount );

                if (mMeshInfoVec.size( ) > 0)
                {
                    for (auto &iter : mMeshInfoVec)
                        iter.Write( output );
                }

                if (mMtrlInfoVec.size( ) > 0)
                {
                    for (auto &iter : mMtrlInfoVec)
                        iter.Write( output );
                }

                if (mBoneInfoVec.size( ) > 0)
                {
                    for (auto &iter : mBoneInfoVec)
                        iter.Write( output );
                }

                if (mMeshLvVec.size( ) > 0)
                {
                    for (auto &iter : mMeshLvVec)
                    {
                        output.Write( iter.meshName );
                        output.Write( iter.meshTM );
                        output.Write( iter.mParentIndex );
                    }
                }

                if (mRigLvVec.size( ) > 0)
                {
                    for (auto &iter : mRigLvVec)
                    {
                        output.Write( iter.boneName );
                        output.Write( iter.mParentIndex );
                    }
                }
            }
        }*/
    }
}
