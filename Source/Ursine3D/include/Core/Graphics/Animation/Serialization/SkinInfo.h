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
				XMFLOAT4 bindPosition;
				XMFLOAT4 bindRotation;

				// bone space - the space that influences the vertices. so-called offset transformation
				// bone offset tm
				XMFLOAT4 boneSpacePosition;
				XMFLOAT4 boneSpaceRotation;

				SkinBone() : mParentIndex(-1),
					bindPosition(0, 0, 0, 1), bindRotation(0, 0, 0, 1),
					boneSpacePosition(0, 0, 0, 1), boneSpaceRotation(0, 0, 0, 1)
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