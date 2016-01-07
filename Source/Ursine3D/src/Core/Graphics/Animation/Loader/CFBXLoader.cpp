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
** - Hiago Desena - hiago.desena@digipen.edu
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include <CFBXLoader.h>

#pragma warning (disable : 4996)

// cut off unneccessary things, extensions to get name
std::string GetName(const char* filename)
{
	std::string ret;
	char fName[128];
	std::strcpy(fName, filename);

	// get pure file name
	char* ptr;
	ptr = strtok(fName, "/");
	while (ptr != nullptr)
	{
		ptr = strtok(nullptr, "/");
		if (nullptr != ptr)
			ret = ptr;
	}

	// get rid of extension
	std::strcpy(fName, ret.c_str());
	ptr = strtok(fName, ".");
	if (nullptr != ptr)
		ret = ptr;

	return ret;
}

void GetMaterialTexuteMapName(std::string& mapName)
{
	char texName[MAXTEXTLEN];
	lstrcpy(texName, mapName.c_str());
	char* ptr;
	ptr = strtok(texName, "\\");
	while (ptr != nullptr)
	{
		ptr = strtok(nullptr, "\\");
		if (nullptr != ptr)
			lstrcpy(texName, ptr);
	}
	mapName = texName;
}

namespace ursine
{
	namespace graphics
	{
		CFBXLoader::CFBXLoader() :
			mSdkManager(nullptr), mScene(nullptr), mModel(nullptr),
			mModelInfo(nullptr), mLevelInfo(nullptr),
			mConverter(nullptr)
		{
		}

		CFBXLoader::~CFBXLoader()
		{
			Release();
		}

		void CFBXLoader::Release()
		{
			// after finished exporting, then release the data
			if (mConverter)
				delete mConverter;

			if (mModelInfo)
				mModelInfo->ReleaseData();

			if (mLevelInfo)
				mLevelInfo->ReleaseData();

			if (mModel)
			{
				delete mModel;
				mModel = nullptr;
			}

			if (mImporter)
			{
				mImporter->Destroy();
				mImporter = nullptr;
			}

			if (mScene)
			{
				mScene->Destroy();
				mScene = nullptr;
			}

			if (mSdkManager)
			{
				mSdkManager->Destroy();
				mSdkManager = nullptr;
			}
		}

		bool CFBXLoader::LoadFBX(const char* filename)
		{
			if (!filename)
				return false;

			InitializeSdkObjects(mSdkManager, mScene);
			if (!mSdkManager)
				return false;

			int lFileFormat = -1;
			if (!mSdkManager->GetIOPluginRegistry()->DetectReaderFileFormat(filename, lFileFormat))
			{
				// Unrecognizable file format. Try to fall back to FbxImporter::eFBX_BINARY
				lFileFormat = mSdkManager->GetIOPluginRegistry()->FindReaderIDByDescription("FBX binary (*.fbx)");;
			}

			// Initialize the importer by providing a filename.
			mImporter = FbxImporter::Create(mSdkManager, "HJImporter");
			if (!mImporter)
				return false;
			if (false == mImporter->Initialize(filename, lFileFormat, mSdkManager->GetIOSettings()))
				return false;
			if (false == mImporter->Import(mScene))
				return false;

			// this will do conversion for me
			FbxGeometryConverter lGConverter(mSdkManager);
			lGConverter.SplitMeshesPerMaterial(mScene, false);
			mConverter = new Converter(mScene);

			// Process Scene
			mModel = new FBX_DATA::FbxModel;
			mModel->name = GetName(filename);
			// Getting Anim Pose
			mModel->mAnimPose.resize(mScene->GetPoseCount());
			for (unsigned i = 0; i < mModel->mAnimPose.size(); ++i)
				mModel->mAnimPose[i] = mScene->GetPose(i);

			ProcessScene(mScene->GetRootNode());

			// Export FBX model as custom file format
			Export();
			CustomFileExport();

			return true;
		}

		void CFBXLoader::InitializeSdkObjects(FbxManager*& pManager, ::FbxScene*& pScene)
		{
			//The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
			pManager = FbxManager::Create();
			if (!pManager)
			{
				FBXSDK_printf("Error: Unable to create FBX Manager!\n");
				exit(1);
			}
			else FBXSDK_printf("Autodesk FBX SDK version %s\n", pManager->GetVersion());

			//Create an IOSettings object. This object holds all import/export settings.
			FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
			pManager->SetIOSettings(ios);

			//Load plugins from the executable directory (optional)
			FbxString lPath = FbxGetApplicationDirectory();
			pManager->LoadPluginsDirectory(lPath.Buffer());

			//Create an FBX scene. This object holds most objects imported/exported from/to files.
			pScene = FbxScene::Create(pManager, "My Scene");
			if (!pScene)
			{
				FBXSDK_printf("Error: Unable to create FBX scene!\n");
				exit(1);
			}
		}

		void CFBXLoader::TriangulateRecursive(FbxNode* pNode)
		{
			FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

			if (lNodeAttribute)
			{
				if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh ||
					lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eNurbs ||
					lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eNurbsSurface ||
					lNodeAttribute->GetAttributeType() == FbxNodeAttribute::ePatch)
				{
					FbxGeometryConverter lConverter(pNode->GetFbxManager());
#if 0
					lConverter.TriangulateInPlace(pNode);
#endif // 0
					lConverter.Triangulate(mScene, true);
				}
			}
			const int lChildCount = pNode->GetChildCount();
			for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
			{
				TriangulateRecursive(pNode->GetChild(lChildIndex));
			}
		}

		// divide this huge function part by part layer
		bool CFBXLoader::Export()
		{
			if (nullptr == mModelInfo)
				mModelInfo = new ufmt_loader::ModelInfo;

			if (nullptr == mLevelInfo)
				mLevelInfo = new ufmt_loader::LevelInfo;

			// unify the loader data structure and exporter data structure someday
			unsigned int i = 0, j = 0, k = 0, l = 0;

			// mesh data
			strcpy(mModelInfo->name, mModel->name.c_str());
			mModelInfo->mmeshCount = static_cast<unsigned int>(mModel->mMeshData.size());
			mModelInfo->marrMeshes = new ufmt_loader::MeshInfo[mModelInfo->mmeshCount];
			for (i = 0; i < mModelInfo->mmeshCount; ++i)
			{
				FBX_DATA::MeshData* currMD = mModel->mMeshData[i];
				ufmt_loader::MeshInfo* currMI = &mModelInfo->marrMeshes[i];
				// name & counter initialization
				std::strcpy(currMI->name, currMD->name.c_str());

				// mesh Transformation
				currMI->meshTM = currMD->meshTM;
				currMI->meshVtxInfoCount = currMD->vertexCnt;
				currMI->meshVtxInfos = new ufmt_loader::MeshVertex[currMI->meshVtxInfoCount];
				for (j = 0; j < currMI->meshVtxInfoCount; ++j)
				{
					currMI->meshVtxInfos[j].pos = currMD->vertices[currMD->indices[j]];

					if (currMD->normalMode == FbxGeometryElement::eByPolygonVertex)
						currMI->meshVtxInfos[j].normal = currMD->normals[j];
					else if (currMD->normalMode == FbxGeometryElement::eByControlPoint)
						currMI->meshVtxInfos[j].normal = currMD->normals[currMD->indices[j]];

					if (currMD->tangentMode == FbxGeometryElement::eByPolygonVertex)
						currMI->meshVtxInfos[j].tangent = currMD->tangents[j];
					else if (currMD->tangentMode == FbxGeometryElement::eByControlPoint)
						currMI->meshVtxInfos[j].tangent = currMD->tangents[currMD->indices[j]];

					currMI->meshVtxInfos[j].uv = currMD->uvs[j];
					currMI->meshVtxInfos[j].uv.y = 1.0f - currMI->meshVtxInfos[j].uv.y;

					// controls - maybe divide this part later if necessary
					if (!mModel->mCtrlPoints.empty())
					{
						// currently, just for using 1st control point vec
						currMI->meshVtxInfos[j].ctrlBlendWeights.x = static_cast<float>(mModel->mCtrlPoints[i]->at(currMD->indices[j])->mBlendingInfo[0].mBlendingWeight);
						currMI->meshVtxInfos[j].ctrlBlendWeights.y = static_cast<float>(mModel->mCtrlPoints[i]->at(currMD->indices[j])->mBlendingInfo[1].mBlendingWeight);
						currMI->meshVtxInfos[j].ctrlBlendWeights.z = static_cast<float>(mModel->mCtrlPoints[i]->at(currMD->indices[j])->mBlendingInfo[2].mBlendingWeight);
						currMI->meshVtxInfos[j].ctrlBlendWeights.w = static_cast<float>(mModel->mCtrlPoints[i]->at(currMD->indices[j])->mBlendingInfo[3].mBlendingWeight);
						currMI->meshVtxInfos[j].ctrlIndices[0] = mModel->mCtrlPoints[i]->at(currMD->indices[j])->mBlendingInfo[0].mBlendingIndex;
						currMI->meshVtxInfos[j].ctrlIndices[1] = mModel->mCtrlPoints[i]->at(currMD->indices[j])->mBlendingInfo[1].mBlendingIndex;
						currMI->meshVtxInfos[j].ctrlIndices[2] = mModel->mCtrlPoints[i]->at(currMD->indices[j])->mBlendingInfo[2].mBlendingIndex;
						currMI->meshVtxInfos[j].ctrlIndices[3] = mModel->mCtrlPoints[i]->at(currMD->indices[j])->mBlendingInfo[3].mBlendingIndex;
					}
				}

				//fbxmaterials
				currMI->mtrlCount = static_cast<unsigned int>(currMD->fbxmaterials.size());
				for (j = 0; j < currMI->mtrlCount; ++j)
				{
					currMI->mtrlName[j] = new char[MAXTEXTLEN];
					lstrcpy(currMI->mtrlName[j], currMD->fbxmaterials[j].name.c_str());
				}

				//this thing and subset stuff will be useful for handling multimaterial model
				currMI->mtrlIndexCount = mModel->mMeshData[i]->mtrlIndexCnt;
				currMI->materialIndices = new unsigned int[currMI->mtrlIndexCount];
				for (j = 0; j < currMI->mtrlIndexCount; ++j)
					currMI->materialIndices[j] = currMD->materialIndices[j];

				///////////////////////////////////////////////////////////////
				// didn't handled subset yet(using more than two materials)
				///////////////////////////////////////////////////////////////
				currMI->subsetCount = static_cast<unsigned int>(mModel->mMeshData[i]->modelSubsets.size());
				for (j = 0; j < currMI->subsetCount; ++j)
					currMI->modelSubsets[j] = mModel->mMeshData[i]->modelSubsets[j];
			}

			// material data
			mModelInfo->mmaterialCount = static_cast<unsigned int>(mModel->mMaterials.size());
			mModelInfo->marrMaterials = new ufmt_loader::MaterialInfo[mModelInfo->mmaterialCount];
			for (i = 0; i < mModelInfo->mmaterialCount; ++i)
			{
				ufmt_loader::MaterialInfo* currMI = &mModelInfo->marrMaterials[i];
				lstrcpy(currMI->name, mModel->mMaterials[i]->name.c_str());
				currMI->ambitype = mModel->mMaterials[i]->ambient.type;
				currMI->ambi_mcolor = mModel->mMaterials[i]->ambient.color;
				currMI->ambi_mapCount = static_cast<unsigned int>(mModel->mMaterials[i]->ambient.textureSetArray.size());
				//ambi
				j = 0;
				for (auto iter1 = mModel->mMaterials[i]->ambient.textureSetArray.begin();
				iter1 != mModel->mMaterials[i]->ambient.textureSetArray.end(); ++iter1, ++j)
				{
					currMI->ambi_texNames[j] = new char[MAXTEXTLEN];
					lstrcpy(mModelInfo->marrMaterials[i].ambi_texNames[j], iter1->second[j].c_str());
				}
				currMI->difftype = mModel->mMaterials[i]->diffuse.type;
				currMI->diff_mcolor = mModel->mMaterials[i]->diffuse.color;
				currMI->diff_mapCount = static_cast<unsigned int>(mModel->mMaterials[i]->diffuse.textureSetArray.size());

				//diff
				j = 0;
				for (auto iter1 = mModel->mMaterials[i]->diffuse.textureSetArray.begin();
				iter1 != mModel->mMaterials[i]->diffuse.textureSetArray.end(); ++iter1, ++j)
				{
					currMI->diff_texNames[j] = new char[MAXTEXTLEN];
					lstrcpy(mModelInfo->marrMaterials[i].diff_texNames[j], iter1->second[j].c_str());
				}
				currMI->emistype = mModel->mMaterials[i]->emissive.type;
				currMI->emis_mcolor = mModel->mMaterials[i]->emissive.color;
				currMI->emis_mapCount = static_cast<unsigned int>(mModel->mMaterials[i]->emissive.textureSetArray.size());

				//emit
				j = 0;
				for (auto iter1 = mModel->mMaterials[i]->emissive.textureSetArray.begin();
				iter1 != mModel->mMaterials[i]->emissive.textureSetArray.end(); ++iter1, ++j)
				{
					currMI->emis_texNames[j] = new char[MAXTEXTLEN];
					lstrcpy(currMI->emis_texNames[j], iter1->second[j].c_str());
				}
				currMI->spectype = mModel->mMaterials[i]->specular.type;
				currMI->spec_mcolor = mModel->mMaterials[i]->specular.color;
				currMI->spec_mapCount = static_cast<unsigned int>(mModel->mMaterials[i]->specular.textureSetArray.size());

				//spec
				j = 0;
				for (auto iter1 = mModel->mMaterials[i]->specular.textureSetArray.begin();
				iter1 != mModel->mMaterials[i]->specular.textureSetArray.end(); ++iter1, ++j)
				{
					currMI->spec_texNames[j] = new char[MAXTEXTLEN];
					lstrcpy(currMI->spec_texNames[j], iter1->second[j].c_str());
				}
				currMI->shineness = mModel->mMaterials[i]->shineness;
				currMI->TransparencyFactor = mModel->mMaterials[i]->TransparencyFactor;
			}

			// bone data
			mModelInfo->mboneCount = static_cast<unsigned int>(mModel->mBoneData.mbonehierarchy.size());
			mModelInfo->marrBones = new ufmt_loader::BoneInfo[mModelInfo->mboneCount];
			for (i = 0; i < mModelInfo->mboneCount; ++i)
			{
				ufmt_loader::BoneInfo* currBI = &mModelInfo->marrBones[i];
				// skin info will use model's name
				lstrcpy(currBI->name, mModel->mBoneData.mbonehierarchy[i].mName.c_str());
				currBI->mParentIndex = mModel->mBoneData.mbonehierarchy[i].mParentIndex;
				currBI->bindPosition = mModel->mBoneData.mbonehierarchy[i].bindPosition;
				currBI->bindRotation = mModel->mBoneData.mbonehierarchy[i].bindRotation;
				currBI->bindScaling = mModel->mBoneData.mbonehierarchy[i].bindScaling;
				currBI->boneSpacePosition = mModel->mBoneData.mbonehierarchy[i].boneSpacePosition;
				currBI->boneSpaceRotation = mModel->mBoneData.mbonehierarchy[i].boneSpaceRotation;
				currBI->boneSpaceScaling = mModel->mBoneData.mbonehierarchy[i].boneSpaceScaling;
			}

			// anim data
			mModelInfo->manimCount = static_cast<unsigned int>(mModel->mAnimationData.size());
			mModelInfo->marrAnims = new ufmt_loader::AnimInfo[mModelInfo->manimCount];
			for (i = 0; i < mModelInfo->manimCount; ++i)
			{
				ufmt_loader::AnimInfo* currAI = &mModelInfo->marrAnims[i];
				// clips count
				currAI->clipCount = static_cast<unsigned int>(mModel->mAnimationData[i]->animations.size());
				currAI->keyIndices = new unsigned int*[currAI->clipCount];
				currAI->keyframes = new FBX_DATA::KeyFrame**[currAI->clipCount];
				j = 0;
				for (auto iter = mModel->mAnimationData[i]->animations.begin();
				iter != mModel->mAnimationData[i]->animations.end();
					++iter, ++j)
				{
					// storing animation name
					std::strcpy(currAI->name, iter->first.c_str());
					// set keycount / keyframes
					currAI->boneCount = static_cast<unsigned int>(iter->second.boneAnim.size());
					currAI->keyIndices[j] = new unsigned int[iter->second.boneAnim.size()];
					currAI->keyframes[j] = new FBX_DATA::KeyFrame*[iter->second.boneAnim.size()];
					unsigned int maxkfCount = 0;
					for (k = 0; k < iter->second.boneAnim.size(); ++k)
					{
						unsigned int kfCount = static_cast<unsigned int>(iter->second.boneAnim[k].keyFrames.size());
						if (maxkfCount < kfCount)
							maxkfCount = kfCount;
					}

					for (k = 0; k < iter->second.boneAnim.size(); ++k)
					{
						unsigned int kfCount = static_cast<unsigned int>(iter->second.boneAnim[k].keyFrames.size());
						currAI->keyIndices[j][k] = maxkfCount;
						currAI->keyframes[j][k] = new FBX_DATA::KeyFrame[maxkfCount];
						for (l = 0; l < maxkfCount; ++l)
						{
							if (l < maxkfCount && l < kfCount)
								currAI->keyframes[j][k][l] = iter->second.boneAnim[k].keyFrames[l];
							else if (l >= kfCount)
								currAI->keyframes[j][k][l] = iter->second.boneAnim[k].keyFrames[kfCount - 1];
						}
					}
				}
			}
			return true;
		}

		bool CFBXLoader::CustomFileExport()
		{
			if (nullptr == mModelInfo || nullptr == mLevelInfo)
				return false;

			// set name for the custom file format and store it into Output folder
			std::string _fileName("Assets/Models/");
			_fileName += mModelInfo->name;
			std::string jdlFile = _fileName;
			std::string jlvlFile = _fileName;
			jdlFile += ".jdl";
			jlvlFile += ".jlvl";

			HANDLE hFile;
			if (mModelInfo)
			{
				hFile = CreateFile(jdlFile.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
				if (!mModelInfo->SerializeOut(hFile))
				{
					MessageBox(nullptr, "Jdl Export Failed!", "", MB_OK);
					return false;
				}
				CloseHandle(hFile);
			}
			if (mLevelInfo)
			{
				hFile = CreateFile(jlvlFile.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
				if (!mLevelInfo->SerializeOut(hFile))
				{
					MessageBox(nullptr, "Jlvl Export Failed!", "", MB_OK);
					return false;
				}
				CloseHandle(hFile);
			}
			return true;
		}

		//========================================================================
		// My FBX Loader public functions+
		//========================================================================
		void CFBXLoader::ProcessScene(FbxNode* pRoot)
		{
			// FBX has 3 types - Static Mesh, Just Animation Data, Skinned Mesh
			// if there is no animation, just static mesh
			// else process animation
			mAnimationFlag.first = mAnimationFlag.second = false;

			// Skeleton 
			ProcessSkeletonHierarchy(pRoot);
			if (mModel->mBoneData.mbonehierarchy.empty())
			{
				// Process Static Mesh
				ProcessStaticMesh(pRoot);
				return;
			}
			else
			{
				// Process Skinned Mesh
				ProcessControlPoint(pRoot);
				mAnimationFlag.first = true;
			}

			// Weight blending for control points
			unsigned int index = 0;
			ProcessWeightBlend(pRoot, index);
			// Animation
			ProcessAnimation();

			RemoveUnnecessaryWeights();
			ProcessGeometry(pRoot);
		}

		void CFBXLoader::ProcessSkeletonHierarchy(FbxNode* pRoot)
		{
			for (int childIndex = 0; childIndex < pRoot->GetChildCount(); ++childIndex)
			{
				ProcessSkeletonHierarchyRecursively(pRoot->GetChild(childIndex), 0, 0, -1);
			}
		}

		void CFBXLoader::ProcessSkeletonHierarchyRecursively(FbxNode* pNode, int inDepth, int myIndex, int inParentIndex)
		{
			FbxNodeAttribute* attr = pNode->GetNodeAttribute();
			bool bBone = false;
			if (attr&& attr->GetAttributeType() && attr->GetAttributeType() == FbxNodeAttribute::eSkeleton)
			{
				FBX_DATA::Joint currJoint;
				currJoint.mParentIndex = inParentIndex;
				currJoint.mName = pNode->GetName();

				int nodeIdx = 0;
				FbxPose* targetFP = nullptr;
				targetFP = GetAnimPoseAndIdx(pNode, nodeIdx);

				FbxAMatrix localMatrix;
				if (-1 == nodeIdx)
					localMatrix.SetIdentity();
				else
					localMatrix = GetPoseMatrix(targetFP, nodeIdx);

				mConverter->ConvertMatrix(localMatrix);
				// Check negative scale
				FbxVector4 scl = localMatrix.GetS();
				if (!CheckScaling(scl) || !CheckPositive(scl))
					localMatrix.SetS(FbxVector4(1, 1, 1));
				FbxAMatrix inverse = localMatrix.Inverse();

				// Set bind position, bind rotation matrices
				currJoint.bindPosition = FBXVectorToXMFLOAT3(localMatrix.GetT());
				Swap(&currJoint.bindPosition.y, &currJoint.bindPosition.z);
				currJoint.bindRotation = FBXQuaternionToXMLOAT4(localMatrix.GetQ());
				Swap(&currJoint.bindRotation.y, &currJoint.bindRotation.z);
				currJoint.bindScaling = FBXVectorToXMFLOAT3(localMatrix.GetS());
				Swap(&currJoint.bindScaling.y, &currJoint.bindScaling.z);

				// Set bone space position, rotation matrices
				currJoint.boneSpacePosition = FBXVectorToXMFLOAT3(inverse.GetT());
				Swap(&currJoint.boneSpacePosition.y, &currJoint.boneSpacePosition.z);
				currJoint.boneSpaceRotation = FBXQuaternionToXMLOAT4(inverse.GetQ());
				Swap(&currJoint.boneSpaceRotation.y, &currJoint.boneSpaceRotation.z);
				currJoint.boneSpaceScaling = FBXVectorToXMFLOAT3(inverse.GetS());
				Swap(&currJoint.boneSpaceScaling.y, &currJoint.boneSpaceScaling.z);

				// skeleton = bone hierarchy
				mModel->mBoneData.mbonehierarchy.push_back(currJoint);
				mModel->mBoneData.mboneNodes.push_back(pNode);
				SMat4 locTM = FBXAMatrixToSMat4(&localMatrix);
				mModel->mBoneData.mboneLocalTM.push_back(locTM);

				bBone = true;
			}
			for (int i = 0; i < pNode->GetChildCount(); ++i)
			{
				if (bBone)
					ProcessSkeletonHierarchyRecursively(pNode->GetChild(i), inDepth + 1, static_cast<int>(mModel->mBoneData.mbonehierarchy.size()), myIndex);
				else
					ProcessSkeletonHierarchyRecursively(pNode->GetChild(i), inDepth + 1, myIndex, inParentIndex);
			}
		}

		void CFBXLoader::ProcessStaticMesh(FbxNode* pNode)
		{
			FbxMesh* mesh = pNode->GetMesh();
			if (mesh)
			{
				FBX_DATA::MeshData* newMesh = new FBX_DATA::MeshData;
				newMesh->name = pNode->GetName();
				newMesh->mLayout = FBX_DATA::STATIC;
				if ("" == newMesh->name)
					newMesh->name = mModel->name;

				FbxAMatrix  meshTransform;
				int nodeIdx = 0;
				FbxPose* targetFP = nullptr;
				targetFP = GetAnimPoseAndIdx(pNode, nodeIdx);
				if (targetFP)
				{
					meshTransform = GetPoseMatrix(targetFP, nodeIdx);
				}
				else
					meshTransform = GetGlobalDefaultPosition(pNode);

				//Meshes have a separate geometry transform that also needs to be applied
				FbxAMatrix geoTransform = GetGeometryTransformation(pNode);
				FbxAMatrix parentTransform = GetParentTransformation(pNode->GetParent());
				newMesh->parentTM = FBXAMatrixToSMat4(&parentTransform);
				meshTransform = parentTransform * meshTransform * geoTransform;
				mConverter->ConvertMatrix(meshTransform);

				// Check negative scale
				FbxVector4 scl = meshTransform.GetS();
				if (!CheckScaling(scl) || !CheckPositive(scl))
					meshTransform.SetS(FbxVector4(1, 1, 1));

				newMesh->meshTM = FBXAMatrixToSMat4(&meshTransform);

				// vertex, normal, tangent, texcoord, material
				ProcessVertices(mesh, newMesh);
				ProcessNormals(mesh, newMesh);
				ProcessTangent(mesh, newMesh);
				ProcessTexcoord(mesh, newMesh);
				ProcessMaterials(pNode, newMesh);
				mModel->mMeshData.push_back(newMesh);

				//go through all the control points(verticies) and multiply by the transformation
				for (unsigned int i = 0; i < newMesh->vertexCnt; ++i)
				{
					SVec3 vtx = SetFloat3ToSVec3(newMesh->vertices[i]);
					SMat4 meshTM = newMesh->meshTM;
					SVec3 result = meshTM.TransformVector(vtx);
					newMesh->vertices[i] = SetSVec3ToFloat3(result);
				}
			}
			for (int i = 0; i < pNode->GetChildCount(); ++i)
				ProcessStaticMesh(pNode->GetChild(i));
		}

		void CFBXLoader::ProcessControlPoint(FbxNode* pNode)
		{
			FbxMesh* mesh = pNode->GetMesh();
			if (mesh)
			{
				FBX_DATA::ControlPoints* control = new FBX_DATA::ControlPoints;
				mModel->mCtrlPoints.push_back(control);

				unsigned int ctrlPointCount = mesh->GetControlPointsCount();
				for (unsigned int i = 0; i < ctrlPointCount; ++i)
				{
					FBX_DATA::CtrlPoint* newCtrlPoint = new FBX_DATA::CtrlPoint;
					FbxVector4 controlPos = mesh->GetControlPointAt(i);
					//mConverter->ConvertVector(controlPos);
					newCtrlPoint->mPosition = FBXVectorToXMFLOAT3(controlPos);
					(*control)[i] = newCtrlPoint;
				}
			}
			for (int i = 0; i < pNode->GetChildCount(); ++i)
				ProcessControlPoint(pNode->GetChild(i));
		}

		void CFBXLoader::ProcessVertices(FbxMesh* pMesh, FBX_DATA::MeshData* pData)
		{
			if (nullptr == pMesh)
				return;
			unsigned int ctrlPtCnt = pMesh->GetControlPointsCount();

			// vertices
			pData->vertexCnt = ctrlPtCnt;
			pData->vertices = new pseudodx::XMFLOAT3[ctrlPtCnt];
			for (unsigned int i = 0; i < ctrlPtCnt; ++i)
			{
				FbxVector4 v = pMesh->GetControlPointAt(i);
				mConverter->ConvertVector(v);
				pData->vertices[i] = FBXVectorToXMFLOAT3(v);
			}

			// indicies
			unsigned int indexCnt = pMesh->GetPolygonVertexCount();
			pData->indexCnt = indexCnt;
			pData->indices = new unsigned int[indexCnt];
			int* indexData = pMesh->GetPolygonVertices();

			for (unsigned int i = 0; i < indexCnt; ++i)
			{
				pData->indices[i] = indexData[i];
				// change index order abc->acb
				// currently, only can control maya setting
				Converter::Axis axis = mConverter->GetAxisStatus();
				if (axis == Converter::Maya)
				{
					if (2 == i % 3)
					{
						unsigned int temp = pData->indices[i - 1];
						pData->indices[i - 1] = pData->indices[i];
						pData->indices[i] = temp;
					}
				}
			}
		}

		void CFBXLoader::ProcessNormals(FbxMesh* pMesh, FBX_DATA::MeshData* pData)
		{
			//get the normal element attribute
			FbxGeometryElementNormal* normalElement = pMesh->GetElementNormal();

			if (normalElement)
			{
				switch (normalElement->GetMappingMode())
				{
				case FbxGeometryElement::eByControlPoint:
				{
					//get the size of our data (triangles)
					unsigned normalCount = pMesh->GetControlPointsCount();
					pData->normalMode = FbxGeometryElement::eByControlPoint;
					pData->normalCnt = normalCount;
					pData->normals = new pseudodx::XMFLOAT3[normalCount];

					for (int lVertexIndex = 0; lVertexIndex < pMesh->GetControlPointsCount(); ++lVertexIndex)
					{
						int lNormalIndex = 0;
						//reference mode
						if (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
							lNormalIndex = lVertexIndex;
						else if (normalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
							lNormalIndex = normalElement->GetIndexArray().GetAt(lVertexIndex);
						FbxVector4 lNormal = normalElement->GetDirectArray().GetAt(lNormalIndex);
						//mConverter->ConvertVector(lNormal);
						pData->normals[lVertexIndex] = FBXVectorToXMFLOAT3(lNormal);
					}
				}
				break;
				case FbxGeometryElement::eByPolygonVertex:
				{
					int lIndexByPolygonVertex = 0;
					//get the size of our data (triangles)
					unsigned normalCount = pMesh->GetPolygonCount() * pMesh->GetPolygonSize(0);
					pData->normalMode = FbxGeometryElement::eByPolygonVertex;
					pData->normalCnt = normalCount;
					pData->normals = new pseudodx::XMFLOAT3[normalCount];

					for (int lPolygonIndex = 0; lPolygonIndex < pMesh->GetPolygonCount(); ++lPolygonIndex)
					{
						int lPolygonSize = pMesh->GetPolygonSize(lPolygonIndex);
						for (int i = 0; i < lPolygonSize; ++i)
						{
							int lNormalIndex = 0;
							//reference mode
							if (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
								lNormalIndex = lIndexByPolygonVertex;
							if (normalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
								lNormalIndex = normalElement->GetIndexArray().GetAt(lIndexByPolygonVertex);
							FbxVector4 lNormal = normalElement->GetDirectArray().GetAt(lNormalIndex);
							//mConverter->ConvertVector(lNormal);
							pData->normals[lIndexByPolygonVertex] = FBXVectorToXMFLOAT3(lNormal);
							++lIndexByPolygonVertex;
						}
					}
				}
				break;
				}
			}
		}

		void CFBXLoader::ProcessTangent(FbxMesh* pMesh, FBX_DATA::MeshData* pData)
		{
			//get the normal element attribute
			FbxGeometryElementTangent* tangentElement = pMesh->GetElementTangent();

			if (tangentElement)
			{
				switch (tangentElement->GetMappingMode())
				{
				case FbxGeometryElement::eByControlPoint:
				{
					unsigned tangentCount = pMesh->GetControlPointsCount();
					pData->tangentMode = FbxGeometryElement::eByControlPoint;
					pData->tangentCnt = tangentCount;
					pData->tangents = new pseudodx::XMFLOAT3[tangentCount];

					for (int lVertexIndex = 0; lVertexIndex < pMesh->GetControlPointsCount(); lVertexIndex++)
					{
						int lTangentIndex = 0;
						if (tangentElement->GetReferenceMode() == FbxGeometryElement::eDirect)
							lTangentIndex = lVertexIndex;
						if (tangentElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
							lTangentIndex = tangentElement->GetIndexArray().GetAt(lVertexIndex);
						FbxVector4 lTangent = tangentElement->GetDirectArray().GetAt(lTangentIndex);
						mConverter->ConvertVector(lTangent);
						pData->tangents[lVertexIndex] = FBXVectorToXMFLOAT3(lTangent.mData);
					}
				}
				break;
				case FbxGeometryElement::eByPolygonVertex:
				{
					int lIndexByPolygonVertex = 0;
					unsigned tangentCount = pMesh->GetPolygonCount() * pMesh->GetPolygonSize(0);
					pData->tangentMode = FbxGeometryElement::eByPolygonVertex;
					pData->tangentCnt = tangentCount;
					pData->tangents = new pseudodx::XMFLOAT3[tangentCount];

					for (int lPolygonIndex = 0; lPolygonIndex < pMesh->GetPolygonCount(); lPolygonIndex++)
					{
						int lPolygonSize = pMesh->GetPolygonSize(lPolygonIndex);
						for (int i = 0; i < lPolygonSize; ++i)
						{
							int lTangentIndex = 0;
							if (tangentElement->GetReferenceMode() == FbxGeometryElement::eDirect)
								lTangentIndex = lIndexByPolygonVertex;
							if (tangentElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
								lTangentIndex = tangentElement->GetIndexArray().GetAt(lIndexByPolygonVertex);
							FbxVector4 lTangent = tangentElement->GetDirectArray().GetAt(lTangentIndex);
							mConverter->ConvertVector(lTangent);
							pData->tangents[lTangentIndex] = FBXVectorToXMFLOAT3(lTangent.mData);
							lIndexByPolygonVertex++;
						}
					}
				}
				break;
				}
			}
		}

		void CFBXLoader::ProcessTexcoord(FbxMesh* pMesh, FBX_DATA::MeshData* pData)
		{
			//get all UV set names
			FbxStringList lUVSetNameList;
			pMesh->GetUVSetNames(lUVSetNameList);
			if (lUVSetNameList.GetCount() < 1)
				return;

			//iterating over all uv sets
			for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); ++lUVSetIndex)
			{
				//get lUVSetIndex-th uv set
				const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
				FbxGeometryElementUV* lUVElement = pMesh->GetElementUV(lUVSetIndex);// lUVSetName);

				if (!lUVElement)
					continue;

				// only support mapping mode eByPolygonVertex and eByControlPoint
				if (lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex
					&&
					lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
					return;

				//index array, where holds the index referenced to the uv data
				const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
				const int lIndexCount = (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;

				switch (lUVElement->GetMappingMode())
				{
				case FbxGeometryElement::eByControlPoint:
				{
					unsigned int uvCount = pMesh->GetControlPointsCount();
					pData->uvCnt = uvCount;
					pData->uvs = new pseudodx::XMFLOAT2[uvCount];
					for (unsigned int lVertexIndex = 0; lVertexIndex < uvCount; ++lVertexIndex)
					{
						int lUVIndex = 0;
						//reference mode
						if (lUVElement->GetReferenceMode() == FbxGeometryElement::eDirect)
							lUVIndex = lVertexIndex;
						else if (lUVElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
							lUVIndex = lUVElement->GetIndexArray().GetAt(lVertexIndex);
						FbxVector2 lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
						pData->uvs[lVertexIndex] = pseudodx::XMFLOAT2(static_cast<float>(lUVValue.mData[0]), static_cast<float>(lUVValue.mData[1]));

						if (2 == lVertexIndex % 3)
						{
							pseudodx::XMFLOAT2 tmp = pData->uvs[lVertexIndex];
							pData->uvs[lVertexIndex] = pData->uvs[lVertexIndex - 1];
							pData->uvs[lVertexIndex - 1] = tmp;
						}
					}
				}
				break;
				case FbxGeometryElement::eByPolygonVertex:
				{
					int lPolyIndexCounter = 0;
					pData->uvCnt = lIndexCount;
					pData->uvs = new pseudodx::XMFLOAT2[pData->uvCnt];
					const int lPolyCount = pMesh->GetPolygonCount();
					for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
					{
						// build the max index array that we need to pass into MakePoly
						const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
						for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
						{
							if (lPolyIndexCounter < lIndexCount)
							{
								FbxVector2 lUVValue;
								//the UV index depends on the reference mode
								int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;
								lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
								pData->uvs[lPolyIndexCounter] = pseudodx::XMFLOAT2(static_cast<float>(lUVValue.mData[0]), static_cast<float>(lUVValue.mData[1]));

								// for converting indicies for texture
								if (2 == lPolyIndexCounter % 3)
								{
									pseudodx::XMFLOAT2 tmp = pData->uvs[lPolyIndexCounter];
									pData->uvs[lPolyIndexCounter] = pData->uvs[lPolyIndexCounter - 1];
									pData->uvs[lPolyIndexCounter - 1] = tmp;
								}
								lPolyIndexCounter++;
							}
						}
					}
				}
				break;
				}
			}
		}

		FbxDouble3 CFBXLoader::GetMaterialProperty(
			const FbxSurfaceMaterial * pMaterial,
			const char * pPropertyName,
			const char * pFactorPropertyName,
			FBX_DATA::Material_Eles* pElement)
		{
			pElement->type = FBX_DATA::Material_Eles::Fac_None;

			FbxDouble3 lResult(0, 0, 0);
			FbxProperty lProperty = pMaterial->FindProperty(pPropertyName);
			FbxProperty lFactorProperty = pMaterial->FindProperty(pFactorPropertyName);
			if (lProperty.IsValid() && lFactorProperty.IsValid())
			{
				lResult = lProperty.Get<FbxDouble3>();
				double lFactor = lFactorProperty.Get<FbxDouble>();
				if (lFactor != 1)
				{
					lResult[0] *= lFactor;
					lResult[1] *= lFactor;
					lResult[2] *= lFactor;
				}
				pElement->type = FBX_DATA::Material_Eles::Fac_Only_Color;
			}

			if (lProperty.IsValid())
			{
				int existTextureCount = 0;
				// Normal Texture
				const int lTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();
				for (int i = 0; i < lTextureCount; i++)
				{
					FbxFileTexture* lFileTexture = lProperty.GetSrcObject<FbxFileTexture>(i);
					if (!lFileTexture)
						continue;

					FbxString uvsetName = lFileTexture->UVSet.Get();
					std::string uvSetString = uvsetName.Buffer();
					std::string fileName = lFileTexture->GetFileName();
					GetMaterialTexuteMapName(fileName);

					pElement->textureSetArray[uvSetString].push_back(fileName);
					existTextureCount++;
				}

				// Layered Texture
				const int lLayeredTextureCount = lProperty.GetSrcObjectCount<FbxLayeredTexture>();
				for (int i = 0; i < lLayeredTextureCount; i++)
				{
					FbxLayeredTexture* lLayeredTexture = lProperty.GetSrcObject<FbxLayeredTexture>(i);
					const int lTextureFileCount = lLayeredTexture->GetSrcObjectCount<FbxFileTexture>();
					for (int j = 0; j < lTextureFileCount; j++)
					{
						FbxFileTexture* lFileTexture = lLayeredTexture->GetSrcObject<FbxFileTexture>(j);
						if (!lFileTexture)
							continue;

						FbxString uvsetName = lFileTexture->UVSet.Get();
						std::string uvSetString = uvsetName.Buffer();
						std::string fileName = lFileTexture->GetFileName();
						GetMaterialTexuteMapName(fileName);

						pElement->textureSetArray[uvSetString].push_back(fileName);
						existTextureCount++;
					}
				}

				if (existTextureCount > 0)
				{
					if (pElement->type == FBX_DATA::Material_Eles::Fac_Only_Color)
						pElement->type = FBX_DATA::Material_Eles::Fac_Both;
					else
						pElement->type = FBX_DATA::Material_Eles::Fac_Only_Texture;
				}
			}

			return lResult;
		}

		void CFBXLoader::ProcessMaterials(FbxNode* pNode, FBX_DATA::MeshData* pData)
		{
			unsigned materialCount = pNode->GetMaterialCount();
			if (materialCount > 0)
			{
				for (unsigned i = 0; i < materialCount; ++i)
				{
					FbxSurfaceMaterial* material = pNode->GetMaterial(i);
					FBX_DATA::FbxMaterial destMat;
					destMat.name = material->GetName();
					if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
					{
						destMat.type = FBX_DATA::FbxMaterial::Type_Lambert;
					}
					else if (material->GetClassId().Is(FbxSurfacePhong::ClassId))
					{
						destMat.type = FBX_DATA::FbxMaterial::Type_Phong;
					}

					// Emis
					const FbxDouble3 lEmissive = GetMaterialProperty(material,
						FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor, &destMat.emissive);
					destMat.emissive.color = FBXDouble3ToXMFLOAT4(lEmissive);

					// Ambi
					const FbxDouble3 lAmbient = GetMaterialProperty(material,
						FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor, &destMat.ambient);
					destMat.ambient.color = FBXDouble3ToXMFLOAT4(lAmbient);

					// Diff
					const FbxDouble3 lDiffuse = GetMaterialProperty(material,
						FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor, &destMat.diffuse);
					destMat.diffuse.color = FBXDouble3ToXMFLOAT4(lDiffuse);

					// Spec
					const FbxDouble3 lSpecular = GetMaterialProperty(material,
						FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor, &destMat.specular);
					destMat.specular.color = FBXDouble3ToXMFLOAT4(lSpecular);

					// Transparency
					FbxProperty lTransparencyFactorProperty = material->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
					if (lTransparencyFactorProperty.IsValid())
					{
						double lTransparencyFactor = lTransparencyFactorProperty.Get<FbxDouble>();
						destMat.TransparencyFactor = static_cast<float>(lTransparencyFactor);
					}

					// Shineness
					FbxProperty lShininessProperty = material->FindProperty(FbxSurfaceMaterial::sShininess);
					if (lShininessProperty.IsValid())
					{
						double lShineness = lShininessProperty.Get<FbxDouble>();
						destMat.shineness = static_cast<float>(lShineness);
					}

					// Store Material 
					// 1:1 type. this is for rendering function here. all meshdata has their own materials
					pData->fbxmaterials.push_back(destMat);
					// 1:many type. this is for exporting function
					// we will use 1:many type only later. At this point, we should modify MaterialConst function
					// so that meshnode can load mtrl and texture by mModel->mMaterials' name
					FBX_DATA::FbxMaterial* newFbxMtrl = new FBX_DATA::FbxMaterial;
					*newFbxMtrl = destMat;
					mModel->mMaterials.push_back(newFbxMtrl);
					// has a risk to store more than two exactly same materials
				}
			}

			//get the material index
			FbxGeometryElementMaterial* materialElement = pNode->GetMesh()->GetElementMaterial();
			FbxLayerElementArrayTemplate<int>& materialIndicies = materialElement->GetIndexArray();
			if (materialElement->GetMappingMode() == FbxGeometryElement::eByPolygon)
			{
				pData->mtrlIndexCnt = materialIndicies.GetCount();
				if (materialIndicies.GetCount() > 0)
				{
					pData->materialIndices = new unsigned[pData->mtrlIndexCnt];
					for (int i = 0; i < materialIndicies.GetCount(); ++i)
						pData->materialIndices[i] = materialIndicies[i];
				}
			}
			else if (materialElement->GetMappingMode() == FbxGeometryElement::eAllSame)
			{
				pData->mtrlIndexCnt = pData->indexCnt / pNode->GetMesh()->GetPolygonSize(0);
				pData->materialIndices = new unsigned[pData->mtrlIndexCnt];
				for (int i = 0; i < static_cast<int>(pData->mtrlIndexCnt); ++i)
					pData->materialIndices[i] = materialIndicies[0];
			}
		}

		void CFBXLoader::ProcessWeightBlend(FbxNode* pNode, unsigned int& index)
		{
			FbxMesh* mesh = pNode->GetMesh();
			// target control point vector
			if (mesh)
			{
				unsigned numOfDeformers = mesh->GetDeformerCount();
				for (unsigned int deformerIndex = 0; deformerIndex < numOfDeformers; ++deformerIndex)
				{
					// Skin
					FbxSkin* skin = reinterpret_cast<FbxSkin*>(mesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
					if (!skin)
						continue;

					// cluster is each mesh fragmented from mesh
					unsigned int numCluster = skin->GetClusterCount();
					for (unsigned int clusterIndex = 0; clusterIndex < numCluster; ++clusterIndex)
					{
						FbxCluster* cluster = skin->GetCluster(clusterIndex);
						if (!cluster->GetLink())
							continue;

						//joint name and index
						FbxNode* pLink = cluster->GetLink();
						int boneIdx = GetJointIndexByName(pLink->GetName());
						double* weights = cluster->GetControlPointWeights();
						int* indicies = cluster->GetControlPointIndices();
						// Associate each joint with the control points it affects
						int ctrlPtIdxCnt = cluster->GetControlPointIndicesCount();

						// control point is vertex of the cluster
						// these control points will be influenced by bone animation
						FBX_DATA::ControlPoints* ctrlPts = mModel->mCtrlPoints[index];
						for (int i = 0; i < ctrlPtIdxCnt; ++i)
						{
							FBX_DATA::BlendIdxWeight currBlendIdxWeight;
							currBlendIdxWeight.mBlendingIndex = boneIdx;
							currBlendIdxWeight.mBlendingWeight = static_cast<float>(weights[i]);
							int ctrlPtIdx = indicies[i];
							(*ctrlPts)[ctrlPtIdx]->mBlendingInfo.push_back(currBlendIdxWeight);
						}
					}
				}
				++index;
			}
			for (int i = 0; i < pNode->GetChildCount(); ++i)
				ProcessWeightBlend(pNode->GetChild(i), index);
		}

		void CFBXLoader::RemoveUnnecessaryWeights()
		{
			for (auto controlIter = mModel->mCtrlPoints.begin(); controlIter != mModel->mCtrlPoints.end(); ++controlIter)
			{
				//Remove or remove BlendWeights if there are more than 4
				for (auto iter = (*controlIter)->begin(); iter != (*controlIter)->end(); ++iter)
				{
					std::sort(iter->second->mBlendingInfo.begin(), iter->second->mBlendingInfo.end(), FBX_DATA::compare_bw_descend);
					while (iter->second->mBlendingInfo.size() > 4)
						iter->second->mBlendingInfo.pop_back();

					//check if we have less than 4 blend weight then we have to add some dummy weights
					// how can I give identity matrix to non-animating mesh? or at least just give it vertex
					FBX_DATA::BlendIdxWeight currBlendWeight;
					while (iter->second->mBlendingInfo.size() < 4)
						iter->second->mBlendingInfo.push_back(currBlendWeight);

					//normalize the weights
					double sum = 0.0f;
					for (int w = 0; w < 4; ++w)
						sum += iter->second->mBlendingInfo[w].mBlendingWeight;
					for (int w = 0; w < 4; ++w)
					{
						if (0.0f == sum)
							sum = 1.0f;
						iter->second->mBlendingInfo[w].mBlendingWeight /= sum;
					}
				}
			}
		}

		int CFBXLoader::GetJointIndexByName(const std::string& inJointName)
		{
			for (unsigned int i = 0; i < mModel->mBoneData.mbonehierarchy.size(); ++i)
			{
				if (mModel->mBoneData.mbonehierarchy[i].mName == inJointName)
				{
					return i;
				}
			}
			return -1;
		}

		FbxAMatrix CFBXLoader::GetPoseMatrix(FbxPose* pose, int index)
		{
			FbxAMatrix lPoseMatrix;
			FbxMatrix lMatrix = pose->GetMatrix(index);
			lPoseMatrix = FBXMatrixToFBXAMatrix(&lMatrix);
			return lPoseMatrix;
		}

		bool CFBXLoader::CheckPositive(FbxVector4 scale)
		{
			if (scale[0] < 0.0f || scale[1] < 0.0f || scale[2] < 0.0f)
				return false;
			return true;
		}

		inline bool IsEqualEpsilon(float A, float B)
		{
			return fabs(A - B) <= 1e-5f;
		}

		bool CFBXLoader::CheckScaling(FbxVector4 scale)
		{
			if (!IsEqualEpsilon((float)scale[0], (float)scale[1]) ||
				!IsEqualEpsilon((float)scale[1], (float)scale[2]) ||
				!IsEqualEpsilon((float)scale[0], (float)scale[2]))
			{
				return false;
			}
			return true;
		}

		void CFBXLoader::ProcessAnimation()
		{
			int count = mScene->GetSrcObjectCount<FbxAnimStack>();
			for (int i = 0; i < count; ++i)
			{
				FbxAnimStack* lAnimStack = mScene->GetSrcObject<FbxAnimStack>(i);
				const char* name = lAnimStack->GetName();
				FbxTakeInfo* takeInfo = mScene->GetTakeInfo(name);
				FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
				FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
				ProcessAnimation(lAnimStack, start, mScene->GetRootNode());
			}
		}

		void CFBXLoader::ProcessAnimation(FbxAnimStack* animStack, FbxTime start, FbxNode* pNode)
		{
			mAnimationFlag.second = true;
			std::set< FbxTime > keyTimes;
			int nbAnimLayers = animStack->GetMemberCount<FbxAnimLayer>();
			FbxString lOutputString;

			FBX_DATA::AnimationData* animationData = new FBX_DATA::AnimationData;
			mModel->mAnimationData.push_back(animationData);

			//create the animation clip name of the animation
			FBX_DATA::AnimationClip& animClip = animationData->animations[animStack->GetName()];

			//resize for each bone in the heirarchy
			animClip.boneAnim.resize(mModel->mBoneData.mbonehierarchy.size());
			boneindex = 0;
			for (int i = 0; i < nbAnimLayers; ++i)
			{
				FbxAnimLayer* lAnimLayer = animStack->GetMember<FbxAnimLayer>(i);
				ProcessAnimation(lAnimLayer, start, pNode, animClip);
			}
		}

		void CFBXLoader::ProcessAnimation(FbxAnimLayer* animLayer, FbxTime start, FbxNode* pNode, FBX_DATA::AnimationClip& animClip)
		{
			FbxString lOutputString;
			FbxNodeAttribute* attr = pNode->GetNodeAttribute();
			if (attr && attr->GetAttributeType() && attr->GetAttributeType() == FbxNodeAttribute::eSkeleton)
			{
				FbxAnimCurve* curve = nullptr;
				std::set<FbxTime> time;
				time.insert(start);

				curve = pNode->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
				ProcessAnimation(curve, time);
				curve = pNode->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
				ProcessAnimation(curve, time);
				curve = pNode->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
				ProcessAnimation(curve, time);

				curve = pNode->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
				ProcessAnimation(curve, time);
				curve = pNode->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
				ProcessAnimation(curve, time);
				curve = pNode->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
				ProcessAnimation(curve, time);

				curve = pNode->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
				ProcessAnimation(curve, time);
				curve = pNode->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
				ProcessAnimation(curve, time);
				curve = pNode->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
				ProcessAnimation(curve, time);

				if (boneindex < animClip.boneAnim.size())
					animClip.boneAnim[boneindex].keyFrames.resize(time.size());
				else
					return;

				int i = 0;
				for (std::set<FbxTime>::iterator iter = time.begin(); iter != time.end(); ++iter, ++i)
				{
					FbxAMatrix localMatrix = GetLocalMatrixFromTime(pNode, *iter);
					mConverter->ConvertMatrix(localMatrix);
					FbxVector4 s = localMatrix.GetS();
					if (!CheckScaling(s) || !CheckPositive(s))
						localMatrix.SetS(FbxVector4(1, 1, 1));

					animClip.boneAnim[boneindex].keyFrames[i].time = static_cast<float>(iter->GetSecondDouble());
					// Scale keyframe translation interpolation by specified scale
					animClip.boneAnim[boneindex].keyFrames[i].trans = FBXVectorToXMFLOAT3(localMatrix.GetT());// *_scale;
					Swap(&animClip.boneAnim[boneindex].keyFrames[i].trans.y, &animClip.boneAnim[boneindex].keyFrames[i].trans.z);
					animClip.boneAnim[boneindex].keyFrames[i].rot = FBXQuaternionToXMLOAT4(localMatrix.GetQ());
					Swap(&animClip.boneAnim[boneindex].keyFrames[i].rot.y, &animClip.boneAnim[boneindex].keyFrames[i].rot.z);
					animClip.boneAnim[boneindex].keyFrames[i].scl = FBXVectorToXMFLOAT3(localMatrix.GetS());
					Swap(&animClip.boneAnim[boneindex].keyFrames[i].scl.y, &animClip.boneAnim[boneindex].keyFrames[i].scl.z);
				}
				time.clear();
				++boneindex;
			}

			//go through all the children nodes and try to read there curves
			for (int j = 0; j < pNode->GetChildCount(); ++j)
				ProcessAnimation(animLayer, start, pNode->GetChild(j), animClip);
		}

		void CFBXLoader::ProcessAnimation(FbxAnimCurve* pCurve, std::set<FbxTime>& time)
		{
			if (!pCurve)
				return;

			FbxTime   lKeyTime;
			FbxString lOutputString;
			int lKeyCount = pCurve->KeyGetCount();
			for (int i = 0; i < lKeyCount; ++i)
			{
				FbxAnimCurveKey key = pCurve->KeyGet(i);
				time.insert(key.GetTime());
			}
		}

		void CFBXLoader::ProcessGeometry(FbxNode* pNode)
		{
			FbxMesh* mesh = pNode->GetMesh();
			if (mesh)
			{
				FBX_DATA::MeshData* newMesh = new FBX_DATA::MeshData;
				newMesh->name = pNode->GetName();
				newMesh->mLayout = FBX_DATA::SKINNED;
				if ("" == newMesh->name)
					newMesh->name = mModel->name;

				int nodeIdx = 0;
				FbxPose* targetFP = nullptr;
				targetFP = GetAnimPoseAndIdx(pNode, nodeIdx);
				FbxAMatrix  meshTransform;
				if (targetFP)
				{
					meshTransform = GetPoseMatrix(targetFP, nodeIdx);
				}
				else
					meshTransform = GetGlobalDefaultPosition(pNode);

				//Meshes have a separate geometry transform that also needs to be applied
				FbxAMatrix geoTransform = GetGeometryTransformation(pNode);
				FbxAMatrix parentTransform = GetParentTransformation(pNode->GetParent());
				newMesh->parentTM = FBXAMatrixToSMat4(&parentTransform);
				//mConverter->ConvertMeshMatrix(meshTransform);
				meshTransform = meshTransform * geoTransform;
				mConverter->ConvertMatrix(meshTransform);

				// Check negative scale
				FbxVector4 scl = meshTransform.GetS();
				if (!CheckScaling(scl) || !CheckPositive(scl))
					meshTransform.SetS(FbxVector4(1, 1, 1));

				newMesh->meshTM = FBXAMatrixToSMat4(&meshTransform);

				ProcessVertices(mesh, newMesh);
				ProcessNormals(mesh, newMesh);
				ProcessTangent(mesh, newMesh);
				ProcessTexcoord(mesh, newMesh);
				ProcessMaterials(pNode, newMesh);
				//
				////go through all the control points(verticies) and multiply by the transformation
				//for (unsigned int i = 0; i < newMesh->vertexCnt; ++i)
				//{
				//	SVec3 vtx = SetFloat3ToSVec3(newMesh->vertices[i]);
				//	SMat4 meshTM = newMesh->meshTM;
				//	SVec3 result = meshTM.TransformVector(vtx);
				//	newMesh->vertices[i] = SetSVec3ToFloat3(result);
				//}

				//go through all the control points(verticies) and multiply by the transformation
				for (unsigned int i = 0; i < newMesh->vertexCnt; ++i)
				{
					//XMVECTOR vtx = SetFloat3ToXMVector(modelMesh->vertices[i]);
					//XMMATRIX meshTM = modelMesh->meshTM;
					//XMVECTOR result = XMVector3Transform(vtx, meshTM);
					//modelMesh->vertices[i] = SetXMVectorToFloat3(result);
					FbxVector4 result;
					result.mData[0] = newMesh->vertices[i].x;
					result.mData[1] = newMesh->vertices[i].y;
					result.mData[2] = newMesh->vertices[i].z;
					result.mData[3] = 0.0f;
					newMesh->vertices[i] = FBXVectorToXMFLOAT3(Transform(meshTransform, result));
				}

				mModel->mMeshData.push_back(newMesh);
			}
			//go through all the child node and grab there geometry information
			int childCnt = pNode->GetChildCount();
			for (int i = 0; i < childCnt; ++i)
				ProcessGeometry(pNode->GetChild(i));
		}

		//getting anim pose
		FbxPose* CFBXLoader::GetAnimPoseAndIdx(FbxNode* pNode, int& index)
		{
			for (auto iter : mModel->mAnimPose)
			{
				//mModel->mAnimPose->Find(pNode->GetName());
				index = iter->Find(pNode);
				if (-1 != index)
					return iter;
			}
			return nullptr;
		}

		void CFBXLoader::ReconstructIndices(FBX_DATA::MeshData* pData)
		{
			FBX_DATA::ModelSubset subset;

			//current index start location for the first model susbet
			unsigned int currentSubsetStart = 0;
			//go through all the materials
			for (unsigned materialId = 0; materialId < pData->fbxmaterials.size(); ++materialId)
			{
				//set the starting location for the model subset
				subset.vertexStart = currentSubsetStart;
				//go through all the material indicies starting the vertex start location of the subset in refrence to the material indicies
				for (unsigned j = currentSubsetStart / 3; j < pData->mtrlIndexCnt; ++j)
				{
					if (materialId < pData->materialIndices[j])
					{
						//find the next one
						int loc = FindNextMaterialIndex(pData, j, materialId);
						if (loc < 0)
						{
							//set the id which is the refrence to the material index in the private data
							subset.id = materialId;
							//set the end location of the model subset (if we are at the last material the vertexEnd is the VertexCount of the Model
							subset.vertexCount =
								((materialId == pData->fbxmaterials.size() - 1) ? pData->vertexCnt : j * 3)
								- subset.vertexStart;

							//add the subset
							pData->modelSubsets.push_back(subset);

							//set new the starting location
							currentSubsetStart = subset.vertexStart + subset.vertexCount;
							break;
						}
						//swap the materialIndicies and the Vertex indicies
						SwapIndicies(pData, j, loc);
					}
				}
			}
		}

		int CFBXLoader::FindNextMaterialIndex(FBX_DATA::MeshData* pData, unsigned start, unsigned materialId)
		{
			for (unsigned i = start; i < pData->mtrlIndexCnt; ++i)
				if (pData->materialIndices[i] == materialId)
					return i;

			return -1;
		}

		void CFBXLoader::SwapIndicies(FBX_DATA::MeshData* pData, unsigned indexMaterial0, unsigned indexMaterial1)
		{
			//get the vertex index start
			unsigned startIndex0 = indexMaterial0 * 3;
			unsigned startIndex1 = indexMaterial1 * 3;

			//first lets swap the vertex indicies
			Utilities::Swap(&pData->indices[1], &pData->indices[2]);
			//swap the normals
			Utilities::Swap(&pData->normals[1], &pData->normals[2]);
			//swap the uv Coords
			Utilities::Swap(&pData->uvs[1], &pData->uvs[2]);
			//now lets swap the material id
			Utilities::Swap(&pData->materialIndices[1], &pData->materialIndices[2]);
		}

		FbxAMatrix CFBXLoader::GetLocalMatrixFromTime(FbxNode* pNode, FbxTime keyTime)
		{
			FbxNode* parent = pNode->GetParent();
			FbxAMatrix localXForm;
			FbxNodeAttribute* attr = pNode->GetNodeAttribute();
			if (attr && attr->GetAttributeType() && attr->GetAttributeType() == FbxNodeAttribute::eSkeleton)
			{
				FbxAMatrix PX = parent->EvaluateGlobalTransform(keyTime);
				FbxAMatrix LX = pNode->EvaluateGlobalTransform(keyTime);
				PX = PX.Inverse();
				localXForm = PX * LX;
			}
			else
			{
				//We want root bones in global space
				localXForm = pNode->EvaluateGlobalTransform(keyTime);
			}

			return localXForm;
		}

		FbxAMatrix CFBXLoader::GetGlobalDefaultPosition(FbxNode* pNode)
		{
			// Stores the local position, global position, and parent's global position
			FbxAMatrix local_position;
			FbxAMatrix global_position;
			FbxAMatrix parent_global_position;

			// Get the translation, rotation, and scaling
			FbxVector4 T, R, S;
			local_position.SetT(pNode->LclTranslation.Get());
			local_position.SetR(pNode->LclRotation.Get());
			local_position.SetS(pNode->LclScaling.Get());

			// If the node has a parent
			if (pNode->GetParent())
			{
				parent_global_position = GetGlobalDefaultPosition(pNode->GetParent());
				global_position = parent_global_position * local_position;
			}
			// Otherwise, we've reached the end of the recursion,
			// so the global position is the local position
			else
			{
				global_position = local_position;
			}

			// Return the global position
			return global_position;
		}

		FbxAMatrix CFBXLoader::GetGeometryTransformation(FbxNode* pNode)
		{
			if (!pNode)
			{
				throw std::exception("Null for mesh geometry");
			}

			const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
			const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
			const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

			return FbxAMatrix(lT, lR, lS);
		}

		FbxAMatrix CFBXLoader::GetParentTransformation(FbxNode* pParentNode)
		{
			int nodeIdx = 0;
			FbxPose* targetFP = nullptr;
			targetFP = GetAnimPoseAndIdx(pParentNode, nodeIdx);
			FbxAMatrix parentTM;
			if (targetFP)
				parentTM = GetPoseMatrix(targetFP, nodeIdx);
			else
				parentTM = GetGlobalDefaultPosition(pParentNode);
			return parentTM;
		}

		FbxVector4 CFBXLoader::Transform(const FbxAMatrix& pAMatrix, const FbxVector4& point)
		{
			FbxMatrix * m = (FbxMatrix*)&pAMatrix;
			return m->MultNormalize(point);
		}
	}
}
