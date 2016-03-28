/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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
    ursine::SVec3 SetFloat3ToSVec3(const pseudodx::XMFLOAT3& rhs)
    {
        return ursine::SVec3(rhs.x, rhs.y, rhs.z);
    }

    ursine::SVec4 SetFloat3ToSVec4(const pseudodx::XMFLOAT3& rhs)
    {
        return ursine::SVec4(rhs.x, rhs.y, rhs.z, 0.0f);
    }

    ursine::SVec4 SetFloat4ToSVec4(const pseudodx::XMFLOAT4& rhs)
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

    pseudodx::XMFLOAT3 SetSVec4ToFloat3(const ursine::SVec4& rhs)
    {
        pseudodx::XMFLOAT3 ret;
        ret.x = rhs.X();
        ret.y = rhs.Y();
        ret.z = rhs.Z();
        return ret;
    }

    pseudodx::XMFLOAT4 SetSVec4rToFloat4(const ursine::SVec4& rhs)
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
        memcpy((double*)&ret.mData, &src->mData, sizeof(src->mData));
        return ret;
    }

    ursine::SMat4 FBXAMatrixToSMat4(FbxAMatrix* src)
    {
        ursine::SMat4 ret;
        ret.Set(static_cast<FLOAT>(src->Get(0, 0)), static_cast<FLOAT>(src->Get(0, 1)), static_cast<FLOAT>(src->Get(0, 2)), static_cast<FLOAT>(src->Get(0, 3)),
            static_cast<FLOAT>(src->Get(1, 0)), static_cast<FLOAT>(src->Get(1, 1)), static_cast<FLOAT>(src->Get(1, 2)), static_cast<FLOAT>(src->Get(1, 3)),
            static_cast<FLOAT>(src->Get(2, 0)), static_cast<FLOAT>(src->Get(2, 1)), static_cast<FLOAT>(src->Get(2, 2)), static_cast<FLOAT>(src->Get(2, 3)),
            static_cast<FLOAT>(src->Get(3, 0)), static_cast<FLOAT>(src->Get(3, 1)), static_cast<FLOAT>(src->Get(3, 2)), static_cast<FLOAT>(src->Get(3, 3)));
        return ret;
    }

    FbxAMatrix SMat4ToFBXAMatrix(ursine::SMat4* src)
    {
        FbxAMatrix ret;
        ret.mData[0][0] = src->GetRow(0).X(); ret.mData[0][1] = src->GetRow(0).Y(); ret.mData[0][2] = src->GetRow(0).Z(); ret.mData[0][3] = src->GetRow(0).W();
        ret.mData[1][0] = src->GetRow(1).X(); ret.mData[1][1] = src->GetRow(1).Y(); ret.mData[1][2] = src->GetRow(1).Z(); ret.mData[1][3] = src->GetRow(1).W();
        ret.mData[2][0] = src->GetRow(2).X(); ret.mData[2][1] = src->GetRow(2).Y(); ret.mData[2][2] = src->GetRow(2).Z(); ret.mData[2][3] = src->GetRow(2).W();
        ret.mData[3][0] = src->GetRow(3).X(); ret.mData[3][1] = src->GetRow(3).Y(); ret.mData[3][2] = src->GetRow(3).Z(); ret.mData[3][3] = src->GetRow(3).W();
        return ret;
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

    FbxVector4 SVec4ToFBXVector(const ursine::SVec4& src)
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
};