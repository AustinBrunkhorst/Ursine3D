/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** MeshInfo.h
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
			struct MeshVertex
			{
				pseudodx::XMFLOAT3	pos;
				pseudodx::XMFLOAT3	normal;
				pseudodx::XMFLOAT3	tangent;
				pseudodx::XMFLOAT2	uv;
				pseudodx::XMUINT4	ctrlIndices;
				pseudodx::XMFLOAT4	ctrlBlendWeights;

				MeshVertex()
				{
					pos = pseudodx::XMFLOAT3(0, 0, 0);
					normal = pseudodx::XMFLOAT3(0, 0, 0);
					tangent = pseudodx::XMFLOAT3(0, 0, 0);
					uv = pseudodx::XMFLOAT2(0, 0);
					ctrlIndices = pseudodx::XMUINT4(0, 0, 0, 0);
					ctrlBlendWeights = pseudodx::XMFLOAT4(0, 0, 0, 0);
				}

				bool operator==(const MeshVertex& rhs)
				{
					if ((pos.x != rhs.pos.x) ||
						(pos.y != rhs.pos.y) ||
						(pos.z != rhs.pos.z))
						return false;

					if ((normal.x != rhs.normal.x) ||
						(normal.y != rhs.normal.y) ||
						(normal.z != rhs.normal.z))
						return false;

					if ((tangent.x != rhs.tangent.x) ||
						(tangent.y != rhs.tangent.y) ||
						(tangent.z != rhs.tangent.z))
						return false;

					if ((uv.x != rhs.uv.x) || (uv.y != rhs.uv.y))
						return false;

					if ((ctrlIndices.x != rhs.ctrlIndices.x) ||
						(ctrlIndices.y != rhs.ctrlIndices.y) ||
						(ctrlIndices.z != rhs.ctrlIndices.z) ||
						(ctrlIndices.w != rhs.ctrlIndices.w))
						return false;

					if ((ctrlBlendWeights.x != rhs.ctrlBlendWeights.x) ||
						(ctrlBlendWeights.y != rhs.ctrlBlendWeights.y) ||
						(ctrlBlendWeights.z != rhs.ctrlBlendWeights.z) ||
						(ctrlBlendWeights.w != rhs.ctrlBlendWeights.w))
						return false;

					return true;
				}
			};

			class MeshInfo : public ISerialize
			{
			public:
				char			name[MAXTEXTLEN];
				unsigned int	meshVtxInfoCount;
				unsigned int	meshVtxIdxCount;
				std::vector<std::string> mtrlName;
				unsigned int	mtrlCount;
				unsigned int	mtrlIndexCount;

				std::vector<MeshVertex>		meshVtxInfos;
				std::vector<unsigned int>	meshVtxIndices; // this will be used when we do vtx reconstruction
				std::vector<unsigned int>	materialIndices;

				MeshInfo();
				virtual ~MeshInfo();
				void ReleaseData();

				bool SerializeIn(HANDLE hFile);
				bool SerializeOut(HANDLE hFile);
			};
		};
	};
};