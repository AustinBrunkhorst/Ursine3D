/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** MaterialInfo.h
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "ISerialize.h"
#include "AnimationDef.h"
#include "ResourceReader.h"
#include "ResourceWriter.h"

namespace ursine
{
    namespace graphics
    {
        namespace ufmt_loader
        {
            class MaterialInfo : public ISerialize
            {
            public:
                std::string name;

                FBX_DATA::FbxMaterial::eMaterial_Type type;
                FBX_DATA::Material_Eles::eMaterial_Fac ambitype;
                FBX_DATA::Material_Eles::eMaterial_Fac difftype;
                FBX_DATA::Material_Eles::eMaterial_Fac emistype;
                FBX_DATA::Material_Eles::eMaterial_Fac spectype;

                pseudodx::XMFLOAT4 ambi_mcolor;
                pseudodx::XMFLOAT4 diff_mcolor;
                pseudodx::XMFLOAT4 emis_mcolor;
                pseudodx::XMFLOAT4 spec_mcolor;

                unsigned int ambi_mapCount;
                unsigned int diff_mapCount;
                unsigned int emis_mapCount;
                unsigned int spec_mapCount;

                std::vector< std::string > ambi_texNames;
                std::vector< std::string > diff_texNames;
                std::vector< std::string > emis_texNames;
                std::vector< std::string > spec_texNames;

                float shineness;
                float TransparencyFactor;

                /** @brief material information constructor
                *
                *  this will construct material information object
                *
                *  @return nothing
                */
                MaterialInfo();

                /** @brief material information destructor
                *
                *  this will destroy material information object
                *
                *  @return nothing
                */
                virtual ~MaterialInfo();

                /** @brief material information release function
                *
                *  this will release memory of the material information
                *
                *  @return nothing
                */
                void ReleaseData();

                virtual void Read(resources::ResourceReader &input);
                virtual void Write(resources::pipeline::ResourceWriter &output);
            };
        };
    };
};