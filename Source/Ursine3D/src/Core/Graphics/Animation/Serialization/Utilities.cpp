#include "UrsinePrecompiled.h"
#include "Utilities.h"


/*===============================
Utility Functions for FBX
===============================*/
namespace ursine
{
	namespace graphics
	{
		namespace FBX_DATA
		{
			//make sure this gft, interpolate function works in jdl model
			void FbxModel::GetFinalTransform(const std::string& clipName, double timePos, std::vector<XMMATRIX>& finalTransform) const
			{
				// Calculating Final Transform
				size_t numBones = mBoneData.mbonehierarchy.size();
				std::vector<XMMATRIX> toParentTransforms(numBones);
				std::vector<XMMATRIX> toRootTransforms(numBones);
				auto AnimData = mAnimationData[0];
				auto AnimClip = AnimData->animations.find(clipName);

				// try just one animation
				// Interpolate all the bones of this clip at the given time instance.
				AnimClip->second.Interpolate(timePos, toParentTransforms);
				// Traverse the hierarchy and transform all the bones to the root space.
				// The root bone has index 0. The root bone has no parent, so
				// its toRootTransform is just its local bone transform.
				toRootTransforms[0] = toParentTransforms[0];
				// Now find the toRootTransform of the children.
				for (size_t i = 1; i < numBones; ++i)
				{
					// toParent = animTM
					XMMATRIX toParent = toParentTransforms[i];
					int parentIndex = mBoneData.mbonehierarchy[i].mParentIndex;
					// parentToRoot = parent to root transformation
					XMMATRIX parentToRoot = toRootTransforms[parentIndex];
					// toRootTransform = local bone transform
					toRootTransforms[i] = XMMatrixMultiply(toParent, parentToRoot);
				}

				// Premultiply by the bone offset transform to get the final transform.
				for (size_t i = 0; i < numBones; ++i)
				{
					XMVECTOR S = XMLoadFloat3(&mBoneData.mbonehierarchy[i].boneSpaceScaling);
					XMVECTOR P = XMLoadFloat3(&mBoneData.mbonehierarchy[i].boneSpacePosition);
					XMVECTOR Q = XMLoadFloat4(&mBoneData.mbonehierarchy[i].boneSpaceRotation);
					XMVECTOR zero = XMVectorSet(0.f, 0.f, 0.f, 1.0f);
					XMMATRIX offset = XMMatrixAffineTransformation(S, zero, Q, P);
					XMMATRIX toroot = toRootTransforms[i];
					finalTransform[i] = XMMatrixMultiply(offset, toroot);
				}
			}

			void AnimationClip::Interpolate(double timePos, std::vector<XMMATRIX>& toParentTMs)
			{
				for (size_t i = 0; i < boneAnim.size(); ++i)
				{
					// rot * trsnl is the correct order in row major
					BoneAnimation* currBa = &(boneAnim[i]);

					//t is before the animation started, so just return the first key frame.
					if (timePos <= currBa->keyFrames.front().time)
					{
						XMVECTOR S = XMLoadFloat3(&currBa->keyFrames.front().scl);
						XMVECTOR P = XMLoadFloat3(&currBa->keyFrames.front().trans);
						XMVECTOR Q = XMLoadFloat4(&currBa->keyFrames.front().rot);
						XMVECTOR zero = XMVectorSet(0.f, 0.f, 0.f, 1.0f);
						toParentTMs[i] = XMMatrixAffineTransformation(S, zero, Q, P);
					} // t is after the animation ended, so just return the last key frame.
					else if (timePos >= currBa->keyFrames.back().time)
					{
						XMVECTOR S = XMLoadFloat3(&currBa->keyFrames.back().scl);
						XMVECTOR P = XMLoadFloat3(&currBa->keyFrames.back().trans);
						XMVECTOR Q = XMLoadFloat4(&currBa->keyFrames.back().rot);
						XMVECTOR zero = XMVectorSet(0.f, 0.f, 0.f, 1.0f);
						toParentTMs[i] = XMMatrixAffineTransformation(S, zero, Q, P);
					} // t is between two key frames, so interpolate.
					else
					{
						for (unsigned int j = 0; j < currBa->keyFrames.size() - 1; ++j)
						{
							if (timePos >= currBa->keyFrames[j].time && timePos <= currBa->keyFrames[j + 1].time)
							{
								float lerpPercent = ((float)timePos - currBa->keyFrames[j].time)
									/ (currBa->keyFrames[j + 1].time - currBa->keyFrames[j].time);
								XMVECTOR s0 = XMLoadFloat3(&currBa->keyFrames[j].scl);
								XMVECTOR s1 = XMLoadFloat3(&currBa->keyFrames[j + 1].scl);
								XMVECTOR p0 = XMLoadFloat3(&currBa->keyFrames[j].trans);
								XMVECTOR p1 = XMLoadFloat3(&currBa->keyFrames[j + 1].trans);
								XMVECTOR q0 = XMLoadFloat4(&currBa->keyFrames[j].rot);
								XMVECTOR q1 = XMLoadFloat4(&currBa->keyFrames[j + 1].rot);
								XMVECTOR S = XMVectorLerp(s0, s1, lerpPercent);
								XMVECTOR P = XMVectorLerp(p0, p1, lerpPercent);
								XMVECTOR Q = XMQuaternionSlerp(q0, q1, lerpPercent);
								XMVECTOR zero = XMVectorSet(0.f, 0.f, 0.f, 1.0f);
								toParentTMs[i] = XMMatrixAffineTransformation(S, zero, Q, P);
								break;
							}
						}
					}
				}
			}

			bool compare_bw_ascend(BlendIdxWeight lhs, BlendIdxWeight rhs)
			{
				return lhs.mBlendingWeight < rhs.mBlendingWeight;
			}

			bool compare_bw_descend(BlendIdxWeight lhs, BlendIdxWeight rhs)
			{
				return lhs.mBlendingWeight > rhs.mBlendingWeight;
			}
		}

		void Utilities::Swap(void* a, void* b)
		{
			void* tmp = a;
			a = b;
			b = tmp;
		}

		XMVECTOR Utilities::Set4FloatsToXMVector(const float& x, const float& y, const float& z, const float& w)
		{
			XMVECTOR ret;
			ret.m128_f32[0] = x; ret.m128_f32[1] = y; ret.m128_f32[2] = z; ret.m128_f32[3] = w;
			return ret;
		}

		XMVECTOR Utilities::SetFloat3ToXMVector(const XMFLOAT3& rhs)
		{
			XMVECTOR ret;
			ret.m128_f32[0] = rhs.x; ret.m128_f32[1] = rhs.y; ret.m128_f32[2] = rhs.z;
			return ret;
		}

		XMVECTOR Utilities::SetFloat4ToXMVector(const XMFLOAT4& rhs)
		{
			XMVECTOR ret;
			ret.m128_f32[0] = rhs.x; ret.m128_f32[1] = rhs.y; ret.m128_f32[2] = rhs.z; ret.m128_f32[3] = rhs.w;
			return ret;
		}

		XMFLOAT3 Utilities::SetXMVectorToFloat3(const XMVECTOR& rhs)
		{
			XMFLOAT3 ret;
			ret.x = rhs.m128_f32[0];
			ret.y = rhs.m128_f32[1];
			ret.z = rhs.m128_f32[2];
			return ret;
		}

		XMFLOAT4 Utilities::SetXMVectorToFloat4(const XMVECTOR& rhs)
		{
			XMFLOAT4 ret;
			ret.x = rhs.m128_f32[0];
			ret.y = rhs.m128_f32[1];
			ret.z = rhs.m128_f32[2];
			ret.w = rhs.m128_f32[3];
			return ret;
		}

		FbxAMatrix Utilities::FBXMatrixToFBXAMatrix(FbxMatrix* src)
		{
			FbxAMatrix ret;
			ret.mData[0][0] = src->mData[0][0], ret.mData[0][1] = src->mData[0][1], ret.mData[0][2] = src->mData[0][2], ret.mData[0][3] = src->mData[0][3];
			ret.mData[1][0] = src->mData[1][0], ret.mData[1][1] = src->mData[1][1], ret.mData[1][2] = src->mData[1][2], ret.mData[1][3] = src->mData[1][3];
			ret.mData[2][0] = src->mData[2][0], ret.mData[2][1] = src->mData[2][1], ret.mData[2][2] = src->mData[2][2], ret.mData[2][3] = src->mData[2][3];
			ret.mData[3][0] = src->mData[3][0], ret.mData[3][1] = src->mData[3][1], ret.mData[3][2] = src->mData[3][2], ret.mData[3][3] = src->mData[3][3];
			return ret;
		}

		XMMATRIX Utilities::FBXAMatrixToXMMatrix(FbxAMatrix* src)
		{
			return XMMATRIX(
				static_cast<float>(src->mData[0][0]), static_cast<float>(src->mData[0][1]), static_cast<float>(src->mData[0][2]), static_cast<float>(src->mData[0][3]),
				static_cast<float>(src->mData[1][0]), static_cast<float>(src->mData[1][1]), static_cast<float>(src->mData[1][2]), static_cast<float>(src->mData[1][3]),
				static_cast<float>(src->mData[2][0]), static_cast<float>(src->mData[2][1]), static_cast<float>(src->mData[2][2]), static_cast<float>(src->mData[2][3]),
				static_cast<float>(src->mData[3][0]), static_cast<float>(src->mData[3][1]), static_cast<float>(src->mData[3][2]), static_cast<float>(src->mData[3][3]));
		}

		XMVECTOR Utilities::ConvertVector4(const FbxVector4& vec)
		{
			XMVECTOR result;
			result.m128_f32[0] = static_cast<float>(vec.mData[0]);
			result.m128_f32[1] = static_cast<float>(vec.mData[2]);
			result.m128_f32[2] = static_cast<float>(vec.mData[1]);
			result.m128_f32[3] = static_cast<float>(vec.mData[3]);
			return result;
		}

		XMVECTOR Utilities::ConvertQuaternion(const FbxQuaternion& quat)
		{
			XMVECTOR result;
			result.m128_f32[0] = static_cast<float>(quat.mData[0]);
			result.m128_f32[1] = static_cast<float>(quat.mData[2]);
			result.m128_f32[2] = static_cast<float>(quat.mData[1]);
			result.m128_f32[3] = static_cast<float>(quat.mData[3]);
			return result;
		}

		FbxVector4 Utilities::XMVectorToFBXVector(const XMVECTOR& src)
		{
			return FbxVector4(src.m128_f32[0], src.m128_f32[1], src.m128_f32[2], src.m128_f32[3]);
		}

		FbxVector4 Utilities::XMFloat3ToFBXVector4(const XMFLOAT3& src)
		{
			return FbxVector4(src.x, src.y, src.z, 1.0f);
		}

		XMFLOAT4 Utilities::FBXDouble3ToXMFLOAT4(const FbxDouble3& src)
		{
			return XMFLOAT4(static_cast<float>(src[0]), static_cast<float>(src[1]), static_cast<float>(src[2]), 1.f);
		}

		XMFLOAT3 Utilities::FBXVectorToXMFLOAT3(const FbxVector4& src)
		{
			return XMFLOAT3(
				static_cast<float>(src.mData[0]),
				static_cast<float>(src.mData[1]),
				static_cast<float>(src.mData[2]));
		}

		FbxVector2 Utilities::XMFloat2ToFBXVector2(const XMFLOAT2& src)
		{
			return FbxVector2(src.x, src.y);
		}

		XMFLOAT4 Utilities::FBXQuaternionToXMLOAT4(const FbxQuaternion& quat)
		{
			return XMFLOAT4(
				static_cast<float>(quat.mData[0]),
				static_cast<float>(quat.mData[1]),
				static_cast<float>(quat.mData[2]),
				static_cast<float>(quat.mData[3]));
		}

		XMFLOAT4 Utilities::FBXVectorToXMFLOAT4(const FbxVector4& src)
		{
			return XMFLOAT4(
				static_cast<float>(src.mData[0]),
				static_cast<float>(src.mData[1]),
				static_cast<float>(src.mData[2]),
				static_cast<float>(src.mData[3]));
		}

		bool Utilities::HJIsZeroMtx(XMMATRIX* _pMtx)
		{
			unsigned int i = 0, j = 0;
			while (i < 4)
			{
				j = 0;
				while (j < 4)
				{
					if (_pMtx->r[i].m128_f32[j] != 0.0f)
						return false;
					++j;
				}
				++i;
			}
			return true;
		}

		bool Utilities::HJIsSameMtx(XMMATRIX* _pMtx1, XMMATRIX* _pMtx2)
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					if (_pMtx1->r[i].m128_f32[j] != _pMtx2->r[i].m128_f32[j])
						return false;
				}
			}
			return true;
		}

		bool Utilities::HJIsIdentityMtx(XMMATRIX* _pMtx)
		{
			if ((_pMtx->r[0].m128_f32[0] == 1.0f) &&
				(_pMtx->r[1].m128_f32[1] == 1.0f) &&
				(_pMtx->r[2].m128_f32[2] == 1.0f) &&
				(_pMtx->r[3].m128_f32[3] == 1.0f)
				)
				return true;
			return false;
		}

		int Utilities::InterpolationFlagToIndex(int flags)
		{
			if ((flags & FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant) return 1;
			if ((flags & FbxAnimCurveDef::eInterpolationLinear) == FbxAnimCurveDef::eInterpolationLinear) return 2;
			if ((flags & FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic) return 3;
			return 0;
		}

		int Utilities::ConstantmodeFlagToIndex(int flags)
		{
			if ((flags & FbxAnimCurveDef::eConstantStandard) == FbxAnimCurveDef::eConstantStandard) return 1;
			if ((flags & FbxAnimCurveDef::eConstantNext) == FbxAnimCurveDef::eConstantNext) return 2;
			return 0;
		}

		int Utilities::TangentmodeFlagToIndex(int flags)
		{
			if ((flags & FbxAnimCurveDef::eTangentAuto) == FbxAnimCurveDef::eTangentAuto) return 1;
			if ((flags & FbxAnimCurveDef::eTangentAutoBreak) == FbxAnimCurveDef::eTangentAutoBreak) return 2;
			if ((flags & FbxAnimCurveDef::eTangentTCB) == FbxAnimCurveDef::eTangentTCB) return 3;
			if ((flags & FbxAnimCurveDef::eTangentUser) == FbxAnimCurveDef::eTangentUser) return 4;
			if ((flags & FbxAnimCurveDef::eTangentGenericBreak) == FbxAnimCurveDef::eTangentGenericBreak) return 5;
			if ((flags & FbxAnimCurveDef::eTangentBreak) == FbxAnimCurveDef::eTangentBreak) return 6;
			return 0;
		}

		int Utilities::TangentweightFlagToIndex(int flags)
		{
			if ((flags & FbxAnimCurveDef::eWeightedNone) == FbxAnimCurveDef::eWeightedNone) return 1;
			if ((flags & FbxAnimCurveDef::eWeightedRight) == FbxAnimCurveDef::eWeightedRight) return 2;
			if ((flags & FbxAnimCurveDef::eWeightedNextLeft) == FbxAnimCurveDef::eWeightedNextLeft) return 3;
			return 0;
		}

		int Utilities::TangentVelocityFlagToIndex(int flags)
		{
			if ((flags & FbxAnimCurveDef::eVelocityNone) == FbxAnimCurveDef::eVelocityNone) return 1;
			if ((flags & FbxAnimCurveDef::eVelocityRight) == FbxAnimCurveDef::eVelocityRight) return 2;
			if ((flags & FbxAnimCurveDef::eVelocityNextLeft) == FbxAnimCurveDef::eVelocityNextLeft) return 3;
			return 0;
		}
	};
};