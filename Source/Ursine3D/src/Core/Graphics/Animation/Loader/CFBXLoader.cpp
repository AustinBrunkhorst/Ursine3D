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
#include <FileSystem.h>

#pragma warning (disable : 4996)
#pragma warning (disable : 4244)
#pragma warning (disable : 4267)

namespace ursine
{
	namespace graphics
	{
		CFBXLoader::CFBXLoader() :
			mSdkManager(nullptr), mScene(nullptr), mModel(nullptr),
			mModelInfo(nullptr), mAnimInfo(nullptr),
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

			if (mAnimInfo)
			{
				mAnimInfo->ReleaseData();
				delete mAnimInfo;
				mAnimInfo = nullptr;
			}

			if (mModelInfo)
			{
				mModelInfo->ReleaseData();
				delete mModelInfo;
				mModelInfo = nullptr;
			}

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
			fs::path fName(filename);
			mModel->name = fName.filename().string();
			mModel->name = mModel->name.substr(0, mModel->name.rfind("."));
			// Getting Anim Pose
			mModel->mAnimPose.resize(mScene->GetPoseCount());
			for (unsigned i = 0; i < mModel->mAnimPose.size(); ++i)
				mModel->mAnimPose[i] = mScene->GetPose(i);

			ProcessScene(mScene->GetRootNode());

			// Export FBX model as custom file format
			ReadyToExport();
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
		bool CFBXLoader::ReadyToExport()
		{
			// unify the loader data structure and exporter data structure someday
			unsigned int i = 0, j = 0, k = 0, l = 0;

			///////////////////////////////////////////////////////////////
			// Model Info
			///////////////////////////////////////////////////////////////
			// mesh data
			if (nullptr == mModelInfo)
			{
				mModelInfo = new ufmt_loader::ModelInfo;
				mModelInfo->name = mModel->name.c_str();
				mModelInfo->mmeshCount = static_cast<unsigned int>(mModel->mMeshData.size());
				for (i = 0; i < mModelInfo->mmeshCount; ++i)
				{
					ufmt_loader::MeshInfo newMeshInfo;
					FBX_DATA::MeshData* currMD = mModel->mMeshData[i];
					// name & counter initialization
					newMeshInfo.name = currMD->name.c_str();

					// Reconstruct Vertices and Indices for data compression
					std::vector<ufmt_loader::MeshVertex> rmvVec;
					std::vector<unsigned int> rIVec;
					Reconstruct(i, rmvVec, rIVec, *currMD);

					newMeshInfo.meshVtxInfoCount = rmvVec.size();
					newMeshInfo.meshVtxIdxCount = rIVec.size();
					for (j = 0; j < newMeshInfo.meshVtxInfoCount; ++j)
						newMeshInfo.meshVtxInfos.push_back(rmvVec[j]);
					for (j = 0; j < newMeshInfo.meshVtxIdxCount; ++j)
						newMeshInfo.meshVtxIndices.push_back(rIVec[j]);

					// push back into the vector
					mModelInfo->mMeshInfoVec.push_back(newMeshInfo);
				}

				// material data
				mModelInfo->mmaterialCount = static_cast<unsigned int>(mModel->mMaterials.size());
				ufmt_loader::MaterialInfo newMtrlInfo;
				for (i = 0; i < mModelInfo->mmaterialCount; ++i)
				{
					//ambi
					j = 0;

					newMtrlInfo.name = mModel->mMaterials[i]->name;

					newMtrlInfo.ambitype = mModel->mMaterials[i]->ambient.type;
					newMtrlInfo.ambi_mcolor = mModel->mMaterials[i]->ambient.color;
					newMtrlInfo.ambi_mapCount = mModel->mMaterials[i]->ambient.textureSetArray.size();
					for (auto iter1 = mModel->mMaterials[i]->ambient.textureSetArray.begin();
					iter1 != mModel->mMaterials[i]->ambient.textureSetArray.end(); ++iter1, ++j)
						newMtrlInfo.ambi_texNames.push_back(iter1->second[j]);

					//diff
					j = 0;
					newMtrlInfo.difftype = mModel->mMaterials[i]->diffuse.type;
					newMtrlInfo.diff_mcolor = mModel->mMaterials[i]->diffuse.color;
					newMtrlInfo.diff_mapCount = mModel->mMaterials[i]->diffuse.textureSetArray.size();
					for (auto iter1 = mModel->mMaterials[i]->diffuse.textureSetArray.begin();
					iter1 != mModel->mMaterials[i]->diffuse.textureSetArray.end(); ++iter1, ++j)
						newMtrlInfo.diff_texNames.push_back(iter1->second[j]);

					//emit
					j = 0;
					newMtrlInfo.emistype = mModel->mMaterials[i]->emissive.type;
					newMtrlInfo.emis_mcolor = mModel->mMaterials[i]->emissive.color;
					newMtrlInfo.emis_mapCount = mModel->mMaterials[i]->emissive.textureSetArray.size();
					for (auto iter1 = mModel->mMaterials[i]->emissive.textureSetArray.begin();
					iter1 != mModel->mMaterials[i]->emissive.textureSetArray.end(); ++iter1, ++j)
						newMtrlInfo.emis_texNames.push_back(iter1->second[j]);
					
					//spec
					j = 0;
					newMtrlInfo.spectype = mModel->mMaterials[i]->specular.type;
					newMtrlInfo.spec_mcolor = mModel->mMaterials[i]->specular.color;
					newMtrlInfo.spec_mapCount = mModel->mMaterials[i]->specular.textureSetArray.size();
					for (auto iter1 = mModel->mMaterials[i]->specular.textureSetArray.begin();
					iter1 != mModel->mMaterials[i]->specular.textureSetArray.end(); ++iter1, ++j)
						newMtrlInfo.spec_texNames.push_back(iter1->second[j]);
					newMtrlInfo.shineness = mModel->mMaterials[i]->shineness;
					newMtrlInfo.TransparencyFactor = mModel->mMaterials[i]->TransparencyFactor;

					// push back into the vector
					mModelInfo->mMtrlInfoVec.push_back(newMtrlInfo);
				}

				// bone data
				mModelInfo->mboneCount = static_cast<unsigned int>(mModel->mBoneData.mbonehierarchy.size());
				ufmt_loader::BoneInfo newBoneInfo;
				for (i = 0; i < mModelInfo->mboneCount; ++i)
				{
					// skin info will use model's name
					newBoneInfo.name = mModel->mBoneData.mbonehierarchy[i].mName.c_str();
					newBoneInfo.mParentIndex = mModel->mBoneData.mbonehierarchy[i].mParentIndex;
					newBoneInfo.bindPosition = mModel->mBoneData.mbonehierarchy[i].bindPosition;
					newBoneInfo.bindRotation = mModel->mBoneData.mbonehierarchy[i].bindRotation;
					newBoneInfo.bindScaling = mModel->mBoneData.mbonehierarchy[i].bindScaling;
					newBoneInfo.boneSpacePosition = mModel->mBoneData.mbonehierarchy[i].boneSpacePosition;
					newBoneInfo.boneSpaceRotation = mModel->mBoneData.mbonehierarchy[i].boneSpaceRotation;
					newBoneInfo.boneSpaceScaling = mModel->mBoneData.mbonehierarchy[i].boneSpaceScaling;

					// push back into the vector
					mModelInfo->mBoneInfoVec.push_back(newBoneInfo);
				}

				// level data
				// mesh lvl
				mModelInfo->mmeshlvlCount = mModelInfo->mmeshCount;
				for (i = 0; i < mModelInfo->mmeshlvlCount; ++i)
				{
					ufmt_loader::MeshInLvl newMLvl;
					newMLvl.meshTM = mModel->mMeshData[i]->meshTM;
					newMLvl.mParentIndex = mModel->mMeshData[i]->parentIndex;
					mModelInfo->mMeshLvVec.push_back(newMLvl);
				}
				// rig lvl
				mModelInfo->mriglvlCount = mModelInfo->mboneCount;
				for (i = 0; i < mModelInfo->mriglvlCount; ++i)
				{
					ufmt_loader::RigInLvl newRLvl;
					newRLvl.mParentIndex = mModelInfo->mBoneInfoVec[i].mParentIndex;
					mModelInfo->mRigLvVec.push_back(newRLvl);
				}
			}

			///////////////////////////////////////////////////////////////
			// Anim Info
			///////////////////////////////////////////////////////////////
			// anim data
			if (nullptr == mAnimInfo && !mModel->mAnimationData.empty())
			{
				mAnimInfo = new ufmt_loader::AnimInfo;

				mAnimInfo->name = mModel->name;
				mAnimInfo->animCount = static_cast<unsigned int>(mModel->mAnimationData.size());
				for (unsigned int i = 0; i < mAnimInfo->animCount; ++i)
				{
					ufmt_loader::AnimData newAD;
					ufmt_loader::KeyIndex newKIs;
					ufmt_loader::KFrames newKFs;

					// counts
					newAD.clipCount = static_cast<unsigned int>(mModel->mAnimationData[i]->animations.size());

					j = 0;
					for (auto iter2 = mModel->mAnimationData[i]->animations.begin();
					iter2 != mModel->mAnimationData[i]->animations.end(); ++iter2, ++j)
					{
						// storing animation clip's name
						newAD.clipname = iter2->first.c_str();

						// set keycount / keyframes
						newAD.boneCount = static_cast<unsigned int>(iter2->second.boneAnim.size());

						// Unifying keyframes of each animation
						unsigned int maxkfCount = 0;
						for (k = 0; k < iter2->second.boneAnim.size(); ++k)
						{
							unsigned int kfCount = static_cast<unsigned int>(iter2->second.boneAnim[k].keyFrames.size());
							if (maxkfCount < kfCount)
								maxkfCount = kfCount;
						}

						for (k = 0; k < iter2->second.boneAnim.size(); ++k)
						{
							ufmt_loader::KFrame newKF;
							unsigned int kfCount = static_cast<unsigned int>(iter2->second.boneAnim[k].keyFrames.size());
							newKIs.push_back(maxkfCount);
							for (l = 0; l < maxkfCount; ++l)
							{
								if (l < maxkfCount && l < kfCount)
									newKF.push_back(iter2->second.boneAnim[k].keyFrames[l]);
								else if (l >= kfCount)
									newKF.push_back(iter2->second.boneAnim[k].keyFrames[kfCount - 1]);
							}
							newKFs.push_back(newKF);
						}
					}
					newAD.keyIndices.push_back(newKIs);
					newAD.keyframes.push_back(newKFs);

					// push back into the vector
					mAnimInfo->animDataArr.push_back(newAD);
				}
			}
			
			return true;
		}
		
		bool CFBXLoader::CustomFileExport()
		{
			if (nullptr == mModelInfo || nullptr == mAnimInfo)
				return false;

			// set name for the custom file format and store it into Output folder
			std::string _fileName("Assets/Models/");
			_fileName += mModelInfo->name;
			std::string jdlFile = _fileName;
			std::string janiFile = _fileName;
			jdlFile += ".jdl";
			janiFile += ".jani";

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
			if (mAnimInfo)
			{
				hFile = CreateFile(janiFile.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
				if (!mAnimInfo->SerializeOut(hFile))
				{
					MessageBox(nullptr, "Jani Export Failed!", "", MB_OK);
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
				ProcessStaticMesh(pRoot, 0, -1);
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
			ProcessGeometry(pRoot, 0, -1);
		}

		void CFBXLoader::ProcessSkeletonHierarchy(FbxNode* pRoot)
		{
			for (int childIndex = 0; childIndex < pRoot->GetChildCount(); ++childIndex)
				ProcessSkeletonHierarchyRecursively(pRoot->GetChild(childIndex), 0, -1);
		}

		void CFBXLoader::ProcessSkeletonHierarchyRecursively(FbxNode* pNode, int myIndex, int inParentIndex)
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
				currJoint.bindRotation = FBXQuaternionToXMLOAT4(localMatrix.GetQ());
				currJoint.bindScaling = FBXVectorToXMFLOAT3(localMatrix.GetS());
				Swap(&currJoint.bindPosition.y, &currJoint.bindPosition.z);
				Swap(&currJoint.bindRotation.y, &currJoint.bindRotation.z);
				Swap(&currJoint.bindScaling.y, &currJoint.bindScaling.z);

				// Set bone space position, rotation matrices
				currJoint.boneSpacePosition = FBXVectorToXMFLOAT3(inverse.GetT());
				currJoint.boneSpaceRotation = FBXQuaternionToXMLOAT4(inverse.GetQ());
				currJoint.boneSpaceScaling = FBXVectorToXMFLOAT3(inverse.GetS());
				Swap(&currJoint.boneSpacePosition.y, &currJoint.boneSpacePosition.z);
				Swap(&currJoint.boneSpaceRotation.y, &currJoint.boneSpaceRotation.z);
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
				if(bBone)
					ProcessSkeletonHierarchyRecursively(pNode->GetChild(i), mModel->mBoneData.mbonehierarchy.size(), myIndex);
				else
					ProcessSkeletonHierarchyRecursively(pNode->GetChild(i), myIndex, inParentIndex);
			}
		}

		void CFBXLoader::ProcessStaticMesh(FbxNode* pNode, int myindex, int inParentIndex)
		{
			FbxMesh* mesh = pNode->GetMesh();
			bool bMesh = false;
			if (mesh)
			{
				FBX_DATA::MeshData* newMesh = new FBX_DATA::MeshData;
				newMesh->name = pNode->GetName();
				newMesh->mLayout = FBX_DATA::STATIC;
				newMesh->parentIndex = inParentIndex;
				if ("" == newMesh->name)
					newMesh->name = mModel->name;

				FbxAMatrix  meshTransform;
				int nodeIdx = 0;
				FbxPose* targetFP = nullptr;
				targetFP = GetAnimPoseAndIdx(pNode, nodeIdx);
				if (targetFP)
					meshTransform = GetPoseMatrix(targetFP, nodeIdx);
				else
					meshTransform = GetGlobalDefaultPosition(pNode);

				//Meshes have a separate geometry transform that also needs to be applied
				FbxAMatrix geoTransform = GetGeometryTransformation(pNode);
				meshTransform = meshTransform * geoTransform;
				mConverter->ConvertMatrix(meshTransform);\

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
					FbxVector4 vtx;
					vtx.mData[0] = newMesh->vertices[i].x;
					vtx.mData[1] = newMesh->vertices[i].y;
					vtx.mData[2] = newMesh->vertices[i].z;
					vtx.mData[3] = 0.0f;
					newMesh->vertices[i] = FBXVectorToXMFLOAT3(Transform(meshTransform, vtx));
				}
			}
			for (int i = 0; i < pNode->GetChildCount(); ++i)
			{
				if(bMesh)
					ProcessStaticMesh(pNode->GetChild(i), mModel->mMeshData.size(), myindex);
				else
					ProcessStaticMesh(pNode->GetChild(i), myindex, inParentIndex);
			}
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
						mConverter->ConvertVector(lNormal);
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
							mConverter->ConvertVector(lNormal);
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
							pData->tangents[lIndexByPolygonVertex] = FBXVectorToXMFLOAT3(lTangent.mData);
							++lIndexByPolygonVertex;
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
				for (int i = 0; i < lTextureCount; ++i)
				{
					FbxFileTexture* lFileTexture = lProperty.GetSrcObject<FbxFileTexture>(i);
					if (!lFileTexture)
						continue;

					FbxString uvsetName = lFileTexture->UVSet.Get();
					std::string uvSetString = uvsetName.Buffer();
					fs::path fName(lFileTexture->GetFileName());
					pElement->textureSetArray[uvSetString].push_back(fName.filename().string());
					++existTextureCount;
				}

				// Layered Texture
				const int lLayeredTextureCount = lProperty.GetSrcObjectCount<FbxLayeredTexture>();
				for (int i = 0; i < lLayeredTextureCount; ++i)
				{
					FbxLayeredTexture* lLayeredTexture = lProperty.GetSrcObject<FbxLayeredTexture>(i);
					const int lTextureFileCount = lLayeredTexture->GetSrcObjectCount<FbxFileTexture>();
					for (int j = 0; j < lTextureFileCount; ++j)
					{
						FbxFileTexture* lFileTexture = lLayeredTexture->GetSrcObject<FbxFileTexture>(j);
						if (!lFileTexture)
							continue;

						FbxString uvsetName = lFileTexture->UVSet.Get();
						std::string uvSetString = uvsetName.Buffer();
						fs::path fName(lFileTexture->GetFileName());
						pElement->textureSetArray[uvSetString].push_back(fName.filename().string());
						++existTextureCount;
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
			if (materialElement)
			{
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
					animClip.boneAnim[boneindex].keyFrames[i].rot = FBXQuaternionToXMLOAT4(localMatrix.GetQ());
					animClip.boneAnim[boneindex].keyFrames[i].scl = FBXVectorToXMFLOAT3(localMatrix.GetS());
					Swap(&animClip.boneAnim[boneindex].keyFrames[i].trans.y, &animClip.boneAnim[boneindex].keyFrames[i].trans.z);
					Swap(&animClip.boneAnim[boneindex].keyFrames[i].rot.y, &animClip.boneAnim[boneindex].keyFrames[i].rot.z);
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

		void CFBXLoader::ProcessGeometry(FbxNode* pNode, int myindex, int inParentIndex)
		{
			FbxMesh* mesh = pNode->GetMesh();
			bool bMesh = false;
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
					meshTransform = GetPoseMatrix(targetFP, nodeIdx);
				else
					meshTransform = GetGlobalDefaultPosition(pNode);

				//Meshes have a separate geometry transform that also needs to be applied
				FbxAMatrix geoTransform = GetGeometryTransformation(pNode);
				meshTransform = meshTransform * geoTransform;
				mConverter->ConvertMatrix(meshTransform);

				ProcessVertices(mesh, newMesh);
				ProcessNormals(mesh, newMesh);
				ProcessTangent(mesh, newMesh);
				ProcessTexcoord(mesh, newMesh);
				ProcessMaterials(pNode, newMesh);

				//go through all the control points(verticies) and multiply by the transformation
				for (unsigned int i = 0; i < newMesh->vertexCnt; ++i)
				{
					FbxVector4 vtx;
					vtx.mData[0] = newMesh->vertices[i].x;
					vtx.mData[1] = newMesh->vertices[i].y;
					vtx.mData[2] = newMesh->vertices[i].z;
					vtx.mData[3] = 0.0f;
					newMesh->vertices[i] = FBXVectorToXMFLOAT3(Transform(meshTransform, vtx));
				}
				mModel->mMeshData.push_back(newMesh);
				bMesh = true;
			}
			//go through all the child node and grab there geometry information
			int childCnt = pNode->GetChildCount();
			for (int i = 0; i < childCnt; ++i)
			{
				if (bMesh)
					ProcessGeometry(pNode->GetChild(i), mModel->mMeshData.size(), myindex);
				else
					ProcessGeometry(pNode->GetChild(i), myindex, inParentIndex);
			}
		}

		//getting anim pose
		FbxPose* CFBXLoader::GetAnimPoseAndIdx(FbxNode* pNode, int& index)
		{
			for (auto iter : mModel->mAnimPose)
			{
				index = iter->Find(pNode);
				if (-1 != index)
					return iter;
			}
			return nullptr;
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
		
		//reconstruct vertices and indices
		void CFBXLoader::Reconstruct(unsigned int meshIdx, std::vector<ufmt_loader::MeshVertex>& mvVec, std::vector<unsigned int>& miVec, const FBX_DATA::MeshData& md)
		{
			for (unsigned int i = 0; i < md.indexCnt; ++i)
			{
				ufmt_loader::MeshVertex newMV;
				newMV.pos = md.vertices[md.indices[i]];

				if (md.normals)
				{
					if (md.normalMode == FbxGeometryElement::eByPolygonVertex)
						newMV.normal = md.normals[i];
					else if (md.normalMode == FbxGeometryElement::eByControlPoint)
						newMV.normal = md.normals[md.indices[i]];
				}

				if (md.tangents)
				{
					if (md.tangentMode == FbxGeometryElement::eByPolygonVertex)
						newMV.tangent = md.tangents[i];
					else if (md.tangentMode == FbxGeometryElement::eByControlPoint)
						newMV.tangent = md.tangents[md.indices[i]];
				}

				if (md.uvs)
				{
					newMV.uv = md.uvs[i];
					newMV.uv.y = 1.0f - newMV.uv.y;
				}

				// controls - maybe divide this part later if necessary
				if (!mModel->mCtrlPoints.empty())
				{
					if (!(*mModel->mCtrlPoints[meshIdx]).empty())
					{
						// currently, just for using 1st control point vec
						newMV.ctrlBlendWeights.x = mModel->mCtrlPoints[meshIdx]->at(md.indices[i])->mBlendingInfo[0].mBlendingWeight;
						newMV.ctrlBlendWeights.y = mModel->mCtrlPoints[meshIdx]->at(md.indices[i])->mBlendingInfo[1].mBlendingWeight;
						newMV.ctrlBlendWeights.z = mModel->mCtrlPoints[meshIdx]->at(md.indices[i])->mBlendingInfo[2].mBlendingWeight;
						newMV.ctrlBlendWeights.w = mModel->mCtrlPoints[meshIdx]->at(md.indices[i])->mBlendingInfo[3].mBlendingWeight;
						newMV.ctrlIndices.x = mModel->mCtrlPoints[meshIdx]->at(md.indices[i])->mBlendingInfo[0].mBlendingIndex;
						newMV.ctrlIndices.y = mModel->mCtrlPoints[meshIdx]->at(md.indices[i])->mBlendingInfo[1].mBlendingIndex;
						newMV.ctrlIndices.z = mModel->mCtrlPoints[meshIdx]->at(md.indices[i])->mBlendingInfo[2].mBlendingIndex;
						newMV.ctrlIndices.w = mModel->mCtrlPoints[meshIdx]->at(md.indices[i])->mBlendingInfo[3].mBlendingIndex;
					}
				}

				bool bFound = false;
				unsigned int index = 0;
				for (unsigned int j = 0; j < mvVec.size(); ++j)
				{
					if (newMV == mvVec[j])
					{
						bFound = true;
						// if there is same MeshVertex, just add the index of it.
						miVec.push_back(j);
						break;
					}
					++index;
				}

				// if there is no MeshVertex, store the vertex and set a index as the last one
				if (!bFound)
				{
					mvVec.push_back(newMV);
					miVec.push_back(index);
				}
			}
		}
	}
}
