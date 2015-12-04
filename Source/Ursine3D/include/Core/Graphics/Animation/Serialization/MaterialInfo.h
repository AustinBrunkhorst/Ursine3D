/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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

namespace ursine
{
	namespace graphics
	{
		namespace ufmt_loader
		{
			class MaterialInfo : public ISerialize
			{
			public:
				char name[MAXTEXTLEN];
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

				//unsigned int* ambi_texIndices;
				//unsigned int* diff_texIndices;
				//unsigned int* emis_texIndices;
				//unsigned int* spec_texIndices;

				char* ambi_texNames[MAXTEXTLEN];
				char* diff_texNames[MAXTEXTLEN];
				char* emis_texNames[MAXTEXTLEN];
				char* spec_texNames[MAXTEXTLEN];

				float shineness;
				float TransparencyFactor;

				MaterialInfo();
				virtual ~MaterialInfo();
				void ReleaseData();

				bool SerializeIn(HANDLE hFile);
				bool SerializeOut(HANDLE hFile);
			};
		};
	};
};