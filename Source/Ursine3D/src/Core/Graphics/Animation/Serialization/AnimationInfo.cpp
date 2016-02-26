/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationInfo.cpp
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "AnimationInfo.h"

namespace ursine
{
    namespace graphics
    {
        namespace ufmt_loader
        {
            void AnimData::ReleaseData()
            {
                keyIndices.clear();
                keyframes.clear();
            }

            AnimInfo::AnimInfo()
                : name("")
                , animCount(0)
                , ISerialize()
            {}

            AnimInfo::~AnimInfo()
            {
                ReleaseData();
            }

            void AnimInfo::ReleaseData()
            {
                for (auto iter = animDataArr.begin(); iter != animDataArr.end(); ++iter)
                    (*iter).ReleaseData();
            }

            bool AnimInfo::SerializeIn(HANDLE hFile)
            {
                DWORD nByteRead;
                unsigned int i = 0, j = 0, k = 0, l = 0;
                char tmp_name[MAXTEXTLEN];
                ReadFile(hFile, &tmp_name, sizeof(char) * MAXTEXTLEN, &nByteRead, nullptr);
                name = tmp_name;
                ReadFile(hFile, &animCount, sizeof(unsigned int), &nByteRead, nullptr);
                animDataArr.resize(animCount);
                for (auto iter = animDataArr.begin(); iter != animDataArr.end(); ++iter)
                {
                    // serializing counts
                    ReadFile(hFile, &tmp_name, sizeof(char) * MAXTEXTLEN, &nByteRead, nullptr);
                    iter->clipname = tmp_name;
                    ReadFile(hFile, &iter->clipCount, sizeof(unsigned int), &nByteRead, nullptr);
                    ReadFile(hFile, &iter->boneCount, sizeof(unsigned int), &nByteRead, nullptr);
                    iter->keyIndices.resize(iter->clipCount);
                    iter->keyframes.resize(iter->clipCount);
                    for (j = 0; j < animDataArr[i].clipCount; ++j)
                    {
                        iter->keyIndices[j].resize(iter->boneCount);
                        iter->keyframes[j].resize(iter->boneCount);
                        for (k = 0; k < iter->boneCount; ++k)
                        {
                            ReadFile(hFile, &iter->keyIndices[j][k], sizeof(unsigned int), &nByteRead, nullptr);
                            iter->keyframes[j][k].resize(iter->keyIndices[j][k]);
                            for (l = 0; l < iter->keyIndices[j][k]; ++l)
                            {
                                ReadFile(hFile, &iter->keyframes[j][k][l], sizeof(FBX_DATA::KeyFrame), &nByteRead, nullptr);
                            }
                        }
                    }
                }
                return true;
            }

            bool AnimInfo::SerializeOut(HANDLE hFile)
            {
                DWORD nBytesWrite;
                unsigned int i = 0, j = 0, k = 0, l = 0;

                if (INVALID_HANDLE_VALUE != hFile)
                {
                    char tmp_name[MAXTEXTLEN];
                    lstrcpy(tmp_name, name.c_str());
                    WriteFile(hFile, &tmp_name, sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);
                    WriteFile(hFile, &animCount, sizeof(unsigned int), &nBytesWrite, nullptr);
                    for (auto iter = animDataArr.begin(); iter != animDataArr.end(); ++iter)
                    {
                        // serializing counts
                        lstrcpy(tmp_name, iter->clipname.c_str());
                        WriteFile(hFile, &tmp_name, sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);
                        WriteFile(hFile, &iter->clipCount, sizeof(unsigned int), &nBytesWrite, nullptr);
                        WriteFile(hFile, &iter->boneCount, sizeof(unsigned int), &nBytesWrite, nullptr);
                        for (j = 0; j < iter->clipCount; ++j)
                        {
                            for (k = 0; k < iter->boneCount; ++k)
                            {
                                WriteFile(hFile, &iter->keyIndices[j][k], sizeof(unsigned int), &nBytesWrite, nullptr);
                                for (l = 0; l < iter->keyIndices[j][k]; ++l)
                                {
                                    FBX_DATA::KeyFrame* currKF = &iter->keyframes[j][k][l];
                                    WriteFile(hFile, currKF, sizeof(FBX_DATA::KeyFrame), &nBytesWrite, nullptr);
                                }
                            }
                        }
                    }
                }
                return true;
            }

            void AnimInfo::SerializeIn(resources::ResourceReader &input)
            {
            }

            void AnimInfo::SerializeOut(resources::pipeline::ResourceWriter &output)
            {
            }
        };
    };
};