///* ----------------------------------------------------------------------------
//** Team Bear King
//** © 2016 DigiPen Institute of Technology, All Rights Reserved.
//**
//** ModelData.h
//**
//** Author:
//** - Park Hyung Jun - park.hyungjun@digipen.edu
//**
//** Contributors:
//** - <list in same format as author if applicable>
//** --------------------------------------------------------------------------*/
//
//#pragma once
//
//#include "ResourceData.h"
//
//#include "GfxDefines.h"
//#include "ModelInfo.h"
//
//namespace ursine
//{
//    namespace resources
//    {
//        class ModelData : public ResourceData
//        {
//            RESOURCE_DATA;
//
//        public:
//            ModelData(graphics::ufmt_loader::ModelInfo modelInfo);
//            ~ModelData(void);
//
//            graphics::GfxHND GetModelHandle(void) const;
//
//        private:
//            graphics::GfxHND m_modelHandle;
//
//            void Write(pipeline::ResourceWriter &output) override;
//
//            meta::Type GetReaderType(void) override;
//        } Meta(Register);
//    }
//}