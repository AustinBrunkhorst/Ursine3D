/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Conversion.h
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <fbxsdk.h>

class Converter
{
public:
	enum Axis
	{
		None = -1,
		Maya,
		Max,
	};

	Converter();
	Converter(FbxScene* pScene);
	void ConvertVector(FbxVector4& sourceVec);
	void ConvertQuarternion(FbxQuaternion& sourceVec);
	void ConvertMatrix(FbxAMatrix& sourceMatX);
	void ConvertMeshMatrix(FbxAMatrix& m);
	Axis GetAxisStatus() const;

	FbxAMatrix ConversionMatrix;
	bool bMaya;
	bool bMax;
	bool NeedToChangedWinding;
};
