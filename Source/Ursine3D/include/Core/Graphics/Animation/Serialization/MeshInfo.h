#pragma once

#include "ISerialize.h"
#include <fbxsdk/scene/geometry/fbxlayer.h>
#include "AnimationDef.h"

using namespace pseudodx;

namespace ursine
{
	namespace graphics
	{
		namespace ufmt_loader
		{
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

                pseudodx::XMFLOAT3*		vertices;
				unsigned int*			indices;
				pseudodx::XMFLOAT3*		normals;
				pseudodx::XMFLOAT3*		tangents;
				pseudodx::XMFLOAT2*		uvs;
				char*					mtrlName[MAXTEXTLEN];

				unsigned int*	materialIndices;
				FBX_DATA::ModelSubset* modelSubsets;

				// maybe we don't need this, because we already have full infos for materials and textures
				// maybe materialIndices will be enough
				// maybe we can find correct material and texture from materialInfo by material Indices and name
				//FBX_DATA::FbxMaterial* fbxmaterials;

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