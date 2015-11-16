#pragma once

#include <string>
#include "UrsinePrecompiled.h"
#include "ModelInfo.h"

namespace ursine
{
	namespace graphics
	{
		namespace ufmt_loader
		{
			ModelInfo::ModelInfo()
				:
				mmeshCount(0), marrMeshes(nullptr),
				mmaterialCount(0), marrMaterials(nullptr),
				mskinCount(0), marrSkins(nullptr),
				manimCount(0), marrAnims(nullptr),
				ISerialize("")
			{
			}

			ModelInfo::~ModelInfo()
			{
			}

			void ModelInfo::ReleaseData()
			{
				if (marrMeshes)
				{
					delete[] marrMeshes;
					marrMeshes = nullptr;
				}
				if (marrMaterials)
				{
					delete[] marrMaterials;
					marrMaterials = nullptr;
				}
				if (marrSkins)
				{
					delete[] marrSkins;
					marrSkins = nullptr;
				}
				if (marrAnims)
				{
					delete[] marrAnims;
					marrAnims = nullptr;
				}
			}

			bool ModelInfo::SerializeIn(HANDLE hFile)
			{
				DWORD nBytesRead;
				unsigned int i = 0;
				if (INVALID_HANDLE_VALUE != hFile)
				{
					ReadFile(hFile, name, sizeof(char) * MAXTEXTLEN, &nBytesRead, nullptr);
					ReadFile(hFile, &mmeshCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &mmaterialCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &mskinCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &manimCount, sizeof(unsigned int), &nBytesRead, nullptr);

					marrMeshes = new MeshInfo[mmeshCount];
					for (i = 0; i < mmeshCount; ++i)
					{
						marrMeshes[i].SerializeIn(hFile);
					}
					marrMaterials = new MaterialInfo[mmaterialCount];
					for (i = 0; i < mmaterialCount; ++i)
					{
						marrMaterials[i].SerializeIn(hFile);
					}
					marrSkins = new SkinInfo[mskinCount];
					for (i = 0; i < mskinCount; ++i)
					{
						marrSkins[i].SerializeIn(hFile);
					}
					marrAnims = new AnimInfo[manimCount];
					for (i = 0; i < manimCount; ++i)
					{
						marrAnims[i].SerializeIn(hFile);
					}
				}
				return true;
			}

			bool ModelInfo::SerializeOut(HANDLE hFile)
			{
				DWORD nBytesWrite;
				unsigned int i = 0;
				if (INVALID_HANDLE_VALUE != hFile)
				{
					WriteFile(hFile, name, sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);
					WriteFile(hFile, &mmeshCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &mmaterialCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &mskinCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &manimCount, sizeof(unsigned int), &nBytesWrite, nullptr);

					for (i = 0; i < mmeshCount; ++i)
					{
						marrMeshes[i].SerializeOut(hFile);
					}
					for (i = 0; i < mmaterialCount; ++i)
					{
						marrMaterials[i].SerializeOut(hFile);
					}
					for (i = 0; i < mskinCount; ++i)
					{
						marrSkins[i].SerializeOut(hFile);
					}
					for (i = 0; i < manimCount; ++i)
					{
						marrAnims[i].SerializeOut(hFile);
					}
				}
				return true;
			}

			void ModelInfo::GetFinalTransform(const std::string& clipName, double timePos, std::vector<XMMATRIX>& finalTransform) const
			{
				// Calculating Final Transform
				size_t numBones = mskinCount;
				std::vector<XMMATRIX> toParentTransforms(numBones);
				std::vector<XMMATRIX> toRootTransforms(numBones);
				if (0 == manimCount)
					return;

				// find animation by name
				int AnimClipIdx = -1;
				auto AnimClip = FindAnimClip(&AnimClipIdx, clipName);
				if (-1 == AnimClipIdx || nullptr == AnimClip)
					return;

				// currently, we can only handle one animation
				// Interpolate all the bones of this clip at the given time instance.
				AnimClip->Interpolate(AnimClipIdx, timePos, toParentTransforms); // need clip index
				//
				// Traverse the hierarchy and transform all the bones to the root space.
				//
				// The root bone has index 0. The root bone has no parent, so
				// its toRootTransform is just its local bone transform.
				toRootTransforms[0] = toParentTransforms[0];
				// Now find the toRootTransform of the children.
				for (size_t i = 1; i < numBones; ++i)
				{
					// toParent = animTM
					XMMATRIX toParent = toParentTransforms[i];
					int parentIndex = marrSkins[i].mbones.mParentIndex;
					// parentToRoot = parent to root transformation
					XMMATRIX parentToRoot = toRootTransforms[parentIndex];
					// toRootTransform = local bone transform
					toRootTransforms[i] = XMMatrixMultiply(toParent, parentToRoot);
				}

				// Premultiply by the bone offset transform to get the final transform.
				for (size_t i = 0; i < numBones; ++i)
				{
					XMVECTOR bsp = Utilities::SetFloat4ToXMVector(marrSkins[i].mbones.boneSpacePosition);
					XMVECTOR bsr = Utilities::SetFloat4ToXMVector(marrSkins[i].mbones.boneSpaceRotation);
					XMMATRIX offPos = XMMatrixTranslationFromVector(bsp);
					XMMATRIX offRot = XMMatrixRotationQuaternion(bsr);
					XMMATRIX offset = XMMatrixMultiply(offRot, offPos);
					XMMATRIX toroot = toRootTransforms[i];
					finalTransform[i] = XMMatrixMultiply(offset, toroot);
				}
			}

			AnimInfo* ModelInfo::FindAnimClip(int* index, const std::string& clipName) const
			{
				for (unsigned int i = 0; i < manimCount; ++i)
				{
					if (clipName == marrAnims[i].name)
					{
						*index = i;
						return &marrAnims[i];
					}
				}
				return nullptr;
			}
		};
	};
};