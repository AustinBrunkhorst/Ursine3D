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
                , ISerialize()
            {
            }

            BoneInfo::~BoneInfo()
            {
                ReleaseData();
            }

            void BoneInfo::ReleaseData()
            {
            }

            bool BoneInfo::SerializeIn(HANDLE hFile)
            {
                DWORD nBytesRead;
                if (hFile != INVALID_HANDLE_VALUE)
                {
                    char tmp_name[MAXTEXTLEN];
                    ReadFile(hFile, tmp_name, sizeof(char) * MAXTEXTLEN, &nBytesRead, nullptr);
                    name = tmp_name;
                    ReadFile(hFile, &mParentIndex, sizeof(int), &nBytesRead, nullptr);
                    ReadFile(hFile, &bindPosition, sizeof(pseudodx::XMFLOAT3), &nBytesRead, nullptr);
                    ReadFile(hFile, &bindRotation, sizeof(pseudodx::XMFLOAT4), &nBytesRead, nullptr);
                    ReadFile(hFile, &bindScaling, sizeof(pseudodx::XMFLOAT3), &nBytesRead, nullptr);
                    ReadFile(hFile, &boneSpacePosition, sizeof(pseudodx::XMFLOAT3), &nBytesRead, nullptr);
                    ReadFile(hFile, &boneSpaceRotation, sizeof(pseudodx::XMFLOAT4), &nBytesRead, nullptr);
                    ReadFile(hFile, &boneSpaceScaling, sizeof(pseudodx::XMFLOAT3), &nBytesRead, nullptr);
                }
                return true;
            }

            bool BoneInfo::SerializeOut(HANDLE hFile)
            {
                DWORD nBytesWrite;
                if (hFile != INVALID_HANDLE_VALUE)
                {
                    char tmp_name[MAXTEXTLEN];
                    lstrcpy(tmp_name, name.c_str());
                    WriteFile(hFile, &tmp_name, sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);
                    WriteFile(hFile, &mParentIndex, sizeof(int), &nBytesWrite, nullptr);
                    WriteFile(hFile, &bindPosition, sizeof(pseudodx::XMFLOAT3), &nBytesWrite, nullptr);
                    WriteFile(hFile, &bindRotation, sizeof(pseudodx::XMFLOAT4), &nBytesWrite, nullptr);
                    WriteFile(hFile, &bindScaling, sizeof(pseudodx::XMFLOAT3), &nBytesWrite, nullptr);
                    WriteFile(hFile, &boneSpacePosition, sizeof(pseudodx::XMFLOAT3), &nBytesWrite, nullptr);
                    WriteFile(hFile, &boneSpaceRotation, sizeof(pseudodx::XMFLOAT4), &nBytesWrite, nullptr);
                    WriteFile(hFile, &boneSpaceScaling, sizeof(pseudodx::XMFLOAT3), &nBytesWrite, nullptr);
                }
                return true;
            }

            void BoneInfo::SerializeIn(resources::ResourceReader &input)
            {
                unsigned stringSize;
                std::string str;

                input >> stringSize;
                str.resize(stringSize);
                input.ReadBytes(&name[0], stringSize);

                input >> mParentIndex;

                input.ReadBytes(reinterpret_cast<char*>(&bindPosition), sizeof(pseudodx::XMFLOAT3));
                input.ReadBytes(reinterpret_cast<char*>(&bindRotation), sizeof(pseudodx::XMFLOAT4));
                input.ReadBytes(reinterpret_cast<char*>(&bindScaling), sizeof(pseudodx::XMFLOAT3));

                input.ReadBytes(reinterpret_cast<char*>(&boneSpacePosition), sizeof(pseudodx::XMFLOAT3));
                input.ReadBytes(reinterpret_cast<char*>(&boneSpaceRotation), sizeof(pseudodx::XMFLOAT4));
                input.ReadBytes(reinterpret_cast<char*>(&boneSpaceScaling), sizeof(pseudodx::XMFLOAT3));
            }

            void BoneInfo::SerializeOut(resources::pipeline::ResourceWriter &output)
            {
                output << name.size();
                output << name;

                output << mParentIndex;

                output.WriteBytes(reinterpret_cast<char*>(&bindPosition), sizeof(pseudodx::XMFLOAT3));
                output.WriteBytes(reinterpret_cast<char*>(&bindRotation), sizeof(pseudodx::XMFLOAT4));
                output.WriteBytes(reinterpret_cast<char*>(&bindScaling), sizeof(pseudodx::XMFLOAT3));

                output.WriteBytes(reinterpret_cast<char*>(&boneSpacePosition), sizeof(pseudodx::XMFLOAT3));
                output.WriteBytes(reinterpret_cast<char*>(&boneSpaceRotation), sizeof(pseudodx::XMFLOAT4));
                output.WriteBytes(reinterpret_cast<char*>(&boneSpaceScaling), sizeof(pseudodx::XMFLOAT3));
            }
        };
    };
};