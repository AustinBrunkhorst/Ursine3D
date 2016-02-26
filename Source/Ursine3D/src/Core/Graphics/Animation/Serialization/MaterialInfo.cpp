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
                type(FBX_DATA::FbxMaterial::Type_None),
                ambitype(FBX_DATA::Material_Eles::Fac_None),
                difftype(FBX_DATA::Material_Eles::Fac_None),
                emistype(FBX_DATA::Material_Eles::Fac_None),
                spectype(FBX_DATA::Material_Eles::Fac_None),
                ambi_mcolor(0, 0, 0, 1), diff_mcolor(0, 0, 0, 1),
                emis_mcolor(0, 0, 0, 1), spec_mcolor(0, 0, 0, 1),
                ambi_mapCount(0), diff_mapCount(0), emis_mapCount(0), spec_mapCount(0),
                shineness(0), TransparencyFactor(0)
            {
            }
            
            MaterialInfo::~MaterialInfo()
            {
                ReleaseData();
            }
            
            void MaterialInfo::ReleaseData()
            {
                ambi_texNames.clear();
                diff_texNames.clear();
                emis_texNames.clear();
                spec_texNames.clear();
            }
            
            bool MaterialInfo::SerializeIn(HANDLE hFile)
            {
                DWORD nBytesRead;
                if (hFile != INVALID_HANDLE_VALUE)
                {
                    char tmp_name[MAXTEXTLEN];
                    ReadFile(hFile, tmp_name, sizeof(char) * MAXTEXTLEN, &nBytesRead, nullptr);
                    name = tmp_name;
                    ReadFile(hFile, &type, sizeof(unsigned int), &nBytesRead, nullptr);
                    
                    ReadFile(hFile, &ambitype, sizeof(unsigned int), &nBytesRead, nullptr);
                    ReadFile(hFile, &difftype, sizeof(unsigned int), &nBytesRead, nullptr);
                    ReadFile(hFile, &emistype, sizeof(unsigned int), &nBytesRead, nullptr);
                    ReadFile(hFile, &spectype, sizeof(unsigned int), &nBytesRead, nullptr);
                    
                    ReadFile(hFile, &ambi_mcolor, sizeof(pseudodx::XMFLOAT4), &nBytesRead, nullptr);
                    ReadFile(hFile, &diff_mcolor, sizeof(pseudodx::XMFLOAT4), &nBytesRead, nullptr);
                    ReadFile(hFile, &emis_mcolor, sizeof(pseudodx::XMFLOAT4), &nBytesRead, nullptr);
                    ReadFile(hFile, &spec_mcolor, sizeof(pseudodx::XMFLOAT4), &nBytesRead, nullptr);
                    
                    ReadFile(hFile, &ambi_mapCount, sizeof(unsigned int), &nBytesRead, nullptr);
                    ReadFile(hFile, &diff_mapCount, sizeof(unsigned int), &nBytesRead, nullptr);
                    ReadFile(hFile, &emis_mapCount, sizeof(unsigned int), &nBytesRead, nullptr);
                    ReadFile(hFile, &spec_mapCount, sizeof(unsigned int), &nBytesRead, nullptr);
                    
                    ambi_texNames.resize(ambi_mapCount);
                    diff_texNames.resize(diff_mapCount);
                    emis_texNames.resize(emis_mapCount);
                    spec_texNames.resize(spec_mapCount);
                    for (unsigned int i = 0; i < ambi_mapCount; ++i)
                    {
                        ReadFile(hFile, &tmp_name, sizeof(char) * MAXTEXTLEN, &nBytesRead, nullptr);
                        ambi_texNames[i] = tmp_name;
                    }
                    for (unsigned int i = 0; i < diff_mapCount; ++i)
                    {
                        ReadFile(hFile, &tmp_name[i], sizeof(char) * MAXTEXTLEN, &nBytesRead, nullptr);
                        diff_texNames[i] = tmp_name;
                    }
                    for (unsigned int i = 0; i < emis_mapCount; ++i)
                    {
                        ReadFile(hFile, &tmp_name[i], sizeof(char) * MAXTEXTLEN, &nBytesRead, nullptr);
                        emis_texNames[i] = tmp_name;
                    }
                    for (unsigned int i = 0; i < spec_mapCount; ++i)
                    {
                        ReadFile(hFile, &tmp_name[i], sizeof(char) * MAXTEXTLEN, &nBytesRead, nullptr);
                        spec_texNames[i] = tmp_name;
                    }

                    ReadFile(hFile, &shineness, sizeof(float), &nBytesRead, nullptr);
                    ReadFile(hFile, &TransparencyFactor, sizeof(float), &nBytesRead, nullptr);
                }
                return true;
            }
            
            bool MaterialInfo::SerializeOut(HANDLE hFile)
            {
                DWORD nBytesWrite;
                if (hFile != INVALID_HANDLE_VALUE)
                {
                    char tmp_name[MAXTEXTLEN];
                    lstrcpy(tmp_name, name.c_str());
                    WriteFile(hFile, tmp_name, sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);
                    unsigned int int_type = static_cast<unsigned int>(type);
                    WriteFile(hFile, &int_type, sizeof(unsigned int), &nBytesWrite, nullptr);
                    
                    int_type = static_cast<unsigned int>(ambitype);
                    WriteFile(hFile, &int_type, sizeof(unsigned int), &nBytesWrite, nullptr);
                    int_type = static_cast<unsigned int>(difftype);
                    WriteFile(hFile, &int_type, sizeof(unsigned int), &nBytesWrite, nullptr);
                    int_type = static_cast<unsigned int>(emistype);
                    WriteFile(hFile, &int_type, sizeof(unsigned int), &nBytesWrite, nullptr);
                    int_type = static_cast<unsigned int>(spectype);
                    WriteFile(hFile, &int_type, sizeof(unsigned int), &nBytesWrite, nullptr);
                    
                    WriteFile(hFile, &ambi_mcolor, sizeof(pseudodx::XMFLOAT4), &nBytesWrite, nullptr);
                    WriteFile(hFile, &diff_mcolor, sizeof(pseudodx::XMFLOAT4), &nBytesWrite, nullptr);
                    WriteFile(hFile, &emis_mcolor, sizeof(pseudodx::XMFLOAT4), &nBytesWrite, nullptr);
                    WriteFile(hFile, &spec_mcolor, sizeof(pseudodx::XMFLOAT4), &nBytesWrite, nullptr);
                    
                    WriteFile(hFile, &ambi_mapCount, sizeof(unsigned int), &nBytesWrite, nullptr);
                    WriteFile(hFile, &diff_mapCount, sizeof(unsigned int), &nBytesWrite, nullptr);
                    WriteFile(hFile, &emis_mapCount, sizeof(unsigned int), &nBytesWrite, nullptr);
                    WriteFile(hFile, &spec_mapCount, sizeof(unsigned int), &nBytesWrite, nullptr);
                    
                    if (ambi_texNames.size() > 0)
                    {
                        for (auto iter : ambi_texNames)
                        {
                            lstrcpy(tmp_name, iter.c_str());
                            WriteFile(hFile, &tmp_name, sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);
                        }
                    }
                    if (diff_texNames.size() > 0)
                    {
                        for (auto iter : diff_texNames)
                        {
                            lstrcpy(tmp_name, iter.c_str());
                            WriteFile(hFile, &tmp_name, sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);
                        }
                    }
                    if (emis_texNames.size() > 0)
                    {
                        for (auto iter : emis_texNames)
                        {
                            lstrcpy(tmp_name, iter.c_str());
                            WriteFile(hFile, &tmp_name, sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);
                        }
                    }
                    if (spec_texNames.size() > 0)
                    {
                        for (auto iter : spec_texNames)
                        {
                            lstrcpy(tmp_name, iter.c_str());
                            WriteFile(hFile, &tmp_name, sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);
                        }
                    }

                    WriteFile(hFile, &shineness, sizeof(float), &nBytesWrite, nullptr);
                    WriteFile(hFile, &TransparencyFactor, sizeof(float), &nBytesWrite, nullptr);
                }
                return true;
            }

            void MaterialInfo::SerializeIn(resources::ResourceReader &input)
            {
                unsigned stringSize;
                std::string str;

                input >> stringSize;
                str.resize(stringSize);
                input.ReadBytes(&name[0], stringSize);
                
                input.ReadBytes( reinterpret_cast<char*>( &ambitype ), sizeof( unsigned int) );
                input.ReadBytes( reinterpret_cast<char*>( &difftype ), sizeof( unsigned int) );
                input.ReadBytes( reinterpret_cast<char*>( &emistype ), sizeof( unsigned int) );
                input.ReadBytes( reinterpret_cast<char*>( &spectype ), sizeof( unsigned int) );

                input.ReadBytes(reinterpret_cast<char*>( &ambi_mcolor ), sizeof( pseudodx::XMFLOAT4 ) );
                input.ReadBytes(reinterpret_cast<char*>( &diff_mcolor ), sizeof( pseudodx::XMFLOAT4 ) );
                input.ReadBytes(reinterpret_cast<char*>( &emis_mcolor ), sizeof( pseudodx::XMFLOAT4 ) );
                input.ReadBytes(reinterpret_cast<char*>( &spec_mcolor ), sizeof( pseudodx::XMFLOAT4 ) );

                input >> ambi_mapCount;
                input >> diff_mapCount;
                input >> emis_mapCount;
                input >> spec_mapCount;

                unsigned int i = 0;
                ambi_texNames.resize(ambi_mapCount);
                for (i = 0; i < ambi_mapCount; ++i)
                {
                    input >> stringSize;
                    str.resize(stringSize);
                    input.ReadBytes( &ambi_texNames[ i ][ 0 ], stringSize );
                }

                diff_texNames.resize(diff_mapCount);
                for (i = 0; i < diff_mapCount; ++i)
                {
                    input >> stringSize;
                    str.resize(stringSize);
                    input.ReadBytes( &diff_texNames[ i ][ 0 ], stringSize);
                }

                emis_texNames.resize(emis_mapCount);
                for (i = 0; i < emis_mapCount; ++i)
                {
                    input >> stringSize;
                    str.resize(stringSize);
                    input.ReadBytes( &emis_texNames[ i ][ 0 ], stringSize);
                }

                spec_texNames.resize(spec_mapCount);
                for (i = 0; i < spec_mapCount; ++i)
                {
                    input >> stringSize;
                    str.resize(stringSize);
                    input.ReadBytes( &spec_texNames[ i ][ 0 ], stringSize);
                }

                input >> shineness;
                input >> TransparencyFactor;
            }

            void MaterialInfo::SerializeOut(resources::pipeline::ResourceWriter &output)
            {
                output << name.size();
                output << name;

                output << static_cast<unsigned int>(type); //unsigned int
                output << static_cast<unsigned int>(ambitype);
                output << static_cast<unsigned int>(difftype);
                output << static_cast<unsigned int>(emistype);
                output << static_cast<unsigned int>(spectype);

                output.WriteBytes( reinterpret_cast<char*>( &ambi_mcolor ), sizeof( pseudodx::XMFLOAT4 ) );
                output.WriteBytes( reinterpret_cast<char*>( &diff_mcolor ), sizeof( pseudodx::XMFLOAT4 ) );
                output.WriteBytes( reinterpret_cast<char*>( &emis_mcolor ), sizeof( pseudodx::XMFLOAT4 ) );
                output.WriteBytes( reinterpret_cast<char*>( &spec_mcolor ), sizeof( pseudodx::XMFLOAT4 ) );

                output << ambi_mapCount;
                output << diff_mapCount;
                output << emis_mapCount;
                output << spec_mapCount;
                
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

                output << shineness;
                output << TransparencyFactor;
            }
        };
    };
};