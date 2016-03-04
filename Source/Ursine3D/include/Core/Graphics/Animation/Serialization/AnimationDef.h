/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationDef.h
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
**
** Contributors:
** - Matt Yan - m.yan@digipen.edu
** -------------------------------------------------------------------------*/

#pragma once

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <set>
#include <fbxsdk.h>
#include <iostream>
#include <SMat4.h>

namespace pseudodx
{
    struct XMUINT4
    {
        uint32_t x;
        uint32_t y;
        uint32_t z;
        uint32_t w;
        XMUINT4() {}
        XMUINT4(uint32_t _x, uint32_t _y, uint32_t _z, uint32_t _w) : x(_x), y(_y), z(_z), w(_w) {}
        XMUINT4& operator= (const XMUINT4& Uint4) { x = Uint4.x; y = Uint4.y; z = Uint4.z; w = Uint4.w; return *this; }
    };

    struct XMFLOAT4
    {
        float x;
        float y;
        float z;
        float w;
        XMFLOAT4() {}
        XMFLOAT4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
        XMFLOAT4& operator= (const XMFLOAT4& Float4) { x = Float4.x; y = Float4.y; z = Float4.z; w = Float4.w; return *this; }
    };

    struct XMFLOAT3
    {
        float x;
        float y;
        float z;

        XMFLOAT3() {}
        XMFLOAT3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
        XMFLOAT3& operator= (const XMFLOAT3& Float3) { x = Float3.x; y = Float3.y; z = Float3.z; return *this; }
    };

    struct XMFLOAT2
    {
        float x;
        float y;

        XMFLOAT2() {}
        XMFLOAT2(float _x, float _y) : x(_x), y(_y) {}
        XMFLOAT2& operator= (const XMFLOAT2& Float2) { x = Float2.x; y = Float2.y; return *this; }
    };
}

namespace ursine
{
    namespace graphics
    {
        namespace FBX_DATA
        {
            // UVSet
            typedef std::tr1::unordered_map<std::string, int> UVsetID;
            // UVSet
            typedef std::tr1::unordered_map<std::string, std::vector<std::string>> TextureSet;

            // layout
            enum eLayout
            {
                NONE = -1,
                STATIC = 0,
                INSTANCE = 0,
                SKINNED,
            };

            struct Material_Consts
            {
                pseudodx::XMFLOAT4	ambient;
                pseudodx::XMFLOAT4	diffuse;
                pseudodx::XMFLOAT4	specular;
                pseudodx::XMFLOAT4	emissive;
                float		shineness;
                float		TransparencyFactor;
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
                pseudodx::XMFLOAT4 color;
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

            struct KeyFrame
            {
                float time;
                pseudodx::XMFLOAT3 trans;
                pseudodx::XMFLOAT4 rot;
                pseudodx::XMFLOAT3 scl;
            };

            struct BoneAnimation
            {
                std::vector<KeyFrame> keyFrames;
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

            static bool compare_bw_ascend(BlendIdxWeight lhs, BlendIdxWeight rhs)
            {
                return lhs.mBlendingWeight < rhs.mBlendingWeight;
            }

            static bool compare_bw_descend(BlendIdxWeight lhs, BlendIdxWeight rhs)
            {
                return lhs.mBlendingWeight > rhs.mBlendingWeight;
            }

            // Each Control Point in FBX is basically a vertex  in the physical world. For example, a cube has 8
            // vertices(Control Points) in FBX Joints are associated with Control Points in FBX
            // The mapping is one joint corresponding to 4 Control Points(Reverse of what is done in a game engine)
            // As a result, this struct stores a XMFLOAT3 and a vector of joint indices
            struct CtrlPoint
            {
                pseudodx::XMFLOAT3 mPosition;
                std::vector<BlendIdxWeight> mBlendingInfo;
            };

            // Control Points
            typedef std::unordered_map<unsigned int, CtrlPoint*> ControlPoints;

            struct AnimationClip
            {
                // animation of each bones
                std::vector<BoneAnimation> boneAnim;
            };

            struct AnimationData
            {
                std::unordered_map<std::string, AnimationClip> animations;
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
                pseudodx::XMFLOAT3 bindPosition;
                pseudodx::XMFLOAT3 bindScaling;
                pseudodx::XMFLOAT4 bindRotation;

                // bone space - the space that influences the vertices. so-called offset transformation
                // bone offset tm
                pseudodx::XMFLOAT3 boneSpacePosition;
                pseudodx::XMFLOAT3 boneSpaceScaling;
                pseudodx::XMFLOAT4 boneSpaceRotation;

                Joint()
                {
                    mParentIndex = -1;
                    bindPosition = pseudodx::XMFLOAT3(0, 0, 0);
                    bindRotation = pseudodx::XMFLOAT4(0, 0, 0, 1);
                    bindScaling = pseudodx::XMFLOAT3(1, 1, 1);
                    boneSpacePosition = pseudodx::XMFLOAT3(0, 0, 0);
                    boneSpaceRotation = pseudodx::XMFLOAT4(0, 0, 0, 1);
                    boneSpaceScaling = pseudodx::XMFLOAT3(1, 1, 1);
                    mToRoot.SetIdentity();
                    mToParent.SetIdentity();
                }
            };

            struct FbxBoneData
            {
                std::vector<Joint>		mbonehierarchy;
                std::vector<FbxNode*>	mboneNodes;
                std::vector<SMat4>		mboneLocalTM;
                ~FbxBoneData()
                {
                    Release();
                }
                void Release()
                {
                    mbonehierarchy.clear();
                    mboneNodes.clear();
                    mboneLocalTM.clear();
                }
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
                int parentIndex;

                FbxLayerElement::EMappingMode normalMode;
                FbxLayerElement::EMappingMode tangentMode;
                SMat4 meshTM;

                pseudodx::XMFLOAT3* vertices;
                unsigned int* indices;
                pseudodx::XMFLOAT3* normals;
                pseudodx::XMFLOAT3* tangents;
                pseudodx::XMFLOAT2* uvs;
                unsigned int* materialIndices;

                // material
                std::vector<FbxMaterial> fbxmaterials;

                MeshData() : mLayout(NONE), vertexCnt(0), indexCnt(0), normalCnt(0), tangentCnt(0), uvCnt(0),
                    normalMode(FbxLayerElement::eNone), tangentMode(FbxLayerElement::eNone),
                    vertices(nullptr), indices(nullptr), normals(nullptr), tangents(nullptr), uvs(nullptr)
                {
                }
            };

            struct FbxModel
            {
                std::vector<FbxPose*>	mAnimPose;

                // ===== Data we need to export =======
                // need to be exported as binary
                eLayout					mLayout;
                std::string				name;
                FbxBoneData				mBoneData;
                std::vector<MeshData*>	mMeshData;
                std::vector<FbxMaterial*> mMaterials;
                std::vector<ControlPoints*> mCtrlPoints;
                std::vector<AnimationData*> mAnimationData;
                // ====================================
                FbxModel() {}
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
            };
        }
    }
}