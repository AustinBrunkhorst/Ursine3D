/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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

void Swap(void *a, void *b)
{
    void *temp;
    temp = *(void**)a;
    *(void**)a = *(void**)b;
    *(void**)b = temp;
}

// Debug Test file
//void DebugPrint()
//{
//    std::ofstream fout;
//    std::string nm = m_Model->name.c_str();
//    nm += ".txt";
//    fout.open(nm);
//
//    fout << "Name : " << iter.name << std::endl;
//    fout << "PI : " << iter.mParentIndex << std::endl;
//    fout << "BP : " << iter.bindPosition.x << "," << iter.bindPosition.y << "," << iter.bindPosition.z << std::endl;
//    fout << "BR : " << iter.bindRotation.x << "," << iter.bindRotation.y << "," << iter.bindRotation.z << std::endl;
//    fout << "BS : " << iter.bindScaling.x << "," << iter.bindScaling.y << "," << iter.bindScaling.z << std::endl;
//    fout << "SP : " << iter.boneSpacePosition.x << "," << iter.boneSpacePosition.y << "," << iter.boneSpacePosition.z << std::endl;
//    fout << "SR : " << iter.boneSpaceRotation.x << "," << iter.boneSpaceRotation.y << "," << iter.boneSpaceRotation.z << std::endl;
//    fout << "SS : " << iter.boneSpaceScaling.x << "," << iter.boneSpaceScaling.y << "," << iter.boneSpaceScaling.z << std::endl;
//
//    if (fout.is_open() == true)
//        fout.close();
//}

namespace ursine
{
    namespace graphics
    {
        CFBXLoader::CFBXLoader() :
            mSdkManager(nullptr),
            mScene(nullptr),
            m_Model(nullptr),
            mConverter(nullptr)
        {
        }

        CFBXLoader::~CFBXLoader()
        {
            // after finished exporting, then release the data
            if (mConverter)
                delete mConverter;

            if (m_Model)
            {
                delete m_Model;
                m_Model = nullptr;
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

        bool CFBXLoader::LoadFBX(const std::string &filename)
        {
            InitializeSdkObjects(mSdkManager, mScene);
            if (!mSdkManager)
                return false;

            int lFileFormat = -1;
            if (!mSdkManager->GetIOPluginRegistry()->DetectReaderFileFormat(filename.c_str(), lFileFormat))
            {
                // Unrecognizable file format. Try to fall back to FbxImporter::eFBX_BINARY
                lFileFormat = mSdkManager->GetIOPluginRegistry()->FindReaderIDByDescription("FBX binary (*.fbx)");
            }

            // Initialize the importer by providing a filename.
            mImporter = FbxImporter::Create(mSdkManager, "HJImporter");
            if (!mImporter)
                return false;
            if (false == mImporter->Initialize(filename.c_str(), lFileFormat, mSdkManager->GetIOSettings()))
                return false;
            if (false == mImporter->Import(mScene))
                return false;

            // this will do conversion for me
            FbxGeometryConverter lGConverter(mSdkManager);
            lGConverter.SplitMeshesPerMaterial(mScene, false);
            mConverter = new Converter(mScene);

            // Process Scene
            m_Model = new FBX_DATA::FbxModel;
            fs::path fName(filename);
            m_Model->name = fName.filename().string();
            m_Model->name = m_Model->name.substr(0, m_Model->name.rfind("."));

            // Getting Anim Pose
            m_Model->mAnimPose.resize(mScene->GetPoseCount());
            for (unsigned i = 0; i < m_Model->mAnimPose.size(); ++i)
                m_Model->mAnimPose[i] = mScene->GetPose(i);

            ProcessScene(mScene->GetRootNode());

            // Export FBX model as custom file format
            ReadyToExport();

            return true;
        }

        void CFBXLoader::InitializeSdkObjects(FbxManager*& pManager, ::FbxScene*& pScene)
        {
            //The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
            pManager = FbxManager::Create();

            UAssertCatchable(pManager,
                "FBXManager was null."
                );

            //Create an IOSettings object. This object holds all import/export settings.
            FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
            pManager->SetIOSettings(ios);

            //Load plugins from the executable directory (optional)
            FbxString lPath = FbxGetApplicationDirectory();
            pManager->LoadPluginsDirectory(lPath.Buffer());

            //Create an FBX scene. This object holds most objects imported/exported from/to files.
            pScene = FbxScene::Create(pManager, "My Scene");

            UAssertCatchable(pScene,
                "Unable to create scene."
                );
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
            unsigned int i = 0, j = 0, k = 0;

            ///////////////////////////////////////////////////////////////
            // Model Info
            ///////////////////////////////////////////////////////////////
            // mesh data
            {
                m_ModelInfo.name = m_Model->name.c_str();

                i = 0;

                m_ModelInfo.mmeshCount = static_cast<unsigned int>(m_Model->mMeshData.size());
                m_ModelInfo.mMeshInfoVec.resize(m_ModelInfo.mmeshCount);

                for (auto &iter : m_ModelInfo.mMeshInfoVec)
                {
                    FBX_DATA::MeshData& currMD = m_Model->mMeshData[i];
                    // name & counter initialization
                    iter.name = currMD.name.c_str();

                    // Reconstruct Vertices and Indices for data compression
                    std::vector<ufmt_loader::MeshVertex> rmvVec;
                    std::vector<unsigned int> rIVec;
                    Reconstruct(i, rmvVec, rIVec, currMD);

                    j = 0;
                    iter.meshVtxInfoCount = rmvVec.size();
                    iter.meshVtxInfos.resize(iter.meshVtxInfoCount);
                    for (auto &iter2 : iter.meshVtxInfos)
                    {
                        iter2 = rmvVec[j];
                        ++j;
                    }

                    j = 0;
                    iter.meshVtxIdxCount = rIVec.size();
                    iter.meshVtxIndices.resize(iter.meshVtxIdxCount);
                    for (auto &iter2 : iter.meshVtxIndices)
                    {
                        iter2 = rIVec[j];
                        ++j;
                    }

                    ++i;
                }

                // material data
                i = 0;
                m_ModelInfo.mmaterialCount = static_cast<unsigned int>(m_Model->mMaterials.size());
                m_ModelInfo.mMtrlInfoVec.resize(m_ModelInfo.mmaterialCount);
                for (auto &iter : m_ModelInfo.mMtrlInfoVec)
                {
                    iter.name = m_Model->mMaterials[i].name;

                    //ambi
                    j = 0;
                    iter.ambitype = m_Model->mMaterials[i].ambient.type;
                    iter.ambi_mcolor = m_Model->mMaterials[i].ambient.color;
                    iter.ambi_mapCount = m_Model->mMaterials[i].ambient.textureSetArray.size();
                    iter.ambi_texNames.resize(iter.ambi_mapCount);
                    for (auto &iter1 : m_Model->mMaterials[i].ambient.textureSetArray)
                    {
                        iter.ambi_texNames[j] = iter1.second[j];
                        ++j;
                    }

                    //diff
                    j = 0;
                    iter.difftype = m_Model->mMaterials[i].diffuse.type;
                    iter.diff_mcolor = m_Model->mMaterials[i].diffuse.color;
                    iter.diff_mapCount = m_Model->mMaterials[i].diffuse.textureSetArray.size();
                    iter.diff_texNames.resize(iter.diff_mapCount);
                    for (auto &iter1 : m_Model->mMaterials[i].diffuse.textureSetArray)
                    {
                        iter.diff_texNames[j] = iter1.second[j];
                        ++j;
                    }

                    //emit
                    j = 0;
                    iter.emistype = m_Model->mMaterials[i].emissive.type;
                    iter.emis_mcolor = m_Model->mMaterials[i].emissive.color;
                    iter.emis_mapCount = m_Model->mMaterials[i].emissive.textureSetArray.size();
                    iter.emis_texNames.resize(iter.emis_mapCount);
                    for (auto &iter1 : m_Model->mMaterials[i].emissive.textureSetArray)
                    {
                        iter.emis_texNames[j] = iter1.second[j];
                        ++j;
                    }

                    //spec
                    j = 0;
                    iter.spectype = m_Model->mMaterials[i].specular.type;
                    iter.spec_mcolor = m_Model->mMaterials[i].specular.color;
                    iter.spec_mapCount = m_Model->mMaterials[i].specular.textureSetArray.size();
                    iter.spec_texNames.resize(iter.spec_mapCount);
                    for (auto &iter1 : m_Model->mMaterials[i].specular.textureSetArray)
                    {
                        iter.spec_texNames[j] = iter1.second[j];
                        ++j;
                    }

                    ++i;
                }

                // bone data
                i = 0;
                m_ModelInfo.mboneCount = static_cast<unsigned int>(m_Model->mBoneData.mbonehierarchy.size());
                m_ModelInfo.mBoneInfoVec.resize(m_ModelInfo.mboneCount);
                for (auto &iter : m_ModelInfo.mBoneInfoVec)
                {
                    // skin info will use model's name
                    iter.name = m_Model->mBoneData.mbonehierarchy[i].mName.c_str();
                    iter.mParentIndex = m_Model->mBoneData.mbonehierarchy[i].mParentIndex;
                    iter.bindPosition = m_Model->mBoneData.mbonehierarchy[i].bindPosition;
                    iter.bindRotation = m_Model->mBoneData.mbonehierarchy[i].bindRotation;
                    iter.bindScaling = m_Model->mBoneData.mbonehierarchy[i].bindScaling;
                    iter.boneSpacePosition = m_Model->mBoneData.mbonehierarchy[i].boneSpacePosition;
                    iter.boneSpaceRotation = m_Model->mBoneData.mbonehierarchy[i].boneSpaceRotation;
                    iter.boneSpaceScaling = m_Model->mBoneData.mbonehierarchy[i].boneSpaceScaling;

                    ++i;
                }

                // level data
                // mesh lvl
                i = 0;
                m_ModelInfo.mmeshlvlCount = m_ModelInfo.mmeshCount;
                m_ModelInfo.mMeshLvVec.resize(m_ModelInfo.mmeshlvlCount);
                for (auto &iter : m_ModelInfo.mMeshLvVec)
                {
                    FBX_DATA::MeshData& currMD = m_Model->mMeshData[i];
                    iter.meshName = currMD.name;
                    iter.meshTM = currMD.meshTM;
                    iter.mParentIndex = currMD.parentIndex;

                    ++i;
                }

                i = 0;
                // rig lvl
                m_ModelInfo.mriglvlCount = m_ModelInfo.mboneCount;
                m_ModelInfo.mRigLvVec.resize(m_ModelInfo.mriglvlCount);
                for (auto &iter : m_ModelInfo.mRigLvVec)
                {
                    iter.boneName = m_ModelInfo.mBoneInfoVec[i].name;
                    iter.mParentIndex = m_ModelInfo.mBoneInfoVec[i].mParentIndex;

                    ++i;
                }
            }

            ///////////////////////////////////////////////////////////////
            // Anim Info
            ///////////////////////////////////////////////////////////////
            // anim data
            if (!m_Model->mAnimationData.empty())
            {
                i = 0;

                m_AnimInfo.name = m_Model->name;
                m_AnimInfo.animCount = static_cast<unsigned int>(m_Model->mAnimationData.size());
                m_AnimInfo.animDataArr.resize(m_AnimInfo.animCount);

                for (auto &iter : m_AnimInfo.animDataArr)
                {
                    iter.keyIndices.resize(m_AnimInfo.animCount);
                    iter.keyframes.resize(m_AnimInfo.animCount);

                    // counts
                    iter.clipCount = static_cast<unsigned int>(m_Model->mAnimationData[i].animations.size());

                    /////////////////////////////////////////////////////
                    // Push back dummy value should includes specific time, not default
                    // Should build keyframe vector which can cover the beginning and the end
                    // and then should cover all keyframe values with reasonable keyframe.
                    /////////////////////////////////////////////////////
                    URSINE_TODO("Jun! You should fix this!!!!");
                    for (auto &iter1 : m_Model->mAnimationData[i].animations)
                    {
                        // storing animation clip's name
                        iter.clipname = iter1.first.c_str();

                        // set keycount / keyframes
                        iter.boneCount = static_cast<unsigned int>(iter1.second.boneAnim.size());
                        iter.keyIndices[i].resize(iter.boneCount);
                        iter.keyframes[i].resize(iter.boneCount);

                        // Unifying keyframes of each animation
                        unsigned int maxkfCount = 0;
                        for (auto &iter2 : iter1.second.boneAnim)
                        {
                            unsigned int kfCount = static_cast<unsigned int>(iter2.keyFrames.size());
                            if (maxkfCount < kfCount)
                                maxkfCount = kfCount;
                        }

                        j = 0;
                        for (auto &iter2 : iter1.second.boneAnim)
                        {
                            unsigned int kfCount = static_cast<unsigned int>(iter2.keyFrames.size());

                            iter.keyIndices[i][j] = maxkfCount;
                            iter.keyframes[i][j].resize(maxkfCount);

                            for (k = 0; k < maxkfCount; ++k)
                            {
                                if (k < maxkfCount && k < kfCount)
                                    iter.keyframes[i][j][k] = iter1.second.boneAnim[j].keyFrames[k];
                                else if (k >= kfCount)
                                    iter.keyframes[i][j][k] = iter1.second.boneAnim[j].keyFrames[kfCount - 1];
                            }
                            ++j;
                        }
                    }
                    ++i;
                }
            }

            return true;
        }

        //========================================================================
        // My FBX Loader public functions+
        //========================================================================
        void CFBXLoader::ProcessScene(FbxNode* pRoot)
        {
            // FBX could be 3 diff types - Static Mesh, Just Animation Data, Skinned Mesh
            // if there is no animation, just static mesh
            // else process animation
            m_AnimationFlag.first = m_AnimationFlag.second = false;

            // Skeleton 
            ProcessSkeletonHierarchy(pRoot);
            if (m_Model->mBoneData.mbonehierarchy.empty())
            {
                // Process Static Mesh
                ProcessStaticMesh(pRoot, 0, -1);
                return;
            }
            else
            {
                m_AnimationFlag.first = true;
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

                int nodeIdx = -1;

                FbxPose* targetFP = nullptr;
                targetFP = GetAnimPoseAndIdx(pNode, nodeIdx);

                FbxAMatrix localMatrix;
                if (targetFP)
                {
                    UAssert(-1 != nodeIdx, "Fuck you Fuck you");
                    localMatrix = GetPoseMatrix(targetFP, nodeIdx);
                }
                else
                    localMatrix.SetIdentity();

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

                // Set bone space position, rotation matrices
                currJoint.boneSpacePosition = FBXVectorToXMFLOAT3(inverse.GetT());
                currJoint.boneSpaceRotation = FBXQuaternionToXMLOAT4(inverse.GetQ());
                currJoint.boneSpaceScaling = FBXVectorToXMFLOAT3(inverse.GetS());

                // skeleton = bone hierarchy
                m_Model->mBoneData.mbonehierarchy.push_back(currJoint);
                m_Model->mBoneData.mboneNodes.push_back(pNode);

                SMat4 locTM = FBXAMatrixToSMat4(&localMatrix);
                m_Model->mBoneData.mboneLocalTM.push_back(locTM);

                bBone = true;
            }

            for (int i = 0; i < pNode->GetChildCount(); ++i)
            {
                if (bBone)
                    ProcessSkeletonHierarchyRecursively(pNode->GetChild(i), m_Model->mBoneData.mbonehierarchy.size(), myIndex);
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
                FBX_DATA::MeshData newMesh;
                newMesh.name = pNode->GetName();
                newMesh.mLayout = FBX_DATA::STATIC;
                newMesh.parentIndex = inParentIndex;
                if ("" == newMesh.name)
                    newMesh.name = m_Model->name;

                FbxAMatrix  meshTransform;

                int nodeIdx = -1;

                FbxPose* targetFP = nullptr;
                targetFP = GetAnimPoseAndIdx(pNode, nodeIdx);

                if (targetFP)
                {
                    UAssert(-1 != nodeIdx, "Fuck you Fuck you");
                    meshTransform = GetPoseMatrix(targetFP, nodeIdx);
                }
                else
                    meshTransform = GetGlobalDefaultPosition(pNode);

                //Meshes have a separate geometry transform that also needs to be applied
                FbxAMatrix geoTransform = GetGeometryTransformation(pNode);
                meshTransform = meshTransform * geoTransform;
                mConverter->ConvertMatrix(meshTransform);

                // vertex, normal, tangent, texcoord, material
                ProcessVertices(mesh, &newMesh);
                ProcessNormals(mesh, &newMesh);
                ProcessBinormal(mesh, &newMesh);
                ProcessTangent(mesh, &newMesh);
                ProcessTexcoord(mesh, &newMesh);
                ProcessMaterials(pNode, &newMesh);
                m_Model->mMeshData.push_back(newMesh);

                //go through all the control points(verticies) and multiply by the transformation
                for (auto &iter : newMesh.vertices)
                {
                    FbxVector4 vtx;
                    vtx.mData[0] = iter.x;
                    vtx.mData[1] = iter.y;
                    vtx.mData[2] = iter.z;
                    vtx.mData[3] = 0.0f;
                    iter = FBXVectorToXMFLOAT3(Transform(meshTransform, vtx));
                }
            }
            for (int i = 0; i < pNode->GetChildCount(); ++i)
            {
                if (bMesh)
                    ProcessStaticMesh(pNode->GetChild(i), m_Model->mMeshData.size(), myindex);
                else
                    ProcessStaticMesh(pNode->GetChild(i), myindex, inParentIndex);
            }
        }

        void CFBXLoader::ProcessVertices(FbxMesh* pMesh, FBX_DATA::MeshData* pData)
        {
            if (nullptr == pMesh)
                return;

            unsigned int ctrlPtCnt = pMesh->GetControlPointsCount();

            // vertices
            pData->vertices.resize(ctrlPtCnt);
            for (unsigned int i = 0; i < ctrlPtCnt; ++i)
            {
                FbxVector4 v = pMesh->GetControlPointAt(i);
                mConverter->ConvertVector(v);
                pData->vertices[i] = FBXVectorToXMFLOAT3(v);
            }

            // indicies
            unsigned int indexCnt = pMesh->GetPolygonVertexCount();
            pData->indices.resize(indexCnt);
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
                    pData->normals.resize(normalCount);

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
                    pData->normals.resize(normalCount);

                    for (int lPolygonIndex = 0; lPolygonIndex < pMesh->GetPolygonCount(); ++lPolygonIndex)
                    {
                        int lPolygonSize = pMesh->GetPolygonSize(lPolygonIndex);

                        UAssertCatchable(lPolygonSize == 3,
                            "Model is not triangulated.\npoly size: %i",
                            lPolygonSize
                            );

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

        void CFBXLoader::ProcessBinormal(FbxMesh* pMesh, FBX_DATA::MeshData* pData)
        {
            //get the normal element attribute
            FbxGeometryElementBinormal* binormalElement = pMesh->GetElementBinormal();
        
            if (binormalElement)
            {
                switch (binormalElement->GetMappingMode())
                {
                case FbxGeometryElement::eByControlPoint:
                {
                    unsigned binormalCount = pMesh->GetControlPointsCount();
                    pData->binormalMode = FbxGeometryElement::eByControlPoint;
                    pData->binormals.resize(binormalCount);
        
                    for (int lVertexIndex = 0; lVertexIndex < pMesh->GetControlPointsCount(); ++lVertexIndex)
                    {
                        int lBinormalIndex = 0;
        
                        if (binormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
                            lBinormalIndex = lVertexIndex;
                        if (binormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
                            lBinormalIndex = binormalElement->GetIndexArray().GetAt(lVertexIndex);
        
                        FbxVector4 lBinormal = binormalElement->GetDirectArray().GetAt(lBinormalIndex);
                        mConverter->ConvertVector(lBinormal);
                        pData->binormals[lVertexIndex] = FBXVectorToXMFLOAT3(lBinormal.mData);
                    }
                }
                break;
                case FbxGeometryElement::eByPolygonVertex:
                {
                    int lIndexByPolygonVertex = 0;
        
                    unsigned binormalCount = pMesh->GetPolygonCount() * pMesh->GetPolygonSize(0);
        
                    pData->binormalMode = FbxGeometryElement::eByPolygonVertex;
                    pData->binormals.resize(binormalCount);
        
                    for (int lPolygonIndex = 0; lPolygonIndex < pMesh->GetPolygonCount(); ++lPolygonIndex)
                    {
                        int lPolygonSize = pMesh->GetPolygonSize(lPolygonIndex);
        
                        UAssertCatchable(lPolygonSize == 3,
                            "Model is not triangulated.\npoly size: %i",
                            lPolygonSize
                            );
        
                        for (int i = 0; i < lPolygonSize; ++i)
                        {
                            int lBinormalIndex = 0;
        
                            if (binormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
                                lBinormalIndex = lIndexByPolygonVertex;
                            if (binormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
                                lBinormalIndex = binormalElement->GetIndexArray().GetAt(lIndexByPolygonVertex);
        
                            FbxVector4 lBinormal = binormalElement->GetDirectArray().GetAt(lBinormalIndex);
                            mConverter->ConvertVector(lBinormal);
                            pData->binormals[lIndexByPolygonVertex] = FBXVectorToXMFLOAT3(lBinormal.mData);
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
                    pData->tangents.resize(tangentCount);

                    for (int lVertexIndex = 0; lVertexIndex < pMesh->GetControlPointsCount(); ++lVertexIndex)
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
                    pData->tangents.resize(tangentCount);

                    for (int lPolygonIndex = 0; lPolygonIndex < pMesh->GetPolygonCount(); ++lPolygonIndex)
                    {
                        int lPolygonSize = pMesh->GetPolygonSize(lPolygonIndex);

                        UAssertCatchable(lPolygonSize == 3,
                            "Model is not triangulated.\npoly size: %i",
                            lPolygonSize
                            );

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
                    && lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
                    return;

                //index array, where holds the index referenced to the uv data
                const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
                const int lIndexCount = (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;

                switch (lUVElement->GetMappingMode())
                {
                case FbxGeometryElement::eByControlPoint:
                {
                    unsigned int uvCount = pMesh->GetControlPointsCount();

                    pData->uvs.resize(uvCount);
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

                    pData->uvs.resize(lIndexCount);

                    const int lPolyCount = pMesh->GetPolygonCount();

                    for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
                    {
                        // build the max index array that we need to pass into MakePoly
                        const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);

                        UAssertCatchable(lPolySize == 3,
                            "Model is not triangulated.\npoly size: %i",
                            lPolySize
                            );

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
                        destMat.type = FBX_DATA::FbxMaterial::Type_Lambert;
                    else if (material->GetClassId().Is(FbxSurfacePhong::ClassId))
                        destMat.type = FBX_DATA::FbxMaterial::Type_Phong;

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
                    m_Model->mMaterials.push_back(destMat);
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
                    pData->materialIndices.resize(materialIndicies.GetCount());
                    unsigned int i = 0;
                    for (auto &iter : pData->materialIndices)
                    {
                        iter = materialIndicies[i];
                        ++i;
                    }
                }
                else if (materialElement->GetMappingMode() == FbxGeometryElement::eAllSame)
                {
                    pData->materialIndices.resize(pData->indices.size() / pNode->GetMesh()->GetPolygonSize(0));
                    for(auto &iter : pData->materialIndices)
                        iter = materialIndicies[0];
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

                    // control point is vertex of the cluster
                    // these control points will be influenced by bone animation
                    FBX_DATA::ControlPoints newCtrlPoints;

                    for (unsigned int clusterIndex = 0; clusterIndex < numCluster; ++clusterIndex)
                    {
                        FbxCluster* cluster = skin->GetCluster(clusterIndex);

                        if (!cluster->GetLink())
                            continue;

                        //joint name and index
                        FbxNode* pLink = cluster->GetLink();

                        int boneIdx = GetJointIndexByName(pLink->GetName());

                        UAssert(boneIdx != -1, "Bone Parsing Fucked up");

                        // New bind pose matrix importing method
                        // Check negative scale
                        {
                            FbxAMatrix bindPoseMatrix;

                            cluster->GetTransformLinkMatrix(bindPoseMatrix);

                            mConverter->ConvertMatrix(bindPoseMatrix);

                            FbxVector4 scl = bindPoseMatrix.GetS();
                            if (!CheckScaling(scl) || !CheckPositive(scl))
                                bindPoseMatrix.SetS(FbxVector4(1, 1, 1));

                            FbxAMatrix inverse = bindPoseMatrix.Inverse();

                            m_Model->mBoneData.mbonehierarchy[boneIdx].bindPosition = FBXVectorToXMFLOAT3(bindPoseMatrix.GetT());
                            m_Model->mBoneData.mbonehierarchy[boneIdx].bindRotation = FBXQuaternionToXMLOAT4(bindPoseMatrix.GetQ());
                            m_Model->mBoneData.mbonehierarchy[boneIdx].bindScaling = FBXVectorToXMFLOAT3(bindPoseMatrix.GetS());

                            m_Model->mBoneData.mbonehierarchy[boneIdx].boneSpacePosition = FBXVectorToXMFLOAT3(inverse.GetT());
                            m_Model->mBoneData.mbonehierarchy[boneIdx].boneSpaceRotation = FBXQuaternionToXMLOAT4(inverse.GetQ());
                            m_Model->mBoneData.mbonehierarchy[boneIdx].boneSpaceScaling = FBXVectorToXMFLOAT3(inverse.GetS());

                            SMat4 locTM = FBXAMatrixToSMat4(&bindPoseMatrix);
                            m_Model->mBoneData.mboneLocalTM[boneIdx] = locTM;
                        }

                        double* weights = cluster->GetControlPointWeights();

                        int* indicies = cluster->GetControlPointIndices();

                        // Associate each joint with the control points it affects
                        int ctrlPtIdxCnt = cluster->GetControlPointIndicesCount();

                        for (int i = 0; i < ctrlPtIdxCnt; ++i)
                        {
                            FBX_DATA::BlendIdxWeight currBlendIdxWeight;

                            currBlendIdxWeight.mBlendingIndex = boneIdx;
                            currBlendIdxWeight.mBlendingWeight = static_cast<float>(weights[i]);

                            int ctrlPtIdx = indicies[i];

                            newCtrlPoints[ctrlPtIdx].mBlendingInfo.push_back(currBlendIdxWeight);
                        }
                    }

                    m_Model->mCtrlPoints.push_back(newCtrlPoints);
                }
                ++index;
            }

            for (int i = 0; i < pNode->GetChildCount(); ++i)
                ProcessWeightBlend(pNode->GetChild(i), index);
        }

        void CFBXLoader::RemoveUnnecessaryWeights()
        {
            for (auto &controlIter : m_Model->mCtrlPoints)
            {
                //Remove or remove BlendWeights if there are more than 4
                for (auto &iter : controlIter)
                {
                    std::sort(iter.second.mBlendingInfo.begin(), iter.second.mBlendingInfo.end(), FBX_DATA::compare_bw_descend);

                    while (iter.second.mBlendingInfo.size() > 4)
                        iter.second.mBlendingInfo.pop_back();

                    // check if we have less than 4 blend weight then we have to add some dummy weights
                    // how can I give identity matrix to non-animating mesh? or at least just give it vertex
                    FBX_DATA::BlendIdxWeight currBlendWeight;
                    if (iter.second.mBlendingInfo.empty())
                    {
                        currBlendWeight.mBlendingWeight = 1.0f;
                        iter.second.mBlendingInfo.push_back(currBlendWeight);

                        currBlendWeight.mBlendingWeight = 0.0f;
                        iter.second.mBlendingInfo.push_back(currBlendWeight);
                        iter.second.mBlendingInfo.push_back(currBlendWeight);
                        iter.second.mBlendingInfo.push_back(currBlendWeight);

                    }
                    else
                    {
                        while (iter.second.mBlendingInfo.size() < 4)
                            iter.second.mBlendingInfo.push_back(currBlendWeight);
                    }

                    //normalize the weights
                    double sum = 0.0f;

                    for (int w = 0; w < 4; ++w)
                        sum += iter.second.mBlendingInfo[w].mBlendingWeight;

                    if (0.0f == sum)
                        continue;

                    for (int w = 0; w < 4; ++w)
                        iter.second.mBlendingInfo[w].mBlendingWeight /= sum;
                }
            }
        }

        int CFBXLoader::GetJointIndexByName(const std::string& inJointName)
        {
            for (unsigned int i = 0; i < m_Model->mBoneData.mbonehierarchy.size(); ++i)
            {
                if (m_Model->mBoneData.mbonehierarchy[i].mName == inJointName)
                    return i;
            }
            return -1;
        }

        FbxAMatrix CFBXLoader::GetPoseMatrix(FbxPose* pose, int index)
        {
            FbxMatrix lMatrix = pose->GetMatrix(index);
            return FBXMatrixToFBXAMatrix(&lMatrix);
        }

        bool CFBXLoader::CheckPositive(FbxVector4 scale)
        {
            if (scale[0] < 0.0f || scale[1] < 0.0f || scale[2] < 0.0f)
                return false;
            return true;
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

        void CFBXLoader::SetPivotStateRecursive(FbxNode* pNode)
        {
            // From FbxNode.h
            FbxVector4 lZero(0, 0, 0);
            FbxVector4 lOne(1, 1, 1);

            pNode->SetPivotState(FbxNode::eSourcePivot, FbxNode::ePivotActive);
            pNode->SetPivotState(FbxNode::eDestinationPivot, FbxNode::ePivotActive);

            EFbxRotationOrder lRotationOrder;
            pNode->GetRotationOrder(FbxNode::eSourcePivot, lRotationOrder);
            pNode->SetRotationOrder(FbxNode::eDestinationPivot, lRotationOrder);

            //For cameras and lights (without targets) let's compensate the postrotation.
            if (pNode->GetCamera() || pNode->GetLight())
            {
                if (!pNode->GetTarget())
                {
                    FbxVector4 lRV(90, 0, 0);

                    if (pNode->GetCamera())
                        lRV.Set(0, 90, 0);

                    FbxVector4 prV = pNode->GetPostRotation(FbxNode::eSourcePivot);
                    FbxAMatrix lSourceR;
                    FbxAMatrix lR(lZero, lRV, lOne);
                    FbxVector4 res = prV;

                    // Rotation order don't affect post rotation, so just use the default XYZ order
                    FbxRotationOrder rOrder;
                    rOrder.V2M(lSourceR, res);

                    lR = lSourceR * lR;
                    rOrder.M2V(res, lR);
                    prV = res;

                    pNode->SetPostRotation(FbxNode::eSourcePivot, prV);
                    pNode->SetRotationActive(true);
                }

                // Point light do not need to be adjusted (since they radiate in all the directions).
                if (pNode->GetLight() && pNode->GetLight()->LightType.Get() == FbxLight::ePoint)
                    pNode->SetPostRotation(FbxNode::eSourcePivot, FbxVector4(0, 0, 0, 0));
            }
            // apply Pre rotations only on bones / end of chains
            if (pNode->GetNodeAttribute() && pNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton
                || (pNode->GetMarker() && pNode->GetMarker()->GetType() == FbxMarker::eEffectorFK)
                || (pNode->GetMarker() && pNode->GetMarker()->GetType() == FbxMarker::eEffectorIK))
            {
                if (pNode->GetRotationActive())
                    pNode->SetPreRotation(FbxNode::eDestinationPivot, pNode->GetPreRotation(FbxNode::eSourcePivot));

                // No pivots on bones
                pNode->SetRotationPivot(FbxNode::eDestinationPivot, lZero);
                pNode->SetScalingPivot(FbxNode::eDestinationPivot, lZero);
                pNode->SetRotationOffset(FbxNode::eDestinationPivot, lZero);
                pNode->SetScalingOffset(FbxNode::eDestinationPivot, lZero);
            }
            else
            {
                // any other type: no pre-rotation support but...
                pNode->SetPreRotation(FbxNode::eDestinationPivot, lZero);

                // support for rotation and scaling pivots.
                pNode->SetRotationPivot(FbxNode::eDestinationPivot, pNode->GetRotationPivot(FbxNode::eSourcePivot));
                pNode->SetScalingPivot(FbxNode::eDestinationPivot, pNode->GetScalingPivot(FbxNode::eSourcePivot));
                // Rotation and scaling offset are supported
                pNode->SetRotationOffset(FbxNode::eDestinationPivot, pNode->GetRotationOffset(FbxNode::eSourcePivot));
                pNode->SetScalingOffset(FbxNode::eDestinationPivot, pNode->GetScalingOffset(FbxNode::eSourcePivot));
                //
                // If we don't "support" scaling pivots, we can simply do:
                // pNode->SetRotationPivot(FbxNode::eDestinationPivot, lZero);
                // pNode->SetScalingPivot(FbxNode::eDestinationPivot, lZero);
            }

            for (int i = 0; i < pNode->GetChildCount(); ++i)
                SetPivotStateRecursive(pNode->GetChild(i));
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

                //mScene->GetRootNode()->ResetPivotSet(FbxNode::eDestinationPivot); 
                //SetPivotStateRecursive(mScene->GetRootNode());
                //mScene->GetRootNode()->ConvertPivotAnimationRecursive(lAnimStack, FbxNode::eDestinationPivot, 30.0f);

                ProcessAnimation(lAnimStack, start, end, mScene->GetRootNode());

                //mScene->GetRootNode()->ResetPivotSet(FbxNode::eSourcePivot);
            }
        }

        void CFBXLoader::ProcessAnimation(FbxAnimStack* animStack, FbxTime start, FbxTime end, FbxNode* pNode)
        {
            m_AnimationFlag.second = true;
            std::set< FbxTime > keyTimes;
            int nbAnimLayers = animStack->GetMemberCount<FbxAnimLayer>();
            FbxString lOutputString;

            FBX_DATA::AnimationData animationData;

            //create the animation clip name of the animation
            FBX_DATA::AnimationClip& animClip = animationData.animations[animStack->GetName()];

            //resize for each bone in the heirarchy
            animClip.boneAnim.resize(m_Model->mBoneData.mbonehierarchy.size());

            boneindex = 0;
            for (int i = 0; i < nbAnimLayers; ++i)
            {
                FbxAnimLayer* lAnimLayer = animStack->GetMember<FbxAnimLayer>(i);
                ProcessAnimation(lAnimLayer, start, end, pNode, animClip);
            }

            m_Model->mAnimationData.push_back(animationData);
        }

        void CFBXLoader::ProcessAnimation(FbxAnimLayer* animLayer, FbxTime start, FbxTime end, FbxNode* pNode, FBX_DATA::AnimationClip& animClip)
        {
            FbxString lOutputString;
            FbxNodeAttribute* attr = pNode->GetNodeAttribute();
            if (attr && attr->GetAttributeType() && attr->GetAttributeType() == FbxNodeAttribute::eSkeleton)
            {
                FbxAnimCurve* curve = nullptr;
                std::set<FbxTime> time;
                time.insert(start);

                curve = pNode->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
                ProcessAnimation(curve, time, start, end);
                curve = pNode->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
                ProcessAnimation(curve, time, start, end);
                curve = pNode->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
                ProcessAnimation(curve, time, start, end);

                curve = pNode->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
                ProcessAnimation(curve, time, start, end);
                curve = pNode->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
                ProcessAnimation(curve, time, start, end);
                curve = pNode->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
                ProcessAnimation(curve, time, start, end);

                curve = pNode->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
                ProcessAnimation(curve, time, start, end);
                curve = pNode->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
                ProcessAnimation(curve, time, start, end);
                curve = pNode->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
                ProcessAnimation(curve, time, start, end);

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
                }
                time.clear();
                ++boneindex;
            }

            //go through all the children nodes and try to read there curves
            for (int j = 0; j < pNode->GetChildCount(); ++j)
                ProcessAnimation(animLayer, start, end, pNode->GetChild(j), animClip);
        }

        void CFBXLoader::ProcessAnimation(FbxAnimCurve* pCurve, std::set<FbxTime>& time, FbxTime start, FbxTime end)
        {
            if (!pCurve)
                return;

            int lKeyCount = pCurve->KeyGetCount();

            for (int i = 0; i < lKeyCount; ++i)
            {
                FbxAnimCurveKey key = pCurve->KeyGet(i);

                if (key.GetTime() >= start && key.GetTime() <= end)
                    time.insert(key.GetTime());
            }
        }

        void CFBXLoader::ProcessGeometry(FbxNode* pNode, int myindex, int inParentIndex)
        {
            FbxMesh* mesh = pNode->GetMesh();
            bool bMesh = false;
            if (mesh)
            {
                FBX_DATA::MeshData newMesh;

                newMesh.name = pNode->GetName();
                newMesh.mLayout = FBX_DATA::SKINNED;
                newMesh.parentIndex = inParentIndex;
                if ("" == newMesh.name)
                    newMesh.mLayout = FBX_DATA::SKINNED;

                int nodeIdx = -1;

                FbxPose* targetFP = nullptr;
                targetFP = GetAnimPoseAndIdx(pNode, nodeIdx);

                FbxAMatrix  meshTransform;

                if (targetFP)
                {
                    UAssert(-1 != nodeIdx, "Fuck you Fuck you");
                    meshTransform = GetPoseMatrix(targetFP, nodeIdx);
                }
                else
                    meshTransform = GetGlobalDefaultPosition(pNode);

                //Meshes have a separate geometry transform that also needs to be applied
                FbxAMatrix geoTransform = GetGeometryTransformation(pNode);

                meshTransform = meshTransform * geoTransform;
                mConverter->ConvertMatrix(meshTransform);

                ProcessVertices(mesh, &newMesh);
                ProcessNormals(mesh, &newMesh);
                ProcessBinormal(mesh, &newMesh);
                ProcessTangent(mesh, &newMesh);
                ProcessTexcoord(mesh, &newMesh);
                ProcessMaterials(pNode, &newMesh);

                //go through all the control points(verticies) and multiply by the transformation
                for (auto &iter: newMesh.vertices)
                {
                    FbxVector4 vtx;
                    vtx.mData[0] = iter.x;
                    vtx.mData[1] = iter.y;
                    vtx.mData[2] = iter.z;
                    vtx.mData[3] = 0.0f;
                    iter = FBXVectorToXMFLOAT3(Transform(meshTransform, vtx));
                }
                m_Model->mMeshData.push_back(newMesh);

                bMesh = true;
            }
            //go through all the child node and grab there geometry information
            int childCnt = pNode->GetChildCount();
            for (int i = 0; i < childCnt; ++i)
            {
                if (bMesh)
                    ProcessGeometry(pNode->GetChild(i), m_Model->mMeshData.size(), myindex);
                else
                    ProcessGeometry(pNode->GetChild(i), myindex, inParentIndex);
            }
        }

        //getting anim pose
        FbxPose* CFBXLoader::GetAnimPoseAndIdx(FbxNode* pNode, int& index)
        {
            for (auto iter : m_Model->mAnimPose)
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
            UAssertCatchable(pNode,
                "Mesh geometry was null."
                );

            const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
            const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
            const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

            return FbxAMatrix(lT, lR, lS);
        }

        FbxAMatrix CFBXLoader::GetParentTransformation(FbxNode* pParentNode)
        {
            int nodeIdx = -1;

            FbxPose* targetFP = nullptr;
            targetFP = GetAnimPoseAndIdx(pParentNode, nodeIdx);

            FbxAMatrix parentTM;
            if (targetFP)
            {
                UAssert(-1 != nodeIdx, "Fuck you Fuck you");
                parentTM = GetPoseMatrix(targetFP, nodeIdx);
            }
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
        void CFBXLoader::Reconstruct(unsigned int meshIdx, std::vector<ufmt_loader::MeshVertex>& target_mvs, std::vector<unsigned int>& target_mis, const FBX_DATA::MeshData& md)
        {
            unsigned int i = 0;
            for (auto &iter: md.indices)
            {
                ufmt_loader::MeshVertex newMV;

                newMV.pos = md.vertices[iter];

                if (!md.normals.empty())
                {
                    if (md.normalMode == FbxGeometryElement::eByPolygonVertex)
                        newMV.normal = md.normals[i];
                    else if (md.normalMode == FbxGeometryElement::eByControlPoint)
                        newMV.normal = md.normals[iter];
                }

                //if (!md.binormals.empty())
                //{
                //    if (md.binormalMode == FbxGeometryElement::eByPolygonVertex)
                //        newMV.binormal = md.binormals[i];
                //    else if (md.binormalMode == FbxGeometryElement::eByControlPoint)
                //        newMV.binormal = md.binormals[iter];
                //}

                if (!md.tangents.empty())
                {
                    if (md.tangentMode == FbxGeometryElement::eByPolygonVertex)
                        newMV.tangent = md.tangents[i];
                    else if (md.tangentMode == FbxGeometryElement::eByControlPoint)
                        newMV.tangent = md.tangents[iter];
                }

                if (!md.uvs.empty())
                {
                    newMV.uv = md.uvs[i];
                    newMV.uv.y = 1.0f - newMV.uv.y;
                }

                // controls - maybe divide this part later if necessary
                if (!m_Model->mCtrlPoints.empty())
                {
                    if (!m_Model->mCtrlPoints[meshIdx].empty())
                    {
                        // currently, just for using 1st control point vec
                        newMV.ctrlBlendWeights.x = m_Model->mCtrlPoints[meshIdx].at(iter).mBlendingInfo[0].mBlendingWeight;
                        newMV.ctrlBlendWeights.y = m_Model->mCtrlPoints[meshIdx].at(iter).mBlendingInfo[1].mBlendingWeight;
                        newMV.ctrlBlendWeights.z = m_Model->mCtrlPoints[meshIdx].at(iter).mBlendingInfo[2].mBlendingWeight;
                        newMV.ctrlBlendWeights.w = m_Model->mCtrlPoints[meshIdx].at(iter).mBlendingInfo[3].mBlendingWeight;

                        newMV.ctrlIndices.x = m_Model->mCtrlPoints[meshIdx].at(iter).mBlendingInfo[0].mBlendingIndex;
                        newMV.ctrlIndices.y = m_Model->mCtrlPoints[meshIdx].at(iter).mBlendingInfo[1].mBlendingIndex;
                        newMV.ctrlIndices.z = m_Model->mCtrlPoints[meshIdx].at(iter).mBlendingInfo[2].mBlendingIndex;
                        newMV.ctrlIndices.w = m_Model->mCtrlPoints[meshIdx].at(iter).mBlendingInfo[3].mBlendingIndex;
                    }
                }

                bool bFound = false;
                unsigned int index = 0;
                for (index = 0; index < target_mvs.size(); ++index)
                {
                    if (newMV == target_mvs[index])
                    {
                        bFound = true;
                        // if there is same MeshVertex, just add the index of it.
                        target_mis.push_back(index);
                        break;
                    }
                }

                // if there is no MeshVertex, store the vertex and set a index as the last one
                if (!bFound)
                {
                    target_mvs.push_back(newMV);
                    target_mis.push_back(index);
                }

                ++i;
            }
        }
    }
}
