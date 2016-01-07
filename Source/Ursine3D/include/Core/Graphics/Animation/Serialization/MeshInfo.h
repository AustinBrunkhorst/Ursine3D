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
				unsigned int		ctrlIndices[4];
				pseudodx::XMFLOAT4	ctrlBlendWeights;

				MeshVertex()
				{
					pos = pseudodx::XMFLOAT3(0, 0, 0);
					normal = pseudodx::XMFLOAT3(0, 0, 0);
					tangent = pseudodx::XMFLOAT3(0, 0, 0);
					uv = pseudodx::XMFLOAT2(0, 0);
					ctrlIndices[0] = 0;
					ctrlIndices[1] = 0;
					ctrlIndices[2] = 0;
					ctrlIndices[3] = 0;
					ctrlBlendWeights = pseudodx::XMFLOAT4(0, 0, 0, 0);
				}
			};

			class MeshInfo : public ISerialize
			{
			public:
				char			name[MAXTEXTLEN];
				unsigned int	meshVtxInfoCount;
				char*			mtrlName[MAXTEXTLEN];
				unsigned int	mtrlCount;
				unsigned int	mtrlIndexCount;
				unsigned int	subsetCount;
				SMat4			meshTM;

				MeshVertex*		meshVtxInfos;
				unsigned int*	materialIndices;
				FBX_DATA::ModelSubset* modelSubsets;

				/** @brief mesh information constructor
				*
				*  this will construct mesh information object
				*
				*  @return nothing
				*/
				MeshInfo();
				/** @brief mesh information destructor
				*
				*  this will destroy mesh information object
				*
				*  @return nothing
				*/
				virtual ~MeshInfo();
				/** @brief mesh information release function
				*
				*  this will release memory of the mesh information
				*
				*  @return nothing
				*/
				void ReleaseData();

				/** @brief mesh information serialize in function
				*
				*  this will read mesh information
				*
				*  @param handle of the file
				*  @return if succeed return true, else return false
				*/
				bool SerializeIn(HANDLE hFile);
				/** @brief mesh information serialize out function
				*
				*  this will write mesh information
				*
				*  @param handle of the file
				*  @return if succeed return true, else return false
				*/
				bool SerializeOut(HANDLE hFile);
			};
		};
	};
};