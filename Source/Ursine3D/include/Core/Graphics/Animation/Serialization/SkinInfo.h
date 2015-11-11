#pragma once

#include "ISerialize.h"
#include "Utilities.h"

namespace ursine
{
	namespace graphics
	{
		namespace ufmt_loader
		{
			struct SkinBone
			{
				char name[MAXTEXTLEN];
				int mParentIndex;

				// bind - local coord system that the entire skin is defined relative to
				// local tm. local about to the skinned mesh
				XMFLOAT3 bindPosition;
				XMFLOAT4 bindRotation;
				XMFLOAT3 bindScaling;

				// bone space - the space that influences the vertices. so-called offset transformation
				// bone offset tm
				XMFLOAT3 boneSpacePosition;
				XMFLOAT4 boneSpaceRotation;
				XMFLOAT3 boneSpaceScaling;

				SkinBone() : mParentIndex(-1),
					bindPosition(0, 0, 0), bindRotation(0, 0, 0, 1), bindScaling(1, 1, 1),
					boneSpacePosition(0, 0, 0), boneSpaceRotation(0, 0, 0, 1), boneSpaceScaling(1, 1, 1)
				{
				}
			};

			class SkinInfo : public ISerialize
			{
			public:
				char name[MAXTEXTLEN];
				SkinBone mbones;

				SkinInfo();
				virtual ~SkinInfo();

				void ReleaseData();
				virtual bool SerializeIn(HANDLE hFile);
				virtual bool SerializeOut(HANDLE hFile);
			};
		};
	};
};