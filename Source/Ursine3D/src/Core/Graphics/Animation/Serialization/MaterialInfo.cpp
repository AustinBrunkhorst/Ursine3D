/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** MaterialInfo.cpp
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "MaterialInfo.h"

namespace ursine
{
    namespace graphics
    {
        namespace ufmt_loader
        {
            MaterialInfo::MaterialInfo(void) 
                : type( FBX_DATA::FbxMaterial::Type_None )
                , ambitype( FBX_DATA::Material_Eles::Fac_None )
                , difftype( FBX_DATA::Material_Eles::Fac_None )
                , emistype( FBX_DATA::Material_Eles::Fac_None )
                , spectype( FBX_DATA::Material_Eles::Fac_None )
                , ambi_mcolor( 0, 0, 0, 1 ), diff_mcolor( 0, 0, 0, 1 )
                , emis_mcolor( 0, 0, 0, 1 ), spec_mcolor( 0, 0, 0, 1 )
                , ambi_mapCount( 0 )
                , diff_mapCount( 0 )
                , emis_mapCount( 0 )
                , spec_mapCount( 0 )
                , shineness( 0 )
                , TransparencyFactor( 0 ) { }

            MaterialInfo::~MaterialInfo(void)
            {
                ReleaseData( );
            }

            void MaterialInfo::ReleaseData(void)
            {
                name = "";

                type = FBX_DATA::FbxMaterial::Type_None;
                ambitype = FBX_DATA::Material_Eles::Fac_None;
                difftype = FBX_DATA::Material_Eles::Fac_None;
                emistype = FBX_DATA::Material_Eles::Fac_None;
                spectype = FBX_DATA::Material_Eles::Fac_None;

                ambi_mapCount = 0;
                diff_mapCount = 0;
                emis_mapCount = 0;
                spec_mapCount = 0;

                ambi_texNames.clear( );
                diff_texNames.clear( );
                emis_texNames.clear( );
                spec_texNames.clear( );

                shineness = 0.f;
                TransparencyFactor = 0.f;
            }

            void MaterialInfo::Read(resources::ResourceReader &input)
            {
                input.ReadString( name );
                                          
                input.Read( type );
                input.Read( ambitype );
                input.Read( difftype );
                input.Read( emistype );
                input.Read( spectype );
                                          
                input.Read( ambi_mcolor );
                input.Read( diff_mcolor );
                input.Read( emis_mcolor );
                input.Read( spec_mcolor );
                                          
                input.Read( ambi_mapCount );
                input.Read( diff_mapCount );
                input.Read( emis_mapCount );
                input.Read( spec_mapCount );
                 
                unsigned int i = 0;

                ambi_texNames.resize( ambi_mapCount );

                for (i = 0; i < ambi_mapCount; ++i)
                    input.ReadString( ambi_texNames[ i ] );

                diff_texNames.resize( diff_mapCount );

                for (i = 0; i < diff_mapCount; ++i)
                    input.ReadString( diff_texNames[ i ] );

                emis_texNames.resize( emis_mapCount );

                for (i = 0; i < emis_mapCount; ++i)
                    input.ReadString( emis_texNames[ i ] );

                spec_texNames.resize( spec_mapCount );

                for (i = 0; i < spec_mapCount; ++i)
                    input.ReadString( spec_texNames[ i ] );

                input.Read( shineness );
                input.Read( TransparencyFactor );
            }

            void MaterialInfo::Write(resources::pipeline::ResourceWriter &output)
            {
                output.WriteString( name );

                output.Write( type );
                output.Write( ambitype );
                output.Write( difftype );
                output.Write( emistype );
                output.Write( spectype );

                output.Write( ambi_mcolor );
                output.Write( diff_mcolor );
                output.Write( emis_mcolor );
                output.Write( spec_mcolor );

                output.Write( ambi_mapCount );
                output.Write( diff_mapCount );
                output.Write( emis_mapCount );
                output.Write( spec_mapCount );
                           
                if (ambi_texNames.size( ) > 0)
                {
                    for (auto &iter : ambi_texNames)
                        output.WriteString( iter );
                }

                if (diff_texNames.size( ) > 0)
                {
                    for (auto &iter : diff_texNames)
                        output.WriteString( iter );
                }

                if (emis_texNames.size( ) > 0)
                {
                    for (auto &iter : emis_texNames)
                        output.WriteString( iter );
                }

                if (spec_texNames.size( ) > 0)
                {
                    for (auto &iter : spec_texNames)
                        output.WriteString( iter );
                }

                output.Write( shineness );
                output.Write( TransparencyFactor );
            }
        };
    };
};
