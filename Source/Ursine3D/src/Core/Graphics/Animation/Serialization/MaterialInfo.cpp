/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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
					ReadFile(hFile, name, sizeof(char) * MAXTEXTLEN, &nBytesRead, nullptr);
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
						ReadFile(hFile, &ambi_texNames[i], sizeof(char) * MAXTEXTLEN, &nBytesRead, nullptr);
					for (unsigned int i = 0; i < diff_mapCount; ++i)
						ReadFile(hFile, &diff_texNames[i], sizeof(char) * MAXTEXTLEN, &nBytesRead, nullptr);
					for (unsigned int i = 0; i < emis_mapCount; ++i)
						ReadFile(hFile, &emis_texNames[i], sizeof(char) * MAXTEXTLEN, &nBytesRead, nullptr);
					for (unsigned int i = 0; i < spec_mapCount; ++i)
						ReadFile(hFile, &spec_texNames[i], sizeof(char) * MAXTEXTLEN, &nBytesRead, nullptr);

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
					WriteFile(hFile, name, sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);
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

					for (unsigned int i = 0; i < ambi_mapCount; ++i)
						WriteFile(hFile, &ambi_texNames[i], sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);
					for (unsigned int i = 0; i < diff_mapCount; ++i)
						WriteFile(hFile, &diff_texNames[i], sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);
					for (unsigned int i = 0; i < emis_mapCount; ++i)
						WriteFile(hFile, &emis_texNames[i], sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);
					for (unsigned int i = 0; i < spec_mapCount; ++i)
						WriteFile(hFile, &spec_texNames[i], sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);

					WriteFile(hFile, &shineness, sizeof(float), &nBytesWrite, nullptr);
					WriteFile(hFile, &TransparencyFactor, sizeof(float), &nBytesWrite, nullptr);
				}
				return true;
			}
		};
	};
};