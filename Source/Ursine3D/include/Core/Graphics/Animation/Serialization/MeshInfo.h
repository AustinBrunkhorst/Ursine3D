/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** MeshInfo.h
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "ISerialize.h"
#include "AnimationDef.h"
#include "ResourceReader.h"
#include "ResourceWriter.h"

namespace ursine
{
    namespace graphics
    {
        namespace ufmt_loader
        {
            struct MeshVertex
            {
                pseudodx::XMFLOAT3	pos;
                pseudodx::XMFLOAT3	normal;
                pseudodx::XMFLOAT3	tangent;
                pseudodx::XMFLOAT2	uv;
                pseudodx::XMUINT4	ctrlIndices;
                pseudodx::XMFLOAT4	ctrlBlendWeights;

                MeshVertex()
                {
                    pos = pseudodx::XMFLOAT3(0, 0, 0);
                    normal = pseudodx::XMFLOAT3(0, 0, 0);
                    tangent = pseudodx::XMFLOAT3(0, 0, 0);
                    uv = pseudodx::XMFLOAT2(0, 0);
                    ctrlIndices = pseudodx::XMUINT4(0, 0, 0, 0);
                    ctrlBlendWeights = pseudodx::XMFLOAT4(0, 0, 0, 0);
                }

                bool operator==(const MeshVertex& rhs)
                {
                    if ((pos.x != rhs.pos.x) ||
                        (pos.y != rhs.pos.y) ||
                        (pos.z != rhs.pos.z))
                        return false;

                    if ((normal.x != rhs.normal.x) ||
                        (normal.y != rhs.normal.y) ||
                        (normal.z != rhs.normal.z))
                        return false;

                    if ((tangent.x != rhs.tangent.x) ||
                        (tangent.y != rhs.tangent.y) ||
                        (tangent.z != rhs.tangent.z))
                        return false;

                    if ((uv.x != rhs.uv.x) || (uv.y != rhs.uv.y))
                        return false;

                    if ((ctrlIndices.x != rhs.ctrlIndices.x) ||
                        (ctrlIndices.y != rhs.ctrlIndices.y) ||
                        (ctrlIndices.z != rhs.ctrlIndices.z) ||
                        (ctrlIndices.w != rhs.ctrlIndices.w))
                        return false;

                    if ((ctrlBlendWeights.x != rhs.ctrlBlendWeights.x) ||
                        (ctrlBlendWeights.y != rhs.ctrlBlendWeights.y) ||
                        (ctrlBlendWeights.z != rhs.ctrlBlendWeights.z) ||
                        (ctrlBlendWeights.w != rhs.ctrlBlendWeights.w))
                        return false;

                    return true;
                }

                bool IsValidWeights()
                {
                    if ((0.0f == ctrlBlendWeights.x)
                        && (0.0f == ctrlBlendWeights.y)
                        && (0.0f == ctrlBlendWeights.z)
                        && (0.0f == ctrlBlendWeights.w))
                        return false;

                    return true;
                }

                bool IsValidControls()
                {
                    if ((0 == ctrlIndices.x)
                        && (0 == ctrlIndices.y)
                        && (0 == ctrlIndices.z)
                        && (0 == ctrlIndices.w))
                        return false;

                    return true;
                }

                bool CheckSum()
                {
                    float sum = ctrlBlendWeights.x + ctrlBlendWeights.y + ctrlBlendWeights.z + ctrlBlendWeights.w;

                    if (sum > 1.0f)
                        return false;

                    if (!IsEqualEpsilon(1.0f, sum))
                        return false;

                    return true;
                }
            };

            class MeshInfo : public ISerialize
            {
            public:
                std::string     name;
                unsigned int    meshVtxInfoCount;
                unsigned int    meshVtxIdxCount;
                unsigned int    mtrlCount;
                unsigned int    mtrlIndexCount;
                std::vector< std::string > mtrlName;

                std::vector<MeshVertex>     meshVtxInfos;
                std::vector<unsigned int>   meshVtxIndices; // this will be used when we do vtx reconstruction
                std::vector<unsigned int>   materialIndices;

                MeshInfo();
                virtual ~MeshInfo();

                virtual bool SerializeIn(HANDLE hFile);
                virtual bool SerializeOut(HANDLE hFile);

                void Read(resources::ResourceReader &input) override;
                void Write(resources::pipeline::ResourceWriter &output) override;
            };
        };
    };
};