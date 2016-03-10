#include "UrsinePrecompiled.h"

#include "TextureImporter.h"
#include "TextureData.h"

#include "ResourcePipelineManager.h"

#include <DirectXTex.h>

using namespace ursine;

namespace dx = DirectX;

namespace
{
    const auto kTextureProcessorTool = "Tools\\TextureProcessor.exe";

    fs::path getTempDirectory(const rp::ResourceImportContext &context);

    bool runTexureProcessor(const std::vector<std::string> &arguments);
}

namespace ursine
{
    rp::TextureImporter::TextureImporter(void)
        : m_importedWidth( 0 )
        , m_importedHeight( 0 )
    {
        
    }

    rp::TextureImporter::~TextureImporter(void)
    {
        
    }

    resources::ResourceData::Handle rp::TextureImporter::Import(ResourceImportContext &context)
    {
        auto tempDirectory = getTempDirectory( context );
        auto sourceFile = context.resource->GetSourceFileName( );
        auto displayName = context.resource->GetDisplayName( );

        std::vector<std::string> ddsArgs {
            "-nologo",
            "-o \""+ tempDirectory.string( ) +"\"",
            "-ft dds",
            "\""+ sourceFile.string( ) +"\""
        };

        if (!exists( tempDirectory ))
            create_directories( tempDirectory );

        UAssertCatchable( runTexureProcessor( ddsArgs ),
            "Unable to run texture processor for building DDS'."
        );

        auto ddsFile = change_extension( tempDirectory / displayName, "dds" );

        BinaryData ddsData;

        UAssertCatchable( fs::LoadAllBinary( ddsFile.string( ), ddsData ),
            "Unable to load built DDS file.\nfile: %s",
            ddsFile.string( ).c_str( )
        );

        dx::TexMetadata meta;

        auto result = GetMetadataFromDDSMemory( 
            ddsData.GetData( ), 
            ddsData.GetSize( ), 
            dx::DDS_FLAGS_NONE, 
            meta 
        );

        UAssertCatchable( result == S_OK,
            "Unable to get meta data from built DDS file.\nfile: %s",
            ddsFile.string( ).c_str( )
        );

        try
        {
            fs::remove( ddsFile );
        } 
        catch(...)
        {
            // do nothing
        }

        m_importedWidth = static_cast<unsigned>( meta.width );
        m_importedHeight = static_cast<unsigned>( meta.height );

        auto data = std::make_shared<TextureData>(
            std::move( ddsData ),
            m_importedWidth, 
            m_importedHeight
        );

        return data;
    }

    bool rp::TextureImporter::BuildPreview(const ResourceImportContext &context, const Vec2 &maxDimension)
    {
        auto maxOriginalComponent = math::Min( m_importedWidth, m_importedHeight );

        auto maxComponent = math::Min( maxDimension.X( ), maxDimension.Y( ) );

        auto sizeRatio = 1.0f;

        if (maxOriginalComponent >= maxComponent)
            sizeRatio = maxComponent / maxOriginalComponent;

        auto previewWidth = static_cast<unsigned>( sizeRatio * m_importedWidth );
        auto previewHeight = static_cast<unsigned>( sizeRatio * m_importedHeight );

        auto tempDirectory = getTempDirectory( context );
        auto sourceFile = context.resource->GetSourceFileName( );
        auto previewFile = context.resource->GetPreviewFileName( );
        auto displayName = context.resource->GetDisplayName( );

        std::vector<std::string> previewArgs {
            "-nologo",
            "-o \""+ tempDirectory.string( ) +"\"",
            "-ft png",
            "-w "+ std::to_string( previewWidth ),
            "-h "+ std::to_string( previewHeight ),
            "\""+ sourceFile.string( ) +"\""
        };

        UAssertCatchable( runTexureProcessor( previewArgs ),
            "Unable to run texture processor for building preview."
        );

        auto pngFile = change_extension( tempDirectory / displayName, "png" );

        boost::system::error_code error;

        copy_file( 
            pngFile, 
            previewFile, 
            fs::copy_option::overwrite_if_exists, 
            error 
        );

        UAssertCatchable( !error,
            "Unable to copy built png to preview file.\nsource: %s\ntarget: %s",
            pngFile.string().c_str( ),
            previewFile.string( ).c_str( )
        );

        try
        {
            // don't need the temp directory any more
            remove_all( tempDirectory );
        } 
        catch(...)
        {
            // do nothing
        }

        return true;
    }
}

namespace
{
    fs::path getTempDirectory(const rp::ResourceImportContext &context)
    {
        return context.pipeline->GetConfig( ).tempDirectory / to_string( context.resource->GetGUID( ) );
    }

    bool runTexureProcessor(const std::vector<std::string> &arguments)
    {
        std::string argString;

        utils::Join( arguments, " ", argString );

        SHELLEXECUTEINFO shExecInfo;

        shExecInfo.cbSize = sizeof( SHELLEXECUTEINFO );
        shExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
        shExecInfo.hwnd = nullptr;
        shExecInfo.lpVerb = nullptr;
        shExecInfo.lpFile = kTextureProcessorTool;
        shExecInfo.lpParameters = argString.c_str( );
        shExecInfo.lpDirectory = nullptr;
        shExecInfo.nShow = SW_HIDE;
        shExecInfo.hInstApp = nullptr;

        auto result = ShellExecuteEx( &shExecInfo );

        if (result != TRUE)
            return false;

        WaitForSingleObject( shExecInfo.hProcess, INFINITE );

        CloseHandle( shExecInfo.hProcess );

        return true;
    }
}
