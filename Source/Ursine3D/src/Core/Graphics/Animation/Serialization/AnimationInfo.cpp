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
                , animDataArr(0)
            {}

            AnimInfo::~AnimInfo()
            {
                ReleaseData();
            }

            void AnimInfo::ReleaseData()
            {
                name = "";

                animCount = 0;

                for (auto iter = animDataArr.begin(); iter != animDataArr.end(); ++iter)
                    (*iter).ReleaseData();
            }

            void AnimInfo::Read(resources::ResourceReader &input)
            {
                unsigned stringSize;

                input >> stringSize;
                name.resize(stringSize);
                input.ReadBytes(&name[0], stringSize);
                
                input.ReadBytes( reinterpret_cast<char*>(&animCount), sizeof(unsigned int) );
                animDataArr.resize(animCount);

                for (auto &iter : animDataArr)
                {
                    // serializing counts
                    input >> stringSize;
                    iter.clipname.resize(stringSize);
                    input.ReadBytes(&iter.clipname[0], stringSize);
                
                    input.ReadBytes( reinterpret_cast<char*>(&iter.clipCount), sizeof(unsigned int) );
                    input.ReadBytes( reinterpret_cast<char*>(&iter.boneCount), sizeof(unsigned int) );
                
                    iter.keyIndices.resize(iter.clipCount);
                    iter.keyframes.resize(iter.clipCount);
                
                    unsigned int i = 0, j = 0, k = 0;
                    for (i = 0; i < iter.clipCount; ++i)
                    {
                        iter.keyIndices[ i ].resize(iter.boneCount);
                        iter.keyframes[ i ].resize(iter.boneCount);
                    
                        for (j = 0; j < iter.boneCount; ++j)
                        {
                            input.ReadBytes( reinterpret_cast <char*>(&iter.keyIndices[ i ][ j ]), sizeof(unsigned int) );

                            auto indicies = iter.keyIndices[ i ][ j ];

                            iter.keyframes[i][j].resize( indicies );
                            
                            input.ReadBytes( reinterpret_cast<char*>(&iter.keyframes[ i ][ j ][ 0 ]), sizeof( FBX_DATA::KeyFrame ) * indicies );
                        }
                    }
                }
            }

            void AnimInfo::Write(resources::pipeline::ResourceWriter &output)
            {
                output << (unsigned)name.size();
                output << name;

                output.WriteBytes( reinterpret_cast<char*>(&animCount), sizeof(unsigned int) );

                for (auto &iter : animDataArr)
                {
                    // serializing counts
                    output << (unsigned)iter.clipname.size();
                    output << iter.clipname;

                    output.WriteBytes( reinterpret_cast<char*>(&iter.clipCount), sizeof(unsigned int) );
                    output.WriteBytes( reinterpret_cast<char*>(&iter.boneCount), sizeof(unsigned int) );

                    unsigned int i = 0, j = 0, k = 0;
                    for (i = 0; i < iter.clipCount; ++i)
                    {
                        for (j = 0; j < iter.boneCount; ++j)
                        {
                            auto indicies = iter.keyIndices[i][j];

                            output.WriteBytes( reinterpret_cast<char*>(&indicies), sizeof(unsigned int) );

                            output.WriteBytes(reinterpret_cast<char*>(&iter.keyframes[i][j][0]), sizeof(FBX_DATA::KeyFrame) * indicies);
                        }
                    }
                }
            }
        };
    };
};