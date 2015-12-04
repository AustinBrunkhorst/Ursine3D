/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Utilities.h
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/
#pragma once

#include <AnimationDef.h>
#include <SVec4.h>

namespace Utilities
{
	/*===============================
	Utility Functions for FBX
	===============================*/
	void Swap(void* a, void* b);
	ursine::SVec3 SetFloat3ToSVec3(const pseudodx::XMFLOAT3& rhs);
	ursine::SVec4 SetFloat3ToXMVector(const pseudodx::XMFLOAT3& rhs);
	ursine::SVec4 SetFloat4ToXMVector(const pseudodx::XMFLOAT4& rhs);
	pseudodx::XMFLOAT3 SetSVec3ToFloat3(const ursine::SVec3& rhs);
	pseudodx::XMFLOAT3 SetXMVectorToFloat3(const ursine::SVec4& rhs);
	pseudodx::XMFLOAT4 SetXMVectorToFloat4(const ursine::SVec4& rhs);
	FbxAMatrix FBXMatrixToFBXAMatrix(FbxMatrix* src);
	ursine::SMat4 FBXAMatrixToXMMatrix(FbxAMatrix* src);
	ursine::SVec4 ConvertVector4(const FbxVector4& vec);
	ursine::SVec4 ConvertQuaternion(const FbxQuaternion& quat);
	FbxVector4 XMVectorToFBXVector(const ursine::SVec4& src);
	FbxVector4 XMFloat3ToFBXVector4(const pseudodx::XMFLOAT3& src);
	pseudodx::XMFLOAT4 FBXDouble3ToXMFLOAT4(const FbxDouble3& src);
	pseudodx::XMFLOAT3 FBXVectorToXMFLOAT3(const FbxVector4& src);
	FbxVector2 XMFloat2ToFBXVector2(const pseudodx::XMFLOAT2& src);
	pseudodx::XMFLOAT4 FBXQuaternionToXMLOAT4(const FbxQuaternion& quat);
	pseudodx::XMFLOAT3 FBXVectorToXMFLOAT3(const FbxVector4& src);
	pseudodx::XMFLOAT4 FBXVectorToXMFLOAT4(const FbxVector4& src);
	int InterpolationFlagToIndex(int flags);
	int ConstantmodeFlagToIndex(int flags);
	int TangentmodeFlagToIndex(int flags);
	int TangentweightFlagToIndex(int flags);
	int TangentVelocityFlagToIndex(int flags);
};