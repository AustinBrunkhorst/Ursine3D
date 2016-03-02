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
            MaterialInfo::MaterialInfo() :
                type(FBX_DATA::FbxMaterial::Type_None)
                , ambitype(FBX_DATA::Material_Eles::Fac_None)
                , difftype(FBX_DATA::Material_Eles::Fac_None)
                , emistype(FBX_DATA::Material_Eles::Fac_None)
                , spectype(FBX_DATA::Material_Eles::Fac_None)
                , ambi_mcolor(0, 0, 0, 1), diff_mcolor(0, 0, 0, 1)
                , emis_mcolor(0, 0, 0, 1), spec_mcolor(0, 0, 0, 1)
                , ambi_mapCount(0)
                , diff_mapCount(0)
                , emis_mapCount(0)
                , spec_mapCount(0)
                , shineness(0)
                , TransparencyFactor(0)
            {
            }

            MaterialInfo::~MaterialInfo()
            {
                ReleaseData();
            }

            void MaterialInfo::ReleaseData()
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

                ambi_texNames.clear();
                diff_texNames.clear();
                emis_texNames.clear();
                spec_texNames.clear();

                shineness = 0.f;
                TransparencyFactor = 0.f;
            }
            
            void MaterialInfo::Read(resources::ResourceReader &input)
            {
                unsigned stringSize;
                
                input >> stringSize;
                name.resize(stringSize);
                input.ReadBytes(&name[0], stringSize);

                input.ReadBytes( reinterpret_cast<char*>(&type)     , sizeof(unsigned int));
                input.ReadBytes( reinterpret_cast<char*>(&ambitype) , sizeof(unsigned int) );
                input.ReadBytes( reinterpret_cast<char*>(&difftype) , sizeof(unsigned int) );
                input.ReadBytes( reinterpret_cast<char*>(&emistype) , sizeof(unsigned int) );
                input.ReadBytes( reinterpret_cast<char*>(&spectype) , sizeof(unsigned int) );
                                 
                input.ReadBytes( reinterpret_cast<char*>(&ambi_mcolor), sizeof(pseudodx::XMFLOAT4) );
                input.ReadBytes( reinterpret_cast<char*>(&diff_mcolor), sizeof(pseudodx::XMFLOAT4) );
                input.ReadBytes( reinterpret_cast<char*>(&emis_mcolor), sizeof(pseudodx::XMFLOAT4) );
                input.ReadBytes( reinterpret_cast<char*>(&spec_mcolor), sizeof(pseudodx::XMFLOAT4) );
                                 
                input.ReadBytes( reinterpret_cast<char*>(&ambi_mapCount), sizeof(unsigned int) );
                input.ReadBytes( reinterpret_cast<char*>(&diff_mapCount), sizeof(unsigned int) );
                input.ReadBytes( reinterpret_cast<char*>(&emis_mapCount), sizeof(unsigned int) );
                input.ReadBytes( reinterpret_cast<char*>(&spec_mapCount), sizeof(unsigned int) );
                
                unsigned int i = 0;
                ambi_texNames.resize(ambi_mapCount);
                for (i = 0; i < ambi_mapCount; ++i)
                {
                    input >> stringSize;
                    ambi_texNames[i].resize(stringSize);
                    input.ReadBytes(&ambi_texNames[i][0], stringSize);
                }
                
                diff_texNames.resize(diff_mapCount);
                for (i = 0; i < diff_mapCount; ++i)
                {
                    input >> stringSize;
                    diff_texNames[i].resize(stringSize);
                    input.ReadBytes(&diff_texNames[i][0], stringSize);
                }
                
                emis_texNames.resize(emis_mapCount);
                for (i = 0; i < emis_mapCount; ++i)
                {
                    input >> stringSize;
                    emis_texNames[i].resize(stringSize);
                    input.ReadBytes(&emis_texNames[i][0], stringSize);
                }
                
                spec_texNames.resize(spec_mapCount);
                for (i = 0; i < spec_mapCount; ++i)
                {
                    input >> stringSize;
                    spec_texNames[i].resize(stringSize);
                    input.ReadBytes(&spec_texNames[i][0], stringSize);
                }
                
                input.ReadBytes( reinterpret_cast<char*>(&shineness), sizeof(unsigned int) );
                input.ReadBytes( reinterpret_cast<char*>(&TransparencyFactor), sizeof(unsigned int) );
            }

            void MaterialInfo::Write(resources::pipeline::ResourceWriter &output)
            {
                output << name.size();
                output << name;
                
                output.WriteBytes( reinterpret_cast<char*>(&type)       , sizeof(unsigned int) );
                output.WriteBytes( reinterpret_cast<char*>(&ambitype)   , sizeof(unsigned int) );
                output.WriteBytes( reinterpret_cast<char*>(&difftype)   , sizeof(unsigned int) );
                output.WriteBytes( reinterpret_cast<char*>(&emistype)   , sizeof(unsigned int) );
                output.WriteBytes( reinterpret_cast<char*>(&spectype)   , sizeof(unsigned int) );
                
                output.WriteBytes( reinterpret_cast<char*>(&ambi_mcolor), sizeof(pseudodx::XMFLOAT4));
                output.WriteBytes( reinterpret_cast<char*>(&diff_mcolor), sizeof(pseudodx::XMFLOAT4));
                output.WriteBytes( reinterpret_cast<char*>(&emis_mcolor), sizeof(pseudodx::XMFLOAT4));
                output.WriteBytes( reinterpret_cast<char*>(&spec_mcolor), sizeof(pseudodx::XMFLOAT4));
                
                output.WriteBytes( reinterpret_cast<char*>(&ambi_mapCount), sizeof(unsigned int));
                output.WriteBytes( reinterpret_cast<char*>(&diff_mapCount), sizeof(unsigned int));
                output.WriteBytes( reinterpret_cast<char*>(&emis_mapCount), sizeof(unsigned int));
                output.WriteBytes( reinterpret_cast<char*>(&spec_mapCount), sizeof(unsigned int));
                
                if (ambi_texNames.size() > 0)
                {
                    for (auto &iter : ambi_texNames)
                    {
                        output << iter.size();
                        output << iter;
                    }
                }
                
                if (diff_texNames.size() > 0)
                {
                    for (auto &iter : diff_texNames)
                    {
                        output << iter.size();
                        output << iter;
                    }
                }
                
                if (emis_texNames.size() > 0)
                {
                    for (auto &iter : emis_texNames)
                    {
                        output << iter.size();
                        output << iter;
                    }
                }
                
                if (spec_texNames.size() > 0)
                {
                    for (auto &iter : spec_texNames)
                    {
                        output << iter.size();
                        output << iter;
                    }
                }
                
                output.WriteBytes( reinterpret_cast<char*>(&shineness), sizeof(float) );
                output.WriteBytes( reinterpret_cast<char*>(&TransparencyFactor), sizeof(float) );
            }
        };
    };
};