/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Conversion.cpp
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "Conversion.h"

//Compute the final matrix MD that transform the vectors
//this may look strange so here is an explanation
//Remember that matrix math is not commutative (order matters)

// D = Destination Space
// F = Whatever space the FBX file is in
// MD = Matrix in Destination Space
// MF = Matrix in FBX Space
// VD = Vector in Destination Space
// VF = Vector in FBX Space
// F->D = Matrix that converts from FBX to Dest
// D->F = Matrix that converts from Dest to FBX
// V' = Transformed vector

//We want MD but we have MF so to convert

//In vector transformation terms these are all straightforward
//1. VD' = VD * MD
//2. VF = VD * D->F
//3. VF' = VF * MF	
//4. VD' = VF' * F->D

//Expanding
//VD' = VD * MD <-(What we want) From 1
//VD' = VF' * F->D From 4
//VD' = VF * MF * F->D From 3
//VD' = VD * D->F * MF * F->D From 2

//Therefore
//MD = D->F * MF * F->D

Converter::Converter()
{
}

Converter::Converter(FbxScene* pScene)
{
	// Convert Axis System to what is used in this example, if needed
	FbxAxisSystem SceneAxisSystem = pScene->GetGlobalSettings().GetAxisSystem();
	FbxAxisSystem DxTransform(FbxAxisSystem::eDirectX);
	FbxAxisSystem MaxTransform(FbxAxisSystem::eMax);
	FbxAxisSystem MayaTransform(FbxAxisSystem::eMayaYUp);

	//construct the conversion matrix that takes a vector and converts 
	//it from one vector space to the other (invert the Z axis)
	ConversionMatrix.SetIdentity();
	NeedToChangedWinding = false;

	if( SceneAxisSystem == MaxTransform )
	{
		//Logger::Log(MsgStd,"Converting from Max Coordinate System to DirectX\n");
		ConversionMatrix.SetRow( 0 , FbxVector4(1,0,0,0)  );
		ConversionMatrix.SetRow( 1 , FbxVector4(0,0,1,0)  );
		ConversionMatrix.SetRow( 2 , FbxVector4(0,1,0,0)  );
		NeedToChangedWinding = true;
		bMaya = false;
		bMax = true;
	}
	else if( SceneAxisSystem == MayaTransform)
	{	
		//Logger::Log(MsgStd,"Converting from Maya/OpenGL Coordinate System to DirectX\n");
		ConversionMatrix.SetRow( 0 , FbxVector4(1,0,0,0) );
		ConversionMatrix.SetRow( 1 , FbxVector4(0,1,0,0) );
		ConversionMatrix.SetRow( 2 , FbxVector4(0,0,-1,0) );
		NeedToChangedWinding = true;
		bMaya = true;
		bMax = false;
	}
	else
	{
		//Logger::Log( MsgError ,"Unsupported Axis System, no conversion available\n");
	}

	//What we might want to do
	//if( SceneAxisSystem != DxTransform )
	//{
	//	DxTransform.ConvertScene(gScene);
	//}
}


void Converter::ConvertVector(FbxVector4& sourceVec)
{
	FbxVector4& srcVec = *(FbxVector4*)&sourceVec;
	if (Converter::Maya == GetAxisStatus())
		srcVec.mData[2] *= -1;
	else if (Converter::Max == GetAxisStatus())
		srcVec = FbxVector4(srcVec.mData[0], srcVec.mData[2], srcVec.mData[1], srcVec.mData[3]);
}

void Converter::ConvertQuarternion(FbxQuaternion& sourceVec)
{
	FbxVector4& srcVec = *(FbxVector4*)&sourceVec;
	if (Converter::Maya == GetAxisStatus())
		srcVec.mData[2] *= -1;
	else if (Converter::Max == GetAxisStatus())
		srcVec = FbxVector4(srcVec.mData[0], srcVec.mData[2], srcVec.mData[1], srcVec.mData[3]);
}

//Convert a matrix in FBX format (right handed,y up) to DirectX format (left handed, y up)
void Converter::ConvertMatrix(FbxAMatrix& sourceMatX)
{		
	//KFbxMatrix does not have a matrix multiplication
	FbxAMatrix& sourceMat = *(FbxAMatrix*)&sourceMatX;
	sourceMat = ConversionMatrix * sourceMat * ConversionMatrix;
}

//Same as convert expect do not apply the second conversion matrix because we want the vector
//in converted space
void Converter::ConvertMeshMatrix(FbxAMatrix& sourceMatX)
{			
	//KFbxMatrix does not have a matrix multiplication
	FbxAMatrix& sourceMat = *(FbxAMatrix*)&sourceMatX;
	sourceMat = ConversionMatrix * sourceMat;
}

Converter::Axis Converter::GetAxisStatus() const
{
	if (bMaya)
		return Maya;
	else if (bMax)
		return Max;
	else return None;
}


