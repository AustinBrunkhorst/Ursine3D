///////////////////////////////////////////////////////////////////////////////
//
//	Conversion.h
//	Converts homgeneous transform matrices between coordinate systems.
//
//	Authors: Chris Peters
//	Copyright 2009, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include <WholeInformation.h>

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
