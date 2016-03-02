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
                input.ReadString( name );
                
                input.Read( animCount );

                animDataArr.resize( animCount );

                for (auto &animation : animDataArr)
                {
                    input.ReadString( animation.clipname );

                    input.Read( animation.clipCount );
                    input.Read( animation.boneCount );

                    animation.keyIndices.resize( animation.clipCount );
                    animation.keyframes.resize( animation.clipCount );
                    
                    unsigned int i = 0, j = 0;

                    for (i = 0; i < animation.clipCount; ++i)
                    {
                        animation.keyIndices[ i ].resize( animation.boneCount );
                        animation.keyframes[ i ].resize( animation.boneCount );

                        for (j = 0; j < animation.boneCount; ++j)
                        {
                            auto &boneIndicies = animation.keyIndices[ i ][ j ];

                            input.Read( boneIndicies );

                            animation.keyframes[ i ][ j ].resize( boneIndicies );

                            input.ReadBytes( 
                                &animation.keyframes[ i ][ j ][ 0 ], 
                                sizeof( FBX_DATA::KeyFrame ) * boneIndicies 
                            );
                        }
                    }
                }
            }

            void AnimInfo::Write(resources::pipeline::ResourceWriter &output)
            {
                output.WriteString( name );

                output.Write( animCount );

                for (auto &animation : animDataArr)
                {
                    output.WriteString( animation.clipname );

                    output.Write( animation.clipCount );
                    output.Write( animation.boneCount );

                    unsigned int i = 0, j = 0;

                    for (i = 0; i < animation.clipCount; ++i)
                    {
                        for (j = 0; j < animation.boneCount; ++j)
                        {
                            auto boneIndicies = animation.keyIndices[ i ][ j ];

                            output.Write( boneIndicies );

                            output.WriteBytes( 
                                &animation.keyframes[ i ][ j ][ 0 ], 
                                sizeof( FBX_DATA::KeyFrame ) * boneIndicies 
                            );
                        }
                    }
                }
            }
        };
    };
};