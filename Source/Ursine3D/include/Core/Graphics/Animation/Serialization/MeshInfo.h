#pragma once

#include "ISerialize.h"
#include "Utilities.h"
namespace ursine
{
	namespace graphics
	{
		namespace ufmt_loader
		{
			// add transparency later
			class MeshInfo : public ISerialize
			{
			public:
				char name[MAXTEXTLEN];
				unsigned int vertexCount;
				unsigned int indexCount;
				unsigned int normalCount;
				unsigned int tangentCount;
				unsigned int uvCount;
				unsigned int mtrlIndexCount;
				unsigned int mtrlCount;
				unsigned int ctrlPtCount;
				unsigned int subsetCount;

				FbxLayerElement::EMappingMode normalMode;
				FbxLayerElement::EMappingMode tangentMode;

				XMFLOAT3*		vertices;
				unsigned int*	indices;
				XMFLOAT3*		normals;
				XMFLOAT3*		tangents;
				XMFLOAT2*		uvs;
				unsigned int*	materialIndices;
				FBX_DATA::Material* materials;
				FBX_DATA::ModelSubset* modelSubsets;
				unsigned int**	ctrlIndices;
				double**		ctrlBlendWeights;

				MeshInfo();
				virtual ~MeshInfo();
				void ReleaseData();

				bool SerializeIn(HANDLE hFile);
				bool SerializeOut(HANDLE hFile);
			};
		};
	};
};