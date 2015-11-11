#pragma once

#include "WholeInformation.h"

using namespace DirectX;

namespace ursine
{
	namespace graphics
	{
		namespace ufmt_loader
		{
			// UVSet
			typedef std::tr1::unordered_map<std::string, int> UVsetID;
			// UVSet
			typedef std::tr1::unordered_map<std::string, std::vector<std::string>> TextureSet;

			enum eLayout
			{
				NONE = -1,
				STATIC = 0,
				INSTANCE = 0,
				SKINNED,
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
				BYTE		vBIdx[4];
			};

			struct Material_Consts
			{
				XMFLOAT4	ambient;
				XMFLOAT4	diffuse;
				XMFLOAT4	specular;
				XMFLOAT4	emissive;
				float		shineness;
				float		TransparencyFactor;
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

			bool compare_bw_ascend(BlendIdxWeight lhs, BlendIdxWeight rhs);
			bool compare_bw_descend(BlendIdxWeight lhs, BlendIdxWeight rhs);

			// Each Control Point in FBX is basically a vertex  in the physical world. For example, a cube has 8
			// vertices(Control Points) in FBX Joints are associated with Control Points in FBX
			// The mapping is one joint corresponding to 4 Control Points(Reverse of what is done in a game engine)
			// As a result, this struct stores a XMFLOAT3 and a vector of joint indices
			struct CtrlPoint
			{
				XMFLOAT3 mPosition;
				std::vector<BlendIdxWeight> mBlendingInfo;
			};
			// Control Points
			typedef std::unordered_map<unsigned int, CtrlPoint*> ControlPoints;

			struct KeyFrame
			{
				float time;
				XMFLOAT3 trans;
				XMFLOAT4 rot;
				XMFLOAT3 scl;
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

			struct Material_Eles
			{
				// determine if material only holds material or only textures
				// or both
				enum eMaterial_Fac
				{
					Fac_None = 0,
					Fac_Only_Color,
					Fac_Only_Texture,
					Fac_Both,
					Fac_Max,
				};
				eMaterial_Fac type;
				XMFLOAT4 color;
				TextureSet textureSetArray;

				Material_Eles()
					:type(Fac_None), color(0, 0, 0, 1)
				{
					textureSetArray.clear();
				}

				~Material_Eles()
				{
					Release();
				}

				void Release()
				{
					for (TextureSet::iterator it = textureSetArray.begin(); it != textureSetArray.end(); ++it)
					{
						it->second.clear();
					}
					textureSetArray.clear();
				}

				Material_Eles& operator=(const Material_Eles& rhs)
				{
					type = rhs.type;
					color = rhs.color;

					for (auto iter = rhs.textureSetArray.begin(); iter != rhs.textureSetArray.end(); ++iter)
					{
						for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2)
						{
							textureSetArray[iter->first].push_back(*iter2);
						}
					}
					return *this;
				}
			};

			// structure for storing material and texture's'
			// this will be used to export info as JMDL
			// Currently, not handling subset or submaterial
			struct FbxMaterial
			{
				enum eMaterial_Type
				{
					Type_None = 0,
					Type_Lambert,
					Type_Phong,
					Type_Max
				};

				std::string  name;
				eMaterial_Type type;
				// ambiet material and texture
				Material_Eles ambient;
				// diffuse material and texture
				Material_Eles diffuse;
				// emmisive material and texture
				Material_Eles emissive;
				// specular material and texture
				Material_Eles specular;
				float shineness;
				float TransparencyFactor;
				Material_Consts mtrl_consts;

				FbxMaterial()
					:name(""), type(Type_None),
					shineness(0), TransparencyFactor(0)
				{}

				void Release()
				{
					ambient.Release();
					diffuse.Release();
					emissive.Release();
					specular.Release();
				}

				FbxMaterial& operator=(const FbxMaterial& rhs)
				{
					name = rhs.name;
					type = rhs.type;
					ambient = rhs.ambient;
					diffuse = rhs.diffuse;
					emissive = rhs.emissive;
					specular = rhs.specular;
					shineness = rhs.shineness;
					TransparencyFactor = rhs.TransparencyFactor;

					mtrl_consts.ambient = rhs.ambient.color;
					mtrl_consts.diffuse = rhs.diffuse.color;
					mtrl_consts.emissive = rhs.emissive.color;
					mtrl_consts.specular = rhs.specular.color;
					mtrl_consts.shineness = rhs.shineness;
					mtrl_consts.TransparencyFactor = rhs.TransparencyFactor;
					return *this;
				}
			};

			// structure which will be used to pass data to shaer
			// this will replace MATERIAL_DATA
			struct Material_Data
			{
				FbxMaterial*				fbxmaterial;
				ID3D11ShaderResourceView*	pSRV;
				ID3D11SamplerState*         pSampler;
				ID3D11Buffer*				pMaterialCb;

				Material_Data()
				{
					fbxmaterial = nullptr;
					pSRV = nullptr;
					pSampler = nullptr;
					pMaterialCb = nullptr;
				}
				void Release()
				{
					if (fbxmaterial)
					{
						fbxmaterial->Release();
						fbxmaterial = nullptr;
					}

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

			// This is the actual representation of a joint in a game engine
			struct Joint
			{
				std::string mName;
				int mParentIndex;
				FbxAMatrix mToRoot;
				FbxAMatrix mToParent;

				// bind - local coord system that the entire skin is defined relative to
				// local tm. local about to the skinned mesh
				XMFLOAT3 bindPosition;
				XMFLOAT4 bindRotation;
				XMFLOAT3 bindScaling;

				// bone space - the space that influences the vertices. so-called offset transformation
				// bone offset tm
				XMFLOAT3 boneSpacePosition;
				XMFLOAT4 boneSpaceRotation;
				XMFLOAT3 boneSpaceScaling;

				Joint() : mParentIndex(-1),
					bindPosition(0, 0, 0), bindRotation(0, 0, 0, 1), bindScaling(1, 1, 1),
					boneSpacePosition(0, 0, 0), boneSpaceRotation(0, 0, 0, 1), boneSpaceScaling(1, 1, 1)
				{
					mToRoot.SetIdentity();
					mToParent.SetIdentity();
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
				eLayout mLayout;
				std::string name;
				unsigned int vertexCnt;
				unsigned int indexCnt;
				unsigned int normalCnt;
				unsigned int tangentCnt;
				unsigned int uvCnt;
				unsigned int mtrlIndexCnt;

				FbxLayerElement::EMappingMode normalMode;
				FbxLayerElement::EMappingMode tangentMode;
				XMMATRIX meshTM;

				XMFLOAT3* vertices;
				unsigned int* indices;
				XMFLOAT3* normals;
				XMFLOAT3* tangents;
				XMFLOAT2* uvs;
				unsigned int* materialIndices;

				// material
				std::vector<FbxMaterial> fbxmaterials;
				std::vector<ModelSubset> modelSubsets;

				MeshData() : mLayout(NONE), vertexCnt(0), indexCnt(0), normalCnt(0), tangentCnt(0), uvCnt(0),
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
				std::vector<FbxMaterial*> mMaterials;
				std::vector<ControlPoints*> mCtrlPoints;
				std::vector<AnimationData*> mAnimationData;
				// ====================================

				FbxModel() :mAnimPose(nullptr), mLayout(NONE) {}
				~FbxModel()
				{
					Release();
				}
				void Release()
				{
					mMeshData.clear();
					mMaterials.clear();
					mCtrlPoints.clear();
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
			XMVECTOR SetFloat3ToXMVector(const XMFLOAT4& rhs);
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
			XMFLOAT4 FBXDouble3ToXMFLOAT4(const FbxDouble3& src);
			XMFLOAT3 FBXVectorToXMFLOAT3(const FbxVector4& src);
			FbxVector2 XMFloat2ToFBXVector2(const XMFLOAT2& src);
			XMFLOAT4 FBXQuaternionToXMLOAT4(const FbxQuaternion& quat);
			XMFLOAT3 FBXVectorToXMFLOAT3(const FbxVector4& src);
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