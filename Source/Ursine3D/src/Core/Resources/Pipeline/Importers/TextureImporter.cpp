#include "UrsinePrecompiled.h"

#include "TextureImporter.h"
#include "TextureData.h"

#include <SDL_image.h>
#include <DirectXTex.h>

namespace dx = DirectX;

namespace ursine
{
    rp::TextureImporter::TextureImporter(void) { }

    resources::ResourceData::Handle rp::TextureImporter::Import(const fs::path &fileName, const ResourceImportContext &context)
    {
        auto *inputSurface = IMG_Load( fileName.string( ).c_str( ) );

        URSINE_TODO( "Make all errors recoverable." );

        UAssert( inputSurface != nullptr,
            "Unable to load texture.\nerror: %s\nfile: %s",
            IMG_GetError( ),
            fileName.string( ).c_str( )
        );

        auto width = static_cast<unsigned>( inputSurface->w );
        auto height = static_cast<unsigned>( inputSurface->h );

        auto conversionFlags = 0;
        auto targetDXFormat = DXGI_FORMAT_UNKNOWN;

        auto bpp = inputSurface->format->BytesPerPixel;

        switch (bpp)
        {
        // RGBA
        case 4:
        case 3:
            conversionFlags = SDL_PIXELFORMAT_RGBA8888;
            targetDXFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
            break;
        default:
            UError( "Unsupported texture format.\nfile: %s\nBPP: %i",
                fileName.string( ).c_str( ),
                bpp 
            );
        }

        auto *format = SDL_AllocFormat( conversionFlags );

        auto *convertedSurface = SDL_ConvertSurface( inputSurface, format, 0 );

        UAssert( convertedSurface != nullptr,
            "Unable to convert texture surface.\nerror: %s\nfile: %s",
            SDL_GetError( ),
            fileName.string( ).c_str( )
        );

        SDL_LockSurface( convertedSurface );

        dx::Image dxImage;
        
        dxImage.width = width;
        dxImage.height = height;
        dxImage.rowPitch = convertedSurface->pitch;

        // ignored on 2D textures
        dxImage.slicePitch = width * height * sizeof(uint8_t);

        dxImage.format = targetDXFormat;
        dxImage.pixels = static_cast<uint8_t*>( convertedSurface->pixels );

        dx::Blob outputBlob;

        auto result = SaveToDDSMemory( dxImage, dx::DDS_FLAGS_NONE, outputBlob );

        SDL_UnlockSurface( convertedSurface );

        UAssert( result == S_OK,
            "Unable to convert texture to dds.\nfile: %s",
            fileName.string( ).c_str( )
        );

        SDL_FreeFormat( format );
        SDL_FreeSurface( inputSurface );
        SDL_FreeSurface( convertedSurface );

        return std::make_shared<TextureData>( 
            outputBlob.GetBufferPointer( ), 
            outputBlob.GetBufferSize( ), 
            width, 
            height 
        );
    }
}