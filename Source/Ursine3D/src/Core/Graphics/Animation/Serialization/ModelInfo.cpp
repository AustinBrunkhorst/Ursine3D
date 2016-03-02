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
        namespace ufmt_loader
        {
            ModelInfo::ModelInfo()
                : mmeshCount(0)
                , mmaterialCount(0)
                , mboneCount(0)
                , mmeshlvlCount(0)
                , mriglvlCount(0)
            {
            }

            ModelInfo::~ModelInfo()
            {
                ReleaseData();
            }

            void ModelInfo::ReleaseData()
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
                unsigned stringSize;
                
                input >> stringSize;
                name.resize(stringSize);
                input.ReadBytes(&name[0], stringSize);
                
                input.ReadBytes( reinterpret_cast<char*>( &mmeshCount)      , sizeof(unsigned int) );
                input.ReadBytes( reinterpret_cast<char*>( &mmaterialCount)  , sizeof(unsigned int) );
                input.ReadBytes( reinterpret_cast<char*>( &mboneCount)      , sizeof(unsigned int) );
                input.ReadBytes( reinterpret_cast<char*>( &mmeshlvlCount)   , sizeof(unsigned int) );
                input.ReadBytes( reinterpret_cast<char*>( &mriglvlCount)    , sizeof(unsigned int) );

                unsigned int i = 0;

                mMeshInfoVec.resize(mmeshCount);
                for (i = 0; i < mmeshCount; ++i)
                    mMeshInfoVec[i].Read(input);
                
                mMtrlInfoVec.resize(mmaterialCount);
                for (i = 0; i < mmaterialCount; ++i)
                    mMtrlInfoVec[i].Read(input);
                
                mBoneInfoVec.resize(mboneCount);
                for (i = 0; i < mboneCount; ++i)
                    mBoneInfoVec[i].Read(input);
                
                mMeshLvVec.resize(mmeshlvlCount);
                for (i = 0; i < mmeshlvlCount; ++i)
                {
                    input >> stringSize;
                    mMeshLvVec[i].meshName.resize(stringSize);
                    input.ReadBytes( &mMeshLvVec[i].meshName[0], stringSize);
                    input.ReadBytes( reinterpret_cast<char*>(&mMeshLvVec[i].meshTM), sizeof(SMat4) );
                    input.ReadBytes( reinterpret_cast<char*>(&mMeshLvVec[i].mParentIndex), sizeof(unsigned int));
                }
                
                mRigLvVec.resize(mriglvlCount);
                for (i = 0; i < mriglvlCount; ++i)
                {
                    input >> stringSize;
                    mRigLvVec[i].boneName.resize(stringSize);
                    input.ReadBytes( &mRigLvVec[i].boneName[0], stringSize);
                    input.ReadBytes( reinterpret_cast<char*>(&mRigLvVec[i].mParentIndex), sizeof(unsigned int));
                }
            }

            void ModelInfo::Write(resources::pipeline::ResourceWriter &output)
            {
                output << name.size();
                output << name;

                output.WriteBytes( reinterpret_cast<char*>(&mmeshCount)     , sizeof(unsigned int) );
                output.WriteBytes( reinterpret_cast<char*>(&mmaterialCount) , sizeof(unsigned int) );
                output.WriteBytes( reinterpret_cast<char*>(&mboneCount)     , sizeof(unsigned int) );
                output.WriteBytes( reinterpret_cast<char*>(&mmeshlvlCount)  , sizeof(unsigned int) );
                output.WriteBytes( reinterpret_cast<char*>(&mriglvlCount)   , sizeof(unsigned int) );

                if (mMeshInfoVec.size() > 0)
                {
                    for (auto &iter : mMeshInfoVec)
                        iter.Write(output);
                }
                
                if (mMtrlInfoVec.size() > 0)
                {
                    for (auto &iter : mMtrlInfoVec)
                        iter.Write(output);
                }
                
                if (mBoneInfoVec.size() > 0)
                {
                    for (auto &iter : mBoneInfoVec)
                        iter.Write(output);
                }
                
                if (mMeshLvVec.size() > 0)
                {
                    for (auto &iter : mMeshLvVec)
                    {
                        output << iter.meshName.size();
                        output << iter.meshName;
                        output.WriteBytes( reinterpret_cast<char*>(&iter.meshTM), sizeof(SMat4) );
                        output.WriteBytes( reinterpret_cast<char*>(&iter.mParentIndex), sizeof(unsigned int) );
                    }
                }
                
                if (mRigLvVec.size() > 0)
                {
                    for (auto &iter : mRigLvVec)
                    {
                        output << iter.boneName.size();
                        output << iter.boneName;
                        output.WriteBytes( reinterpret_cast<char*>(&iter.mParentIndex), sizeof(unsigned int) );
                    }
                }
            }
        };
    };
};