/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BoneInfo.h
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
            class BoneInfo : public ISerialize
            {
            public:
                std::string name;
                int mParentIndex;

                // bind - local coord system that the entire skin is defined relative to
                // local tm. local about to the skinned mesh
                pseudodx::XMFLOAT3 bindPosition;
                pseudodx::XMFLOAT4 bindRotation;
                pseudodx::XMFLOAT3 bindScaling;

                // bone space - the space that influences the vertices. so-called offset transformation
                // bone offset tm
                pseudodx::XMFLOAT3 boneSpacePosition;
                pseudodx::XMFLOAT4 boneSpaceRotation;
                pseudodx::XMFLOAT3 boneSpaceScaling;

                /** @brief bone information constructor
                *
                *  this will construct bone information object
                *
                *  @return nothing
                */
                BoneInfo();

                /** @brief bone information destructor
                *
                *  this will destroy bone information object
                *
                *  @return nothing
                */
                virtual ~BoneInfo();

                /** @brief bone information serialize in function
                *
                *  this will read bone information
                *
                *  @param handle of the file
                *  @return if succeed return true, else return false
                */
                virtual bool SerializeIn(HANDLE hFile);

                /** @brief bone information serialize out function
                *
                *  this will write bone information
                *
                *  @param handle of the file
                *  @return if succeed return true, else return false
                */
                virtual bool SerializeOut(HANDLE hFile);

                void Read(resources::ResourceReader &input) override;
                void Write(resources::pipeline::ResourceWriter &output) override;
            };
        };
    };
};