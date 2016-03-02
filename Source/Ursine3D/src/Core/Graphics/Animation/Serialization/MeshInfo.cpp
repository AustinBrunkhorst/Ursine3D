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