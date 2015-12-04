/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Utilities.cpp
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include <Utilities.h>


namespace Utilities
{
	/*===============================
	Utility Functions for FBX
	===============================*/
	void Swap(void* a, void* b)
	{
		void* tmp = a;
		a = b;
		b = tmp;
	}

	ursine::SVec3 SetFloat3ToSVec3(const pseudodx::XMFLOAT3& rhs)
	{
		return ursine::SVec3(rhs.x, rhs.y, rhs.z);
	}

	ursine::SVec4 SetFloat3ToXMVector(const pseudodx::XMFLOAT3& rhs)
	{
		return ursine::SVec4(rhs.x, rhs.y, rhs.z, 0.0f);
	}

	ursine::SVec4 SetFloat4ToXMVector(const pseudodx::XMFLOAT4& rhs)
	{
		return ursine::SVec4(rhs.x, rhs.y, rhs.z, rhs.w);
	}

	pseudodx::XMFLOAT3 SetSVec3ToFloat3(const ursine::SVec3& rhs)
	{
		pseudodx::XMFLOAT3 ret;
		ret.x = rhs.X();
		ret.y = rhs.Y();
		ret.z = rhs.Z();
		return ret;
	}

	pseudodx::XMFLOAT3 SetXMVectorToFloat3(const ursine::SVec4& rhs)
	{
		pseudodx::XMFLOAT3 ret;
		ret.x = rhs.X();
		ret.y = rhs.Y();
		ret.z = rhs.Z();
		return ret;
	}

	pseudodx::XMFLOAT4 SetXMVectorToFloat4(const ursine::SVec4& rhs)
	{
		pseudodx::XMFLOAT4 ret;
		ret.x = rhs.X();
		ret.y = rhs.Y();
		ret.z = rhs.Z();
		ret.w = rhs.W();
		return ret;
	}

	FbxAMatrix FBXMatrixToFBXAMatrix(FbxMatrix* src)
	{
		FbxAMatrix ret;
		ret.mData[0][0] = src->mData[0][0], ret.mData[0][1] = src->mData[0][1], ret.mData[0][2] = src->mData[0][2], ret.mData[0][3] = src->mData[0][3];
		ret.mData[1][0] = src->mData[1][0], ret.mData[1][1] = src->mData[1][1], ret.mData[1][2] = src->mData[1][2], ret.mData[1][3] = src->mData[1][3];
		ret.mData[2][0] = src->mData[2][0], ret.mData[2][1] = src->mData[2][1], ret.mData[2][2] = src->mData[2][2], ret.mData[2][3] = src->mData[2][3];
		ret.mData[3][0] = src->mData[3][0], ret.mData[3][1] = src->mData[3][1], ret.mData[3][2] = src->mData[3][2], ret.mData[3][3] = src->mData[3][3];
		return ret;
	}

	ursine::SMat4 FBXAMatrixToXMMatrix(FbxAMatrix* src)
	{
		return ursine::SMat4(
			static_cast<float>(src->mData[0][0]), static_cast<float>(src->mData[0][1]), static_cast<float>(src->mData[0][2]), static_cast<float>(src->mData[0][3]),
			static_cast<float>(src->mData[1][0]), static_cast<float>(src->mData[1][1]), static_cast<float>(src->mData[1][2]), static_cast<float>(src->mData[1][3]),
			static_cast<float>(src->mData[2][0]), static_cast<float>(src->mData[2][1]), static_cast<float>(src->mData[2][2]), static_cast<float>(src->mData[2][3]),
			static_cast<float>(src->mData[3][0]), static_cast<float>(src->mData[3][1]), static_cast<float>(src->mData[3][2]), static_cast<float>(src->mData[3][3]));
	}

	ursine::SVec4 ConvertVector4(const FbxVector4& vec)
	{
		return ursine::SVec4(static_cast<float>(vec.mData[0]),
			static_cast<float>(vec.mData[2]),
			static_cast<float>(vec.mData[1]),
			static_cast<float>(vec.mData[3]));
	}

	ursine::SVec4 ConvertQuaternion(const FbxQuaternion& quat)
	{
		return ursine::SVec4(static_cast<float>(quat.mData[0]),
			static_cast<float>(quat.mData[2]),
			static_cast<float>(quat.mData[1]),
			static_cast<float>(quat.mData[3]));
	}

	FbxVector4 XMVectorToFBXVector(const ursine::SVec4& src)
	{
		return FbxVector4(src.X(), src.Y(), src.Z(), src.W());
	}

	FbxVector4 XMFloat3ToFBXVector4(const pseudodx::XMFLOAT3& src)
	{
		return FbxVector4(src.x, src.y, src.z, 1.0f);
	}

	pseudodx::XMFLOAT4 FBXDouble3ToXMFLOAT4(const FbxDouble3& src)
	{
		return pseudodx::XMFLOAT4(static_cast<float>(src[0]), static_cast<float>(src[1]), static_cast<float>(src[2]), 1.f);
	}

	pseudodx::XMFLOAT3 FBXVectorToXMFLOAT3(const FbxVector4& src)
	{
		return pseudodx::XMFLOAT3(
			static_cast<float>(src.mData[0]),
			static_cast<float>(src.mData[1]),
			static_cast<float>(src.mData[2]));
	}

	FbxVector2 XMFloat2ToFBXVector2(const pseudodx::XMFLOAT2& src)
	{
		return FbxVector2(src.x, src.y);
	}

	pseudodx::XMFLOAT4 FBXQuaternionToXMLOAT4(const FbxQuaternion& quat)
	{
		return pseudodx::XMFLOAT4(
			static_cast<float>(quat.mData[0]),
			static_cast<float>(quat.mData[1]),
			static_cast<float>(quat.mData[2]),
			static_cast<float>(quat.mData[3]));
	}

	pseudodx::XMFLOAT4 FBXVectorToXMFLOAT4(const FbxVector4& src)
	{
		return pseudodx::XMFLOAT4(
			static_cast<float>(src.mData[0]),
			static_cast<float>(src.mData[1]),
			static_cast<float>(src.mData[2]),
			static_cast<float>(src.mData[3]));
	}

	int InterpolationFlagToIndex(int flags)
	{
		if ((flags & FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant) return 1;
		if ((flags & FbxAnimCurveDef::eInterpolationLinear) == FbxAnimCurveDef::eInterpolationLinear) return 2;
		if ((flags & FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic) return 3;
		return 0;
	}

	int ConstantmodeFlagToIndex(int flags)
	{
		if ((flags & FbxAnimCurveDef::eConstantStandard) == FbxAnimCurveDef::eConstantStandard) return 1;
		if ((flags & FbxAnimCurveDef::eConstantNext) == FbxAnimCurveDef::eConstantNext) return 2;
		return 0;
	}

	int TangentmodeFlagToIndex(int flags)
	{
		if ((flags & FbxAnimCurveDef::eTangentAuto) == FbxAnimCurveDef::eTangentAuto) return 1;
		if ((flags & FbxAnimCurveDef::eTangentAutoBreak) == FbxAnimCurveDef::eTangentAutoBreak) return 2;
		if ((flags & FbxAnimCurveDef::eTangentTCB) == FbxAnimCurveDef::eTangentTCB) return 3;
		if ((flags & FbxAnimCurveDef::eTangentUser) == FbxAnimCurveDef::eTangentUser) return 4;
		if ((flags & FbxAnimCurveDef::eTangentGenericBreak) == FbxAnimCurveDef::eTangentGenericBreak) return 5;
		if ((flags & FbxAnimCurveDef::eTangentBreak) == FbxAnimCurveDef::eTangentBreak) return 6;
		return 0;
	}

	int TangentweightFlagToIndex(int flags)
	{
		if ((flags & FbxAnimCurveDef::eWeightedNone) == FbxAnimCurveDef::eWeightedNone) return 1;
		if ((flags & FbxAnimCurveDef::eWeightedRight) == FbxAnimCurveDef::eWeightedRight) return 2;
		if ((flags & FbxAnimCurveDef::eWeightedNextLeft) == FbxAnimCurveDef::eWeightedNextLeft) return 3;
		return 0;
	}

	int TangentVelocityFlagToIndex(int flags)
	{
		if ((flags & FbxAnimCurveDef::eVelocityNone) == FbxAnimCurveDef::eVelocityNone) return 1;
		if ((flags & FbxAnimCurveDef::eVelocityRight) == FbxAnimCurveDef::eVelocityRight) return 2;
		if ((flags & FbxAnimCurveDef::eVelocityNextLeft) == FbxAnimCurveDef::eVelocityNextLeft) return 3;
		return 0;
	}
};