#pragma once
#include "ISerialize.h"
#include "Utilities.h"

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
				FbxMaterial::eMaterial_Type type;
				Material_Eles::eMaterial_Fac ambitype;
				Material_Eles::eMaterial_Fac difftype;
				Material_Eles::eMaterial_Fac emistype;
				Material_Eles::eMaterial_Fac spectype;

				XMFLOAT4 ambi_mcolor;
				XMFLOAT4 diff_mcolor;
				XMFLOAT4 emis_mcolor;
				XMFLOAT4 spec_mcolor;

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