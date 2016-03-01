/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** MeshInfo.cpp
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "UrsinePrecompiled.h"
#include "MeshInfo.h"

namespace ursine
{
    namespace graphics
    {
        namespace ufmt_loader
        {
            MeshInfo::MeshInfo() :
                meshVtxInfoCount(0)
                , meshVtxIdxCount(0)
                , mtrlCount(0)
                , mtrlIndexCount(0)
            {
            }

            MeshInfo::~MeshInfo()
            {
                ReleaseData();
            }

            void MeshInfo::ReleaseData()
            {
                name = "";

                meshVtxInfoCount = 0;
                meshVtxIdxCount = 0;
                mtrlCount = 0;
                mtrlIndexCount = 0;

                mtrlName.clear();
                meshVtxInfos.clear();
                meshVtxIndices.clear();
                materialIndices.clear();
            }

            bool MeshInfo::SerializeIn(HANDLE hFile)
            {
                DWORD nBytesRead;
                unsigned int i = 0;

                if (hFile != INVALID_HANDLE_VALUE)
                {
                    char tmp_name[MAXTEXTLEN];
                    ReadFile(hFile, tmp_name, sizeof(char) * MAXTEXTLEN, &nBytesRead, nullptr);
                    name = tmp_name;
                    ReadFile(hFile, &meshVtxInfoCount, sizeof(unsigned int), &nBytesRead, nullptr);
                    ReadFile(hFile, &meshVtxIdxCount, sizeof(unsigned int), &nBytesRead, nullptr);
                    ReadFile(hFile, &mtrlCount, sizeof(unsigned int), &nBytesRead, nullptr);
                    ReadFile(hFile, &mtrlIndexCount, sizeof(unsigned int), &nBytesRead, nullptr);

                    meshVtxInfos.resize(meshVtxInfoCount);
                    for (i = 0; i < meshVtxInfoCount; ++i)
                    {
                        MeshVertex newMV;
                        ReadFile(hFile, &newMV, sizeof(MeshVertex), &nBytesRead, nullptr);
                        meshVtxInfos[i] = newMV;
                    }
                    meshVtxIndices.resize(meshVtxIdxCount);
                    for (i = 0; i < meshVtxIdxCount; ++i)
                    {
                        unsigned int mvi;
                        ReadFile(hFile, &mvi, sizeof(unsigned int), &nBytesRead, nullptr);
                        meshVtxIndices[i] = mvi;
                    }
                    mtrlName.resize(mtrlCount);
                    for (i = 0; i < mtrlCount; ++i)
                    {
                        ReadFile(hFile, tmp_name, sizeof(char) * MAXTEXTLEN, &nBytesRead, nullptr);
                        mtrlName[i] = tmp_name;
                    }
                    materialIndices.resize(mtrlIndexCount);
                    for (i = 0; i < mtrlIndexCount; ++i)
                    {
                        unsigned int mi;
                        ReadFile(hFile, &mi, sizeof(unsigned int), &nBytesRead, nullptr);
                        materialIndices[i] = mi;
                    }
                }
                return true;
            }

            bool MeshInfo::SerializeOut(HANDLE hFile)
            {
                DWORD nBytesWrite;
                unsigned int i = 0;

                if (hFile != INVALID_HANDLE_VALUE)
                {
                    char tmp_name[MAXTEXTLEN];
                    lstrcpy(tmp_name, name.c_str());
                    WriteFile(hFile, tmp_name, sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);
                    WriteFile(hFile, &meshVtxInfoCount, sizeof(unsigned int), &nBytesWrite, nullptr);
                    WriteFile(hFile, &meshVtxIdxCount, sizeof(unsigned int), &nBytesWrite, nullptr);
                    WriteFile(hFile, &mtrlCount, sizeof(unsigned int), &nBytesWrite, nullptr);
                    WriteFile(hFile, &mtrlIndexCount, sizeof(unsigned int), &nBytesWrite, nullptr);

                    if (meshVtxInfos.size() > 0)
                    {
                        for (auto iter : meshVtxInfos)
                            WriteFile(hFile, &iter, sizeof(MeshVertex), &nBytesWrite, nullptr);
                    }

                    if (meshVtxIndices.size() > 0)
                    {
                        for (auto iter : meshVtxIndices)
                            WriteFile(hFile, &iter, sizeof(unsigned int), &nBytesWrite, nullptr);
                    }

                    if (mtrlName.size() > 0)
                    {
                        for (auto iter : mtrlName)
                        {
                            lstrcpy(tmp_name, iter.c_str());
                            WriteFile(hFile, tmp_name, sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);
                        }
                    }

                    if (materialIndices.size() > 0)
                    {
                        for (auto iter : materialIndices)
                            WriteFile(hFile, &iter, sizeof(unsigned int), &nBytesWrite, nullptr);
                    }
                }
                return true;
            }

            void MeshInfo::Read(resources::ResourceReader &input)
            {
                unsigned stringSize;
                
                input >> stringSize;
                name.resize(stringSize);
                input.ReadBytes(&name[0], stringSize);

                input.ReadBytes( reinterpret_cast<char*>(&meshVtxInfoCount)  , sizeof(unsigned int) );
                input.ReadBytes( reinterpret_cast<char*>(&meshVtxIdxCount)   , sizeof(unsigned int) );
                input.ReadBytes( reinterpret_cast<char*>(&mtrlCount)         , sizeof(unsigned int) );
                input.ReadBytes( reinterpret_cast<char*>(&mtrlIndexCount)    , sizeof(unsigned int) );

                unsigned int i = 0;

                meshVtxInfos.resize(meshVtxInfoCount);
                for (i = 0; i < meshVtxInfoCount; ++i)
                    input.ReadBytes( reinterpret_cast<char*>(&meshVtxInfos[i]), sizeof(MeshVertex) );
                
                meshVtxIndices.resize(meshVtxIdxCount);
                for (i = 0; i < meshVtxIdxCount; ++i)
                    input.ReadBytes( reinterpret_cast<char*>(&meshVtxIndices[i]), sizeof(unsigned int) );
                
                mtrlName.resize(mtrlCount);
                for (i = 0; i < mtrlCount; ++i)
                {
                    input >> stringSize;
                    mtrlName[i].resize(stringSize);
                    input.ReadBytes( &mtrlName[i][0], stringSize);
                }
                
                materialIndices.resize(mtrlIndexCount);
                for (i = 0; i < mtrlIndexCount; ++i)
                    input.ReadBytes( reinterpret_cast<char*>(&materialIndices[i]), sizeof(unsigned int) );
            }

            void MeshInfo::Write(resources::pipeline::ResourceWriter &output)
            {
                output << name.size();
                output << name;
                
                output.WriteBytes( reinterpret_cast<char*>(&meshVtxInfoCount)   , sizeof(unsigned int) );
                output.WriteBytes( reinterpret_cast<char*>(&meshVtxIdxCount)    , sizeof(unsigned int) );
                output.WriteBytes( reinterpret_cast<char*>(&mtrlCount)          , sizeof(unsigned int) );
                output.WriteBytes( reinterpret_cast<char*>(&mtrlIndexCount)     , sizeof(unsigned int) );
                                
                if (meshVtxInfos.size() > 0)
                {
                    for (auto &iter : meshVtxInfos)
                        output.WriteBytes( reinterpret_cast<char*>(&iter), sizeof(MeshVertex) );
                }
                
                if (meshVtxIndices.size() > 0)
                {
                    for (auto &iter : meshVtxIndices)
                        output.WriteBytes( reinterpret_cast<char*>(&iter), sizeof(unsigned int) );
                }
                
                if (mtrlName.size() > 0)
                {
                    for (auto &iter : mtrlName)
                    {
                        output << iter.size();
                        output << iter;
                    }
                }
                
                if (materialIndices.size() > 0)
                {
                    for (auto &iter : materialIndices)
                        output.WriteBytes( reinterpret_cast<char*>(&iter), sizeof(unsigned int) );
                }
            }
        };
    };
};