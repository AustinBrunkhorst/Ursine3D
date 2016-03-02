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
                ReleaseData( );
            }

            void MeshInfo::ReleaseData(void)
            {
                name = "";

                meshVtxInfoCount = 0;
                meshVtxIdxCount = 0;
                mtrlCount = 0;
                mtrlIndexCount = 0;

                mtrlName.clear( );
                meshVtxInfos.clear( );
                meshVtxIndices.clear( );
                materialIndices.clear( );
            }

            void MeshInfo::Read(resources::ResourceReader &input)
            {
                input.ReadString( name );

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
                    input.ReadString( mtrlName[ i ] );

                materialIndices.resize( mtrlIndexCount );

                for (i = 0; i < mtrlIndexCount; ++i)
                    input.Read( materialIndices[ i ] );
            }

            void MeshInfo::Write(resources::pipeline::ResourceWriter &output)
            {
                output.WriteString( name );

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
                        output.WriteString( iter );
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
