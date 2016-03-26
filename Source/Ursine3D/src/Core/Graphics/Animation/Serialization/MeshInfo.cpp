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
            MeshInfo::MeshInfo(void) 
                : meshVtxInfoCount( 0 )
                , meshVtxIdxCount( 0 )
                , mtrlCount( 0 )
                , mtrlIndexCount( 0 ) { }

            MeshInfo::~MeshInfo(void)
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
                input.Read( name );

                input.Read( meshVtxInfoCount );
                input.Read( meshVtxIdxCount );
                input.Read( mtrlCount );
                input.Read( mtrlIndexCount );

                unsigned int i = 0;

                meshVtxInfos.resize( meshVtxInfoCount );

                for (i = 0; i < meshVtxInfoCount; ++i)
                    input.Read( meshVtxInfos[ i ] );

                meshVtxIndices.resize( meshVtxIdxCount );

                for (i = 0; i < meshVtxIdxCount; ++i)
                    input.Read( meshVtxIndices[ i ] );

                mtrlName.resize( mtrlCount );

                for (i = 0; i < mtrlCount; ++i)
                    input.Read( mtrlName[ i ] );

                materialIndices.resize( mtrlIndexCount );

                for (i = 0; i < mtrlIndexCount; ++i)
                    input.Read( materialIndices[ i ] );
            }

            void MeshInfo::Write(resources::pipeline::ResourceWriter &output)
            {
                output.Write( name );

                output.Write( meshVtxInfoCount );
                output.Write( meshVtxIdxCount );
                output.Write( mtrlCount );
                output.Write( mtrlIndexCount );

                if (meshVtxInfos.size( ) > 0)
                {
                    for (auto &iter : meshVtxInfos)
                        output.Write( iter );
                }

                if (meshVtxIndices.size( ) > 0)
                {
                    for (auto &iter : meshVtxIndices)
                        output.Write( iter );
                }

                if (mtrlName.size( ) > 0)
                {
                    for (auto &iter : mtrlName)
                        output.Write( iter );
                }

                if (materialIndices.size( ) > 0)
                {
                    for (auto &iter : materialIndices)
                        output.Write( &iter );
                }
            }
        };
    };
};
