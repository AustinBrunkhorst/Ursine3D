/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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

    /** @brief swap function
    *
    *  this will just swap both a and b(for swaping y,z factor of the vector)
    *
    *  @return nothing
    */
    void Swap(void* a, void* b);
    /** @brief convert XMFLOAT3 to SVec3 function
    *
    *  this will convert XMFLOAT3 format to SVec3
    *
    *  @param rhs XMFLOAT3
    *  @return converted SVec3 result
    */
    ursine::SVec3 SetFloat3ToSVec3(const pseudodx::XMFLOAT3& rhs);
    /** @brief convert XMFLOAT3 to SVec4 function
    *
    *  this will convert XMFLOAT3 format to SVec4
    *
    *  @param rhs XMFLOAT3
    *  @return converted SVec4 result
    */
    ursine::SVec4 SetFloat3ToSVec4(const pseudodx::XMFLOAT3& rhs);
    /** @brief convert XMFLOAT4 to SVe43 function
    *
    *  this will convert XMFLOAT4 format to SVec4
    *
    *  @param rhs XMFLOAT4
    *  @return converted SVec4 result
    */
    ursine::SVec4 SetFloat4ToSVec4(const pseudodx::XMFLOAT4& rhs);
    /** @brief convert SVec3 to XMFLOAT3 function
    *
    *  this will convert SVec3 format to XMFLOAT3
    *
    *  @param rhs SVec3
    *  @return converted XMFLOAT3 result
    */
    pseudodx::XMFLOAT3 SetSVec3ToFloat3(const ursine::SVec3& rhs);
    /** @brief convert SVec4 to XMFLOAT3 function
    *
    *  this will convert SVec4 format to XMFLOAT3
    *
    *  @param rhs SVec4
    *  @return converted XMFLOAT3 result
    */
    pseudodx::XMFLOAT3 SetSVec4ToFloat3(const ursine::SVec4& rhs);
    /** @brief convert SVec4 to XMFLOAT4 function
    *
    *  this will convert SVec4 format to XMFLOAT4
    *
    *  @param rhs SVec4
    *  @return converted XMFLOAT4 result
    */
    pseudodx::XMFLOAT4 SetSVec4rToFloat4(const ursine::SVec4& rhs);
    /** @brief convert FbxMatrix to FbxAMatrix function
    *
    *  this will convert FbxMatrix format to FbxAMatrix
    *
    *  @param src FbxMatrix
    *  @return converted FbxAMatrix result
    */
    FbxAMatrix FBXMatrixToFBXAMatrix(FbxMatrix* src);
    /** @brief convert FbxAMatrix to SMat4 function
    *
    *  this will convert FbxAMatrix format to SMat4
    *
    *  @param src FbxAMatrix
    *  @return converted SMat4 result
    */
    ursine::SMat4 FBXAMatrixToSMat4(FbxAMatrix* src);
    /** @brief convert FbxVector4 to SVec4 function
    *
    *  this will convert FbxVector4 format to SVec4
    *
    *  @param src FbxVector4
    *  @return converted SVec4 result
    */
    ursine::SVec4 ConvertVector4(const FbxVector4& vec);
    /** @brief convert FbxQuaternion to SVec4 function
    *
    *  this will convert FbxQuaternion format to SVec4
    *
    *  @param src FbxQuaternion
    *  @return converted SVec4 result
    */
    ursine::SVec4 ConvertQuaternion(const FbxQuaternion& quat);
    /** @brief convert SVec4 to FbxVector4 function
    *
    *  this will convert SVec4 format to FbxVector4
    *
    *  @param src SVec4
    *  @return converted FbxVector4 result
    */
    FbxVector4 SVec4ToFBXVector(const ursine::SVec4& src);
    /** @brief convert XMFLOAT3 to FbxVector4 function
    *
    *  this will convert XMFLOAT3 format to FbxVector4
    *
    *  @param src XMFLOAT3
    *  @return converted FbxVector4 result
    */
    FbxVector4 XMFloat3ToFBXVector4(const pseudodx::XMFLOAT3& src);
    /** @brief convert FbxDouble3 to FbxVector4 function
    *
    *  this will convert FbxDouble3 format to FbxVector4
    *
    *  @param src FbxDouble3
    *  @return converted FbxVector4 result
    */
    pseudodx::XMFLOAT4 FBXDouble3ToXMFLOAT4(const FbxDouble3& src);
    /** @brief convert FbxVector4 to XMFLOAT3 function
    *
    *  this will convert FbxVector4 format to XMFLOAT3
    *
    *  @param src FbxVector4
    *  @return converted XMFLOAT3 result
    */
    pseudodx::XMFLOAT3 FBXVectorToXMFLOAT3(const FbxVector4& src);
    /** @brief convert XMFLOAT2 to FbxVector2 function
    *
    *  this will convert XMFLOAT2 format to FbxVector2
    *
    *  @param src XMFLOAT2
    *  @return converted FbxVector2 result
    */
    FbxVector2 XMFloat2ToFBXVector2(const pseudodx::XMFLOAT2& src);
    /** @brief convert FbxQuaternion to XMFLOAT4 function
    *
    *  this will convert FbxQuaternion format to XMFLOAT4
    *
    *  @param src FbxQuaternion
    *  @return converted XMFLOAT4 result
    */
    pseudodx::XMFLOAT4 FBXQuaternionToXMLOAT4(const FbxQuaternion& quat);
    /** @brief convert FbxVector4 to XMFLOAT3 function
    *
    *  this will convert FbxVector4 format to XMFLOAT3
    *
    *  @param src FbxVector4
    *  @return converted XMFLOAT3 result
    */
    pseudodx::XMFLOAT3 FBXVectorToXMFLOAT3(const FbxVector4& src);
    /** @brief convert FbxVector4 to XMFLOAT4 function
    *
    *  this will convert FbxVector4 format to XMFLOAT4
    *
    *  @param src FbxVector4
    *  @return converted XMFLOAT4 result
    */
    pseudodx::XMFLOAT4 FBXVectorToXMFLOAT4(const FbxVector4& src);
};