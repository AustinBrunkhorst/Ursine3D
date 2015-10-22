#pragma once

#include "WholeInformation.h"

using namespace DirectX;

namespace ursine
{
	namespace graphics
	{
		namespace FBX_DATA
		{
			// layouts
			enum eLayout
			{
				NONE = -1,
				STATIC,
				SKINNED
			};

			struct LAYOUT
			{
				D3D11_INPUT_ELEMENT_DESC STATIC_LAYOUT[3];
				D3D11_INPUT_ELEMENT_DESC SKINNED_LAYOUT[5];

				LAYOUT()
				{
					STATIC_LAYOUT[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
					STATIC_LAYOUT[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
					STATIC_LAYOUT[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };

					SKINNED_LAYOUT[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
					SKINNED_LAYOUT[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
					SKINNED_LAYOUT[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
					SKINNED_LAYOUT[3] = { "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
					SKINNED_LAYOUT[4] = { "BLENDINDICES", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
				}
			};

			struct VERTEX_DATA_STATIC
			{
				XMFLOAT3	vPos;
				XMFLOAT3	vNor;
				XMFLOAT2	vTexcoord;
			};

			struct VERTEX_DATA_SKIN
			{
				XMFLOAT3	vPos;
				XMFLOAT3	vNor;
				XMFLOAT2	vTexcoord;
				XMFLOAT4	vBWeight;
				XMINT4		vBIdx;
			};

			struct MATERIAL_CONSTANT_DATA
			{
				XMFLOAT4	ambient;
				XMFLOAT4	diffuse;
				XMFLOAT4	specular;
				XMFLOAT4	emmisive;
			};

			struct MATERIAL_DATA
			{
				XMFLOAT4	ambient;
				XMFLOAT4	diffuse;
				XMFLOAT4	specular;
				XMFLOAT4	emmisive;
				float specularPower;
				float TransparencyFactor;

				MATERIAL_CONSTANT_DATA materialConstantData;

				ID3D11ShaderResourceView*	pSRV;
				ID3D11SamplerState*         pSampler;
				ID3D11Buffer*				pMaterialCb;

				MATERIAL_DATA()
				{
					pSRV = nullptr;
					pSampler = nullptr;
					pMaterialCb = nullptr;
				}

				void Release()
				{
					if (pMaterialCb)
					{
						pMaterialCb->Release();
						pMaterialCb = nullptr;
					}

					if (pSRV)
					{
						pSRV->Release();
						pSRV = nullptr;
					}

					if (pSampler)
					{
						pSampler->Release();
						pSampler = nullptr;
					}
				}
			};

			struct BlendIdxWeight
			{
				unsigned int mBlendingIndex;
				double mBlendingWeight;

				BlendIdxWeight() :
					mBlendingIndex(0),
					mBlendingWeight(0)
				{}

				bool operator<(const BlendIdxWeight& rhs)
				{
					return mBlendingWeight < rhs.mBlendingWeight;
				}
			};

			// Each Control Point in FBX is basically a vertex  in the physical world. For example, a cube has 8
			// vertices(Control Points) in FBX Joints are associated with Control Points in FBX
			// The mapping is one joint corresponding to 4 Control Points(Reverse of what is done in a game engine)
			// As a result, this struct stores a XMFLOAT3 and a vector of joint indices
			struct CtrlPoint
			{
				XMFLOAT3 mPosition;
				std::vector<BlendIdxWeight> mBlendingInfo;
			};

			typedef std::unordered_map<unsigned int, CtrlPoint*> ControlPoints;

			struct KeyFrame
			{
				float time;
				XMFLOAT4 trans;
				XMFLOAT4 rot;
			};

			struct BoneAnimation
			{
				std::vector<KeyFrame> keyFrames;
			};

			struct AnimationClip
			{
				// animation of each bones
				std::vector<BoneAnimation> boneAnim;
				void Interpolate(double timePos, std::vector<XMMATRIX>& toParentTMs);
			};

			struct AnimationData
			{
				std::unordered_map<std::string, AnimationClip> animations;
			};

			struct Material
			{
				float specularPower;
				float TransparencyFactor;
				std::string  diffMapName;
				std::string  normMapName;
				std::string  specMapName;
				std::string  emitMapName;
				std::string  name;
			};

			// This is the actual representation of a joint in a game engine
			struct Joint
			{
				std::string mName;
				int mParentIndex;
				FbxAMatrix mToRoot;
				FbxAMatrix mToParent;

				// bind - local coord system that the entire skin is defined relative to
				// local tm. local about to the skinned mesh
				XMFLOAT4 bindPosition;
				XMFLOAT4 bindRotation;

				// bone space - the space that influences the vertices. so-called offset transformation
				// bone offset tm
				XMFLOAT4 boneSpacePosition;
				XMFLOAT4 boneSpaceRotation;

				Joint()
				{
					mToRoot.SetIdentity();
					mToParent.SetIdentity();
					mParentIndex = -1;
				}

				~Joint()
				{
				}
			};

			struct FbxSkinnedData
			{
				std::vector<Joint>			mbonehierarchy;
				std::vector<FbxNode*>		mboneNodes;
				~FbxSkinnedData()
				{
					Release();
				}
				void Release()
				{
					mbonehierarchy.clear();
					mboneNodes.clear();
				}
			};

			struct ModelSubset
			{
				ModelSubset() : id(-1), vertexStart(0), vertexCount(0) {};
				//id number of this subset
				int         id;

				//location of where this subset starts and end
				unsigned    vertexStart;
				unsigned    vertexCount;
			};

			struct MeshData
			{
				std::string name;
				unsigned int vertexCnt;
				unsigned int indexCnt;
				unsigned int normalCnt;
				unsigned int tangentCnt;
				unsigned int uvCnt;
				unsigned int mtrlIndexCnt;

				FbxLayerElement::EMappingMode normalMode;
				FbxLayerElement::EMappingMode tangentMode;

				XMFLOAT3* vertices;
				unsigned int* indices;
				XMFLOAT3* normals;
				XMFLOAT3* tangents;
				XMFLOAT2* uvs;
				unsigned int* materialIndices;

				// material
				std::vector<Material> materials;
				std::vector<ModelSubset> modelSubsets;

				MeshData() : vertexCnt(0), indexCnt(0), normalCnt(0), tangentCnt(0), uvCnt(0),
					normalMode(FbxLayerElement::eNone), tangentMode(FbxLayerElement::eNone),
					vertices(nullptr), indices(nullptr), normals(nullptr), tangents(nullptr), uvs(nullptr)
				{}
			};

			struct FbxModel
			{
				FbxPose*				mAnimPose;

				// ===== Data we need to export =======
				// need to be exported as binary
				eLayout					mLayout;
				std::string				name;
				FbxSkinnedData			mSkinnedData;
				std::vector<MeshData*>	mMeshData;
				std::vector<ControlPoints*> mCtrlPoints;
				std::vector<AnimationData*> mAnimationData;
				// ====================================

				FbxModel() :mAnimPose(nullptr) {}
				~FbxModel()
				{
					Release();
				}
				void Release()
				{
					mMeshData.clear();
					mAnimationData.clear();
				}
				void GetFinalTransform(const std::string& clipName, double timePos, std::vector<XMMATRIX>& finalTransform) const;
			};
		}

		namespace Utilities
		{
			/*===============================
			Utility Functions for FBX
			===============================*/
			XMVECTOR Set4FloatsToXMVector(const float& x, const float& y, const float& z, const float& w);
			XMVECTOR SetFloat4ToXMVector(const XMFLOAT4& rhs);
			XMFLOAT4 SetXMVectorToFloat4(const XMVECTOR& rhs);
			FbxAMatrix GetGeometryTransformation(FbxNode* inNode);
			FbxAMatrix FBXMatrixToFBXAMatrix(FbxMatrix* src);
			XMMATRIX FBXMatrixToXMMatrix(FbxAMatrix* src);
			XMMATRIX FBXAMatrixToXMMatrix(FbxAMatrix* src);
			void FBXMatrixToFloat16(FbxMatrix* src, float dest[16]);
			void FBXAMatrixToFloat16(FbxAMatrix* src, float dest[16]);
			void XMMatrixToFloat16(XMMATRIX& src, float dest[16]);
			XMVECTOR ConvertVector4(const FbxVector4& vec);
			XMVECTOR ConvertQuaternion(const FbxQuaternion& quat);
			FbxVector4 XMVectorToFBXVector(const XMVECTOR& src);
			FbxVector4 XMFloat3ToFBXVector4(const XMFLOAT3& src);
			XMFLOAT3 FBXVectorToXMFLOAT3(const FbxVector4& src);
			FbxVector2 XMFloat2ToFBXVector2(const XMFLOAT2& src);
			XMFLOAT4 FBXQuaternionToXMLOAT4(const FbxQuaternion& quat);
			XMFLOAT4 FBXVectorToXMFLOAT4(const FbxVector4& src);
			bool HJIsZeroMtx(XMMATRIX* _pMtx);
			bool HJIsSameMtx(XMMATRIX* _pMtx1, XMMATRIX* _pMtx2);
			bool HJIsIdentityMtx(XMMATRIX* _pMtx);
			int InterpolationFlagToIndex(int flags);
			int ConstantmodeFlagToIndex(int flags);
			int TangentmodeFlagToIndex(int flags);
			int TangentweightFlagToIndex(int flags);
			int TangentVelocityFlagToIndex(int flags);
		};
	};
};