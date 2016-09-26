///* ---------------------------------------------------------------------------
//** Team Bear King
//** ?2015 DigiPen Institute of Technology, All Rights Reserved.
//**
//** CFBXLoader.h
//**
//** Author:
//** - Park Hyung Jun - park.hyungjun@digipen.edu
//**
//** Contributors:
//** - Hiago Desena - hiago.desena@digipen.edu
//** -------------------------------------------------------------------------*/
//
//#pragma once
//// For Parsing FBX infos
//#include <ModelInfo.h>
//#include <AnimationInfo.h>
//#include <Utilities.h>
//#include <Conversion.h>
//
//using namespace Utilities;
//
//namespace ursine
//{
//    namespace graphics
//    {
//        class CFBXLoader
//        {
//        public:
//            enum eAXIS_SYSTEM
//            {
//                eAXIS_DIRECTX = 0,
//                eAXIS_OPENGL,
//            };
//
//        protected:
//            // FBX SDK
//            FbxManager*		mSdkManager;
//            FbxScene*		mScene;
//            FbxImporter*	mImporter;
//            Converter*		mConverter;
//
//            void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene);
//            void TriangulateRecursive(FbxNode* pNode);
//
//        public:
//            /** @brief fbx loader constructor
//            *
//            *  this will construct the fbx loader
//            *
//            *  @return nothing
//            */
//            CFBXLoader();
//
//            /** @brief fbx loader destructor
//            *
//            *  this will destroy the fbx loader
//            *
//            *  @return nothing
//            */
//            ~CFBXLoader();
//
//            /** @brief fbx loader export function
//            *
//            *  this will build up custom file datastructure from fbx
//            *
//            *  @return if success return true, else return false
//            */
//            bool ReadyToExport();
//
//            /** @brief fbx loader file loading function
//            *
//            *  this will import data from fbx file, and saving it into the data structure
//            *
//            *  @param filename name of the fbx file
//            *  @return if success return true, else return false
//            */
//            bool LoadFBX(const std::string &filename);
//
//            /** @brief fbx loader file processing scene function
//            *
//            *  this will import scene data from fbx file, and saving it into the data structure
//            *
//            *  @param pRoot the root of the FbxScene
//            *  @return nothing
//            */
//            void ProcessScene(FbxNode* pRoot);
//
//            /** @brief fbx loader processing skeleton hierarchy function
//            *
//            *  this will import bone hierarchy data from fbx file, and saving it into the data structure
//            *
//            *  @param pRoot the root of the FbxScene
//            *  @return nothing
//            */
//            void ProcessSkeletonHierarchy(FbxNode* pRoot);
//
//            /** @brief fbx loader processing skeleton hierarchy auxiliary function
//            *
//            *  this will help to import bone hierarchy data from fbx file, and saving it into the data structure
//            *
//            *  @param pNode the pointer of the FbxNode
//            *  @param inDepth the integer of the hierarchy level.(Root = -1)
//            *  @param inParentIndex the index of the parent node's(-1 if there is no parent)
//            *  @return nothing
//            */
//            void ProcessSkeletonHierarchyRecursively(FbxNode* pNode, int myIndex, int inParentIndex);
//
//            /** @brief fbx loader static mesh(no animation mesh) funciton
//            *
//            *  this will build up static mesh from the fbx file
//            *
//            *  @param pNode the pointer of the FbxNode
//            *  @return nothing
//            */
//            void ProcessStaticMesh(FbxNode* pNode, int myindex, int inParentIndex);
//
//            /** @brief fbx loader vertices funciton
//            *
//            *  this will import vertices data from fbx file
//            *
//            *  @param pNode the pointer of the FbxNode
//            *  @return nothing
//            */
//            void ProcessVertices(FbxMesh* pMesh, FBX_DATA::MeshData* pData);
//
//            /** @brief fbx loader normal funciton
//            *
//            *  this will import normal data from fbx file
//            *
//            *  @param pNode the pointer of the FbxNode
//            *  @return nothing
//            */
//            void ProcessNormals(FbxMesh* pMesh, FBX_DATA::MeshData* pData);
//
//            /** @brief fbx loader binormal funciton
//            *
//            *  this will import binormal data from fbx file
//            *
//            *  @param pNode the pointer of the FbxNode
//            *  @return nothing
//            */
//            void ProcessBinormal(FbxMesh* pMesh, FBX_DATA::MeshData* pData);
//
//            /** @brief fbx loader tangent funciton
//            *
//            *  this will import tangent data from fbx file
//            *
//            *  @param pNode the pointer of the FbxNode
//            *  @return nothing
//            */
//            void ProcessTangent(FbxMesh* pMesh, FBX_DATA::MeshData* pData);
//
//            /** @brief fbx loader texcoord funciton
//            *
//            *  this will import uv coordinates data from fbx file
//            *
//            *  @param pNode the pointer of the FbxNode
//            *  @return nothing
//            */
//            void ProcessTexcoord(FbxMesh* pMesh, FBX_DATA::MeshData* pData);
//
//            /** @brief fbx loader get material property funciton
//            *
//            *  this will import material property data from fbx file
//            *
//            *  @param pMaterial the pointer of the FbxSurfaceMaterial
//            *  @param pPropertyName the name of the property
//            *  @param pFactorPropertyName the name of the factor property
//            *  @param pElement the pointer of the material element
//            *  @return FbxDouble3 type of material property(diffuse, specular, ambient, emissive)
//            */
//            FbxDouble3 GetMaterialProperty(
//                const FbxSurfaceMaterial * pMaterial,
//                const char * pPropertyName,
//                const char * pFactorPropertyName,
//                FBX_DATA::Material_Eles* pElement);
//
//            /** @brief fbx loader material funciton
//            *
//            *  this will import material data from fbx file
//            *
//            *  @param pNode the pointer of the FbxNode
//            *  @param pData the pointer of the mesh data structure
//            *  @return nothing
//            */
//            void ProcessMaterials(FbxNode* pNode, FBX_DATA::MeshData* pData);
//
//            /** @brief fbx loader weight blend funciton
//            *
//            *  this will import blending weight for control points from fbx file
//            *
//            *  @param pNode the pointer of the FbxNode
//            *  @param index the reference of the control point
//            *  @return nothing
//            */
//            void ProcessWeightBlend(FbxNode* pNode, unsigned int& index);
//
//            /** @brief fbx loader remove unncessary weight funciton
//            *
//            *  sort and remove unnecessary blending weight more than 4
//            *
//            *  @return nothing
//            */
//            void RemoveUnnecessaryWeights();
//
//            /** @brief fbx loader get joint index by name funciton
//            *
//            *  find joint by name
//            *
//            *  @param inJointName the name of the joint
//            *  @return index of the joint
//            */
//            int GetJointIndexByName(const std::string& inJointName);
//
//            /** @brief fbx loader get pose matrix funciton
//            *
//            *  find animation bind pose matrix of the node in the bind pose array
//            *
//            *  @param pose the array of the animation bind pose
//            *  @param index of the specific node in the list
//            *  @return the animation bind pose matrix of the node
//            */
//            FbxAMatrix GetPoseMatrix(FbxPose* pose, int index);
//
//            /** @brief fbx loader check negative scale funciton
//            *
//            *  check if the scale is positive or negative
//            *
//            *  @param scale the scale vector
//            *  @return if scale is not positive return false, else return true
//            */
//            bool CheckPositive(FbxVector4 scale);
//
//            /** @brief fbx loader check scale factor funciton
//            *
//            *  check if the scale is close to epsilon value
//            *
//            *  @param scale the scale vector
//            *  @return if scale is not positive return false, else return true
//            */
//            bool CheckScaling(FbxVector4 scale);
//
//            void SetPivotStateRecursive(FbxNode* pNode);
//
//            /** @brief fbx loader animation funciton
//            *
//            *  this will import animation data by searching whole fbx animation data
//            *
//            *  @return nothing
//            */
//            void ProcessAnimation();
//
//            /** @brief fbx loader animation auxiliary funciton
//            *
//            *  this will import animation data by searching animation stack
//            *
//            *  @param animStack the animation stack
//            *  @param start the starting time
//            *  @param end the end time
//            *  @return nothing
//            */
//            void ProcessAnimation(FbxAnimStack* animStack, FbxTime start, FbxTime end, FbxNode* pNode);
//
//            /** @brief fbx loader animation auxiliary funciton
//            *
//            *  this will import animation keyframe data by searching animation layer
//            *
//            *  @param animLayer the animation layer
//            *  @param start the starting time
//            *  @param pNode the pointer of the node
//            *  @param animData the reference of each animation clip
//            *  @return nothing
//            */
//            void ProcessAnimation(FbxAnimLayer* animLayer, FbxTime start, FbxTime end, FbxNode* pNode, FBX_DATA::AnimationClip& animData);
//
//            /** @brief fbx loader animation auxiliary funciton
//            *
//            *  this will import animation keyframe time data by searching animation curve
//            *
//            *  @param pCurve the animation curve
//            *  @param time the set of times in each animation curve
//            *  @return nothing
//            */
//            void ProcessAnimation(FbxAnimCurve* pCurve, std::set<FbxTime>& time, FbxTime start, FbxTime end);
//
//            /** @brief fbx loader geometry funciton
//            *
//            *  this will import geometric data from each fbx node
//            *
//            *  @param pNode the pointer of the node
//            *  @return nothing
//            */
//            void ProcessGeometry(FbxNode* pNode, int myindex, int inParentIndex);
//
//            /** @brief fbx loader get animation pose and index funciton
//            *
//            *  this will get animation bind pose matrix and its index of the node in the bind pose array
//            *
//            *  @param pNode the pointer of the node
//            *  @param index the reference of the node
//            *  @return FbxPose type data structure of the specific node
//            */
//            FbxPose* GetAnimPoseAndIdx(FbxNode* pNode, int& index);
//
//            /** @brief fbx loader get local matrix from time function
//            *
//            *  this will get the local matrix of the node according to time
//            *
//            *  @param pNode the pointer of the node
//            *  @param keyTime the passed time for the node
//            *  @return FbxAMatrix type matrix of the node's local matrix
//            */
//            FbxAMatrix GetLocalMatrixFromTime(FbxNode* pNode, FbxTime keyTime);
//
//            /** @brief fbx loader get default global matrix function
//            *
//            *  this will get the default global matrix of the node
//            *
//            *  @param pNode the pointer of the node
//            *  @return FbxAMatrix type matrix of the node's default global matrix
//            */
//            FbxAMatrix GetGlobalDefaultPosition(FbxNode* pNode);
//
//            /** @brief fbx loader get geometry matrix function
//            *
//            *  this will get the geometry matrix of the node
//            *
//            *  @param pNode the pointer of the node
//            *  @return FbxAMatrix type matrix of the node
//            */
//            FbxAMatrix GetGeometryTransformation(FbxNode* pNode);
//
//            /** @brief fbx loader get parent matrix function
//            *
//            *  this will get the matrix of the node's parent
//            *
//            *  @param pParentNode the pointer of the node
//            *  @return FbxAMatrix type matrix of the node's parent matrix
//            */
//            FbxAMatrix GetParentTransformation(FbxNode* pParentNode);
//
//            /** @brief transform vector multiply and normalize by matrix using fbx sdk supported function
//            *
//            *  this will get transformed vector
//            *
//            *  @param pAMatrix the transformation matrix
//            *  @param point the pointer
//            *  @return FbxVector4 type transformed vector
//            */
//            FbxVector4 Transform(const FbxAMatrix& pAMatrix, const FbxVector4& point);
//
//            void TransformWithInverseMeshTM(std::vector<pseudodx::XMFLOAT3> &Float3Vec, const FbxAMatrix &mat);
//
//            void CFBXLoader::Reconstruct(unsigned int meshIdx, std::vector<ufmt_loader::MeshVertex>& mvVec, std::vector<unsigned int>& miVec, const FBX_DATA::MeshData& md);
//
//            /** @brief fbx loader get mesh data function
//            *
//            *  this will get the mesh data of the model
//            *
//            *  @param index the id of the mesh data
//            *  @return the mesh data of the model accoring to its index
//            */
//            FBX_DATA::MeshData& GetMeshData(int index) const { return (m_Model->mMeshData[index]); }
//
//            /** @brief fbx loader get mesh data count function
//            *
//            *  this will get the number of mesh data
//            *
//            *  @return the size of the mesh data of the model
//            */
//            size_t GetMeshDataCount() const { return m_Model->mMeshData.size(); }
//
//            /** @brief fbx loader get model function
//            *
//            *  this will get the model data of the fbx file
//            *
//            *  @return the model data structure
//            */
//            FBX_DATA::FbxModel& GetModel() const { return *m_Model; }
//
//            /** @brief fbx loader is skinned function
//            *
//            *  check if the model is skinned model or just static mesh
//            *
//            *  @return if there is animation return true, else return false
//            */
//            bool IsSkinned() { return (m_Model->mAnimationData.empty()) ? false : true; }
//
//            /** @brief fbx loader get model info function
//            *
//            *  this will get the model info
//            *
//            *  @return the model info data structure
//            */
//            const ufmt_loader::ModelInfo& GetModelInfo() const { return m_ModelInfo; }
//
//            /** @brief fbx loader get animation info function
//            *
//            *  this will get the animation info(each mesh's transform matrices)
//            *
//            *  @return the animation info data structure
//            */
//            const ufmt_loader::AnimInfo& GetAnimInfo() const { return m_AnimInfo; }
//
//        private:
//            unsigned int            boneindex;
//            std::pair<bool, bool>   m_AnimationFlag;	// first flag = if there is animation
//                                                        // second flag = if there is only animation
//            FbxPose*                m_AnimPose;
//            FBX_DATA::FbxModel*     m_Model;
//
//            ufmt_loader::ModelInfo  m_ModelInfo;
//            ufmt_loader::AnimInfo   m_AnimInfo;
//        };
//    }
//}	// FBX_LOADER