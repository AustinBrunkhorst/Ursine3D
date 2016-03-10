#include "UrsinePrecompiled.h"

#include "FontImporter.h"
#include "FontData.h"

#include "ResourcePipelineManager.h"

namespace ursine
{
    namespace
    {
        const auto kFontProcessorTool = "Tools\\FontProcessor.exe";
        const auto kFntExtension = "fnt";
        const auto kFontTexturePageExtension = ".dds";

        fs::path getTempDirectory(const rp::ResourceImportContext &context);

        bool runFontProcessor(const std::vector<std::string> &arguments);

        void addTexturePage(
            const fs::path &sourcePath,
            const fs::path &entryPath,
            resources::FontData::TexturePageTable &table
        );
    }

    resources::pipeline::FontImporter::FontImporter(void)
    {
        
    }

    resources::ResourceData::Handle rp::FontImporter::Import(ResourceImportContext &context)
    {
        auto tempDirectory = getTempDirectory( context );
        auto sourceFile = context.resource->GetSourceFileName( );

        auto outputFntFile = 
            tempDirectory / sourceFile.filename( ).replace_extension( kFntExtension );

        std::vector<std::string> fontArgs {
            "-c \""+ sourceFile.string( ) + "\"",
            "-o \""+ outputFntFile.string( ) +"\""
        };

        if (!exists( tempDirectory ))
            create_directories( tempDirectory );

        UAssertCatchable( runFontProcessor( fontArgs ),
            "Unable to run font processor."
        );

        BinaryData fntData;

        UAssertCatchable( fs::LoadAllBinary( outputFntFile.string( ), fntData ),
            "Unable to load built fnt file.\nfile: %s",
            outputFntFile.string( ).c_str( )
        );

        FontData::TexturePageTable table;

        fs::DirectoryIterator it( tempDirectory );
        fs::DirectoryIterator itEnd;

        for (; it != itEnd; ++it)
        {
            auto &entry = it->path( );

            auto extension = entry.extension( ).string( );

            utils::MakeLowerCase( extension );

            if (extension == kFontTexturePageExtension)
            {
                auto relativePath = fs::MakeRelativePath( tempDirectory, entry );

                addTexturePage( entry, relativePath, table );
            }
                
        }

        try
        {
            // don't need the temp directory any more
            remove_all( tempDirectory );
        } 
        catch(...)
        {
            // do nothing
        }

        return std::make_shared<FontData>( std::move( fntData ), std::move( table ) );
    }

    namespace
    {
        fs::path getTempDirectory(const rp::ResourceImportContext &context)
        {
            return context.pipeline->GetConfig( ).tempDirectory / to_string( context.resource->GetGUID( ) );
        }

        bool runFontProcessor(const std::vector<std::string> &arguments)
        {
            std::string argString;

            utils::Join( arguments, " ", argString );

            SHELLEXECUTEINFO shExecInfo;

            shExecInfo.cbSize = sizeof( SHELLEXECUTEINFO );
            shExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
            shExecInfo.hwnd = nullptr;
            shExecInfo.lpVerb = nullptr;
            shExecInfo.lpFile = kFontProcessorTool;
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

        void addTexturePage(
            const fs::path &sourcePath,
            const fs::path &entryPath,
            resources::FontData::TexturePageTable &table
        )
        {
            auto data = std::make_shared<BinaryData>( );

            UAssert( fs::LoadAllBinary( sourcePath.string( ), *data ),
                "Failed to load font texture page.\nfile: %s",
                sourcePath.string( ).c_str( )
            );

            table.emplace( entryPath, data );
        }
    }
}
