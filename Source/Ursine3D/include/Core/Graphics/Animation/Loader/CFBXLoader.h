// *********************************************************************************************************************
/// 
/// @file 		CFBXLoader.h
/// @brief		FBX info loader
/// 
/// @author 	Park Hyung Jun
/// @date 		09/03/2015
/// 
// *********************************************************************************************************************

#pragma once
// For Parsing FBX infos
#include <ModelInfo.h>
#include <LevelInfo.h>
#include <Utilities.h>
#include <Conversion.h>

using namespace Utilities;

namespace ursine
{
	namespace graphics
	{
		class CFBXLoader
		{
		public:
			enum eAXIS_SYSTEM
			{
				eAXIS_DIRECTX = 0,
				eAXIS_OPENGL,
			};

		protected:
			// FBX SDK
			FbxManager*		mSdkManager;
			FbxScene*		mScene;
			FbxImporter*	mImporter;
			Converter*		mConverter;

			void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene);
			void TriangulateRecursive(FbxNode* pNode);

		public:
			CFBXLoader();
			~CFBXLoader();
			void Release();

			bool Export();
			bool CustomFileExport();

			//load fbx file
			bool LoadFBX(const char* filename);
			//process scene
			void ProcessScene(FbxNode* pRoot);
			//process skeleton hierarchy
			void ProcessSkeletonHierarchy(FbxNode* pRoot);
			void ProcessSkeletonHierarchyRecursively(FbxNode* pNode, int inDepth, int myIndex, int inParentIndex);
			//process static mesh
			void ProcessStaticMesh(FbxNode* pNode);
			//process control pts
			void ProcessControlPoint(FbxNode* pNode);
			//process vertices and indices
			void ProcessVertices(FbxMesh* pMesh, FBX_DATA::MeshData* pData);
			void ProcessNormals(FbxMesh* pMesh, FBX_DATA::MeshData* pData);
			void ProcessTangent(FbxMesh* pMesh, FBX_DATA::MeshData* pData);
			//process texture coords(uv)
			void ProcessTexcoord(FbxMesh* pMesh, FBX_DATA::MeshData* pData);
			//process materials
			FbxDouble3 GetMaterialProperty(
				const FbxSurfaceMaterial * pMaterial,
				const char * pPropertyName,
				const char * pFactorPropertyName,
				FBX_DATA::Material_Eles* pElement);
			void ProcessMaterials(FbxNode* pNode, FBX_DATA::MeshData* pData);
			//process blending weight
			void ProcessWeightBlend(FbxNode* pNode, unsigned int& index);
			//sort and remove unnecessary blending weight more than 4
			void RemoveUnnecessaryWeights();
			//find joint by name
			int GetJointIndexByName(const std::string& inJointName);
			//get pose matrix
			FbxAMatrix GetPoseMatrix(FbxPose* pose, int index);
			//for checking negative scales
			bool CheckPositive(FbxVector4 scale);
			bool CheckScaling(FbxVector4 scale);

			//go through all the animation call stack
			void ProcessAnimation();
			//go through all the animation layers
			void ProcessAnimation(FbxAnimStack* animStack, FbxTime start, FbxNode* pNode);
			//displays all the the animation channels
			void ProcessAnimation(FbxAnimLayer* animLayer, FbxTime start, FbxNode* pNode, FBX_DATA::AnimationClip& animData);
			//read the key frame information for this frame
			void ProcessAnimation(FbxAnimCurve* pCurve, std::set<FbxTime>& time);
			//process geometry
			void ProcessGeometry(FbxNode* pNode);
			//reconstruct indices
			void ReconstructIndices(FBX_DATA::MeshData* pData);
			int FindNextMaterialIndex(FBX_DATA::MeshData* pData, unsigned start, unsigned materialId);
			void SwapIndicies(FBX_DATA::MeshData* pData, unsigned indexMaterial0, unsigned indexMaterial1);

			//get local matrix by time
			FbxAMatrix GetLocalMatrixFromTime(FbxNode* pNode, FbxTime keyTime);
			FbxAMatrix GetGlobalDefaultPosition(FbxNode* pNode);
			FbxAMatrix GetGeometryTransformation(FbxNode* pNode);
			FbxAMatrix GetParentTransformation(FbxNode* pParentNode);
			FBX_DATA::MeshData& GetMeshData(int index) const { return (*mModel->mMeshData[index]); }
			size_t GetMeshDataCount() const { return mModel->mMeshData.size(); }
			FBX_DATA::FbxModel& GetModel() const { return *mModel; }
			bool IsSkinned() { return (mModel->mAnimationData.empty()) ? false : true; }
			ufmt_loader::ModelInfo GetModelInfo() const { return *mModelInfo; }
			ufmt_loader::LevelInfo GetLevelInfo() const { return *mLevelInfo; }

		private:
			unsigned int			boneindex;
			std::pair<bool, bool>	mAnimationFlag;	// first flag = if there is animation
													// second flag = if there is only animation
			FbxPose*				mAnimPose;
			FBX_DATA::FbxModel*		mModel;

			ufmt_loader::ModelInfo*	mModelInfo;
			ufmt_loader::LevelInfo*	mLevelInfo;
		};
	}
}	// FBX_LOADER