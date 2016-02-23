#include "UrsinePrecompiled.h"

#include "TextureImporter.h"
#include "TextureData.h"

#include <SDL_image.h>
#include <DirectXTex.h>

namespace dx = DirectX;

namespace ursine
{
    rp::TextureImporter::TextureImporter(void)
        : m_inputSurface( nullptr )
    {
        
    }

    rp::TextureImporter::~TextureImporter(void)
    {
        SDL_FreeSurface( m_inputSurface );
    }

    resources::ResourceData::Handle rp::TextureImporter::Import(const fs::path &fileName, const ResourceImportContext &context)
    {
        m_inputSurface = IMG_Load( fileName.string( ).c_str( ) );

        URSINE_TODO( "Make all errors recoverable." );

        UAssert( m_inputSurface != nullptr,
            "Unable to load texture.\nerror: %s\nfile: %s",
            IMG_GetError( ),
            fileName.string( ).c_str( )
        );

        auto width = static_cast<unsigned>( m_inputSurface->w );
        auto height = static_cast<unsigned>( m_inputSurface->h );

        auto conversionFlags = 0;
        auto targetDXFormat = DXGI_FORMAT_UNKNOWN;

        auto bpp = m_inputSurface->format->BytesPerPixel;

        switch (bpp)
        {
        // RGBA
        case 4:
            conversionFlags = SDL_PIXELFORMAT_RGB332;
            targetDXFormat = DXGI_FORMAT_R8G8B8A8_UINT;
            break;
        // RGB
        case 3:
            conversionFlags = SDL_PIXELFORMAT_RGBA4444;
            targetDXFormat = DXGI_FORMAT_R32G32B32_UINT;
            break;
        default:
            UError( "Unsupported texture format.\nfile: %s\nBPP: %i",
                fileName.string( ).c_str( ),
                bpp 
            );
        }

        auto *format = SDL_AllocFormat( conversionFlags );

        auto *convertedSurface = SDL_ConvertSurface( m_inputSurface, format, 0 );

        UAssert( convertedSurface != nullptr,
            "Unable to convert texture surface.\nerror: %s\nfile: %s",
            SDL_GetError( ),
            fileName.string( ).c_str( )
        );

        dx::Image dxImage;
        
        dxImage.width = width;
        dxImage.height = height;
        dxImage.rowPitch = convertedSurface->pitch;

        // ignored on 2D textures
        dxImage.slicePitch = 1;

        dxImage.format = targetDXFormat;
        dxImage.pixels = static_cast<uint8_t*>( convertedSurface->pixels );

        dx::Blob outputBlob;

        SDL_LockSurface( convertedSurface );

        auto result = SaveToDDSMemory( dxImage, dx::DDS_FLAGS_NONE, outputBlob );

        SDL_UnlockSurface( convertedSurface );

        UAssert( result == S_OK,
            "Unable to convert texture to dds.\nfile: %s",
            fileName.string( ).c_str( )
        );

        SDL_FreeFormat( format );
        SDL_FreeSurface( convertedSurface );

        return std::make_shared<TextureData>(
            outputBlob.GetBufferPointer( ), 
            outputBlob.GetBufferSize( ), 
            width, 
            height 
        );
    }

    bool rp::TextureImporter::BuildPreview(const fs::path &previewFileName, const Vec2 &maxDimension)
    {
        auto originalWidth = m_inputSurface->w;
        auto originalHeight = m_inputSurface->h;

        auto maxOriginalComponent = math::Min( originalWidth, originalHeight );

        auto maxComponent = math::Min( maxDimension.X( ), maxDimension.Y( ) );

        auto sizeRatio = 1.0f;

        if (maxOriginalComponent >= maxComponent)
            sizeRatio = maxComponent / maxOriginalComponent;

        // the original image can fit in the max dimensions
        if (math::IsEqual( sizeRatio, 1.0f ))
        {
            IMG_SavePNG( m_inputSurface, previewFileName.string( ).c_str( ) );
        }
        else
        {
            auto previewWidth = static_cast<int>( originalWidth * sizeRatio );
            auto previewHeight = static_cast<int>( originalHeight * sizeRatio );

            auto *format = m_inputSurface->format; 

            auto *preview = SDL_CreateRGBSurface(
                SDL_SWSURFACE, 
                previewWidth, 
                previewHeight, 
                format->BitsPerPixel, 
                format->Rmask, 
                format->Gmask, 
                format->Bmask, 
                format->Amask 
            ); 

            // fill with black transparency
            SDL_FillRect( preview, nullptr, SDL_MapRGBA( preview->format, 0, 0, 0, 0 ) ); 

            // resize input to preview
            SDL_SoftStretch( m_inputSurface, nullptr, preview, nullptr );

            // write the preview
            IMG_SavePNG( preview, previewFileName.string( ).c_str( ) );

            SDL_FreeSurface( preview ); 
        }

        return true;
    }
}