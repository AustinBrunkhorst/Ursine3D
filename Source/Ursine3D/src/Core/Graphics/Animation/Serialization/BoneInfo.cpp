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
            BoneInfo::BoneInfo()
                : mParentIndex(-1)
                , bindPosition(0, 0, 0)
                , bindRotation(0, 0, 0, 1)
                , bindScaling(1, 1, 1)
                , boneSpacePosition(0, 0, 0)
                , boneSpaceRotation(0, 0, 0, 1)
                , boneSpaceScaling(1, 1, 1)
            {
            }

            BoneInfo::~BoneInfo()
            {
                ReleaseData();
            }

            void BoneInfo::ReleaseData()
            {
                name = "";
                mParentIndex = 0;

                bindPosition = pseudodx::XMFLOAT3(0.f, 0.f, 0.f);
                bindRotation = pseudodx::XMFLOAT4(0.f, 0.f, 0.f, 1.f);
                bindScaling = pseudodx::XMFLOAT3(1.f, 1.f, 1.f);

                boneSpacePosition = pseudodx::XMFLOAT3(0.f, 0.f, 0.f);
                boneSpaceRotation = pseudodx::XMFLOAT4(0.f, 0.f, 0.f, 1.f);
                boneSpaceScaling = pseudodx::XMFLOAT3(1.f, 1.f, 1.f);
            }
            
            void BoneInfo::Read(resources::ResourceReader &input)
            {
                unsigned stringSize;
                
                input >> stringSize;
                name.resize(stringSize);
                input.ReadBytes(&name[0], stringSize);
                
                input.ReadBytes( reinterpret_cast<char*>(&mParentIndex) , sizeof(unsigned int));
                                 
                input.ReadBytes( reinterpret_cast<char*>(&bindPosition) , sizeof(pseudodx::XMFLOAT3));
                input.ReadBytes( reinterpret_cast<char*>(&bindRotation) , sizeof(pseudodx::XMFLOAT4));
                input.ReadBytes( reinterpret_cast<char*>(&bindScaling)  , sizeof(pseudodx::XMFLOAT3));
                                 
                input.ReadBytes( reinterpret_cast<char*>(&boneSpacePosition) , sizeof(pseudodx::XMFLOAT3));
                input.ReadBytes( reinterpret_cast<char*>(&boneSpaceRotation) , sizeof(pseudodx::XMFLOAT4));
                input.ReadBytes( reinterpret_cast<char*>(&boneSpaceScaling)  , sizeof(pseudodx::XMFLOAT3));
            }

            void BoneInfo::Write(resources::pipeline::ResourceWriter &output)
            {
                output << name.size();
                output << name;
                
                output.WriteBytes(reinterpret_cast<char*>(&mParentIndex) , sizeof(unsigned int));
                
                output.WriteBytes( reinterpret_cast<char*>(&bindPosition) , sizeof(pseudodx::XMFLOAT3));
                output.WriteBytes( reinterpret_cast<char*>(&bindRotation) , sizeof(pseudodx::XMFLOAT4));
                output.WriteBytes( reinterpret_cast<char*>(&bindScaling)  , sizeof(pseudodx::XMFLOAT3));
                
                output.WriteBytes( reinterpret_cast<char*>(&boneSpacePosition)  , sizeof(pseudodx::XMFLOAT3));
                output.WriteBytes( reinterpret_cast<char*>(&boneSpaceRotation)  , sizeof(pseudodx::XMFLOAT4));
                output.WriteBytes( reinterpret_cast<char*>(&boneSpaceScaling)   , sizeof(pseudodx::XMFLOAT3));
            }
        };
    };
};