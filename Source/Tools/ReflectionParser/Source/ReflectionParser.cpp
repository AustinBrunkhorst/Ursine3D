#include "Precompiled.h"

#include "ReflectionParser.h"

#include "Class.h"
#include "Global.h"
#include "Function.h"
#include "Enum.h"

#define RECURSE_NAMESPACES(kind, cursor, method, ns) \
    if (kind == CXCursor_Namespace)                  \
    {                                                \
        auto displayName = cursor.GetDisplayName( ); \
        if (!displayName.empty( ))                   \
        {                                            \
            ns.emplace_back( displayName );          \
            method( cursor, ns );                    \
            ns.pop_back( );                          \
        }                                            \
    }                                                \

ReflectionParser::ReflectionParser(const ReflectionOptions &options)
    : m_options( options )
    , m_index( nullptr )
    , m_translationUnit( nullptr )
{
   
}

ReflectionParser::~ReflectionParser(void)
{
    for (auto *klass : m_classes)
        delete klass;

    for (auto *global : m_globals)
        delete global;

    for (auto *globalFunction : m_globalFunctions)
        delete globalFunction;

    for (auto *enewm : m_enums)
        delete enewm;

    if (m_translationUnit)
        clang_disposeTranslationUnit( m_translationUnit );

    if (m_index)
        clang_disposeIndex( m_index );
}

void ReflectionParser::Parse(void)
{
     m_index = clang_createIndex( true, false );

    m_translationUnit = clang_createTranslationUnitFromSourceFile(
        m_index,
        m_options.inputSourceFile.c_str( ),
        m_options.arguments.size( ),
        m_options.arguments.data( ),
        0,
        nullptr
    );

    auto cursor = clang_getTranslationUnitCursor( m_translationUnit );

    Namespace tempNamespace;

    buildClasses( cursor, tempNamespace );

    tempNamespace.clear( );

    buildGlobals( cursor, tempNamespace );

    tempNamespace.clear( );

    buildGlobalFunctions( cursor, tempNamespace );

    tempNamespace.clear( );

    buildEnums( cursor, tempNamespace );
}

std::string ReflectionParser::GenerateHeader(const std::string &tmpl) const
{
    TemplateData headerData { TemplateData::Type::Object };

    // general options

    headerData[ "targetName" ] = m_options.targetName;
    headerData[ "inputSourceFile" ] = m_options.inputSourceFile;
    headerData[ "ouputHeaderFile" ] = m_options.outputHeaderFile;
    headerData[ "outpuSourceFile" ] = m_options.outputSourceFile;

    MustacheTemplate headerTemplate { tmpl };

    if (!headerTemplate.isValid( ))
    {
        std::stringstream error;

        error << "Unable to compile header template." << std::endl;
        error << headerTemplate.errorMessage( );

        throw std::exception( error.str( ).c_str( ) );
    }

    return headerTemplate.render( headerData );
}

std::string ReflectionParser::GenerateSource(const std::string &tmpl) const
{
    TemplateData sourceData { TemplateData::Type::Object };

    // general options

    sourceData[ "targetName" ] = m_options.targetName;
    sourceData[ "inputSourceFile" ] = m_options.inputSourceFile;
    sourceData[ "ouputHeaderFile" ] = m_options.outputHeaderFile;
    sourceData[ "outpuSourceFile" ] = m_options.outputSourceFile;

    // language type data

    sourceData[ "class" ] = compileClassTemplates( );
    sourceData[ "global" ] = compileGlobalTemplates( );
    sourceData[ "globalFunction" ] = compileGlobalFunctionTemplates( );
    sourceData[ "enum" ] = compileEnumTemplates( );

    MustacheTemplate sourceTemplate { tmpl };

    if (!sourceTemplate.isValid( ))
    {
        std::stringstream error;

        error << "Unable to compile header template." << std::endl;
        error << sourceTemplate.errorMessage( );

        throw std::exception( error.str( ).c_str( ) );
    }

    return sourceTemplate.render( sourceData );
}

void ReflectionParser::buildClasses(const Cursor &cursor, Namespace &currentNamespace)
{
    for (auto &child : cursor.GetChildren( ))
    {
        auto kind = child.GetKind( );

        // actual definition and a class or struct
        if (child.IsDefinition( ) && (kind == CXCursor_ClassDecl || kind == CXCursor_StructDecl))
        {
            m_classes.emplace_back( 
                new Class( child, currentNamespace )
            );
        }
        
        RECURSE_NAMESPACES( kind, child, buildClasses, currentNamespace );
    }
}

void ReflectionParser::buildGlobals(const Cursor &cursor, Namespace &currentNamespace)
{
    for (auto &child : cursor.GetChildren( ))
    {
        // skip static globals (hidden)
        if (child.GetStorageClass( ) == CX_SC_Static)
            continue;

        auto kind = child.GetKind( );

        // variable declaration, which is global
        if (kind == CXCursor_VarDecl) 
        {
            m_globals.emplace_back( 
                new Global( child, currentNamespace ) 
            );
        }

        RECURSE_NAMESPACES( kind, child, buildGlobals, currentNamespace );
    }
}

void ReflectionParser::buildGlobalFunctions(const Cursor &cursor, Namespace &currentNamespace)
{
    for (auto &child : cursor.GetChildren( ))
    {
        // skip static globals (hidden)
        if (child.GetStorageClass( ) == CX_SC_Static)
            continue;

        auto kind = child.GetKind( );

        // function declaration, which is global
        if (kind == CXCursor_FunctionDecl) 
        {
            m_globalFunctions.emplace_back( 
                new Function( child, currentNamespace ) 
            );
        }

        RECURSE_NAMESPACES( kind, child, buildGlobalFunctions, currentNamespace );
    }
}

void ReflectionParser::buildEnums(const Cursor &cursor, Namespace &currentNamespace)
{
    for (auto &child : cursor.GetChildren( ))
    {
        auto kind = child.GetKind( );

        // actual definition and an enum
        if (child.IsDefinition( ) && kind == CXCursor_EnumDecl)
        {
            // anonymous enum if the underlying type display name contains this
            if (child.GetType( ).GetDisplayName( ).find( "anonymous enum at" ) != std::string::npos)
            {
                // anonymous enums are just loaded as globals with each of their values
                Enum::LoadAnonymous( m_globals, child, currentNamespace );
            }
            else
            {
                m_enums.emplace_back( 
                    new Enum( child, currentNamespace ) 
                );
            }
        }

        RECURSE_NAMESPACES( kind, child, buildEnums, currentNamespace );
    }
}

TemplateData ReflectionParser::compileClassTemplates(void) const
{
    TemplateData data = { TemplateData::Type::List };

    for (auto *klass : m_classes)
        data << klass->CompileTemplate( );

    return data;
}

TemplateData ReflectionParser::compileGlobalTemplates(void) const
{
    TemplateData data = { TemplateData::Type::List };

    for (auto *global : m_globals)
        data << global->CompileTemplate( );

    return data;
}

TemplateData ReflectionParser::compileGlobalFunctionTemplates(void) const
{
    TemplateData data = { TemplateData::Type::List };

    for (auto *globalFunction : m_globalFunctions)
        data << globalFunction->CompileTemplate( );

    return data;
}

TemplateData ReflectionParser::compileEnumTemplates(void) const
{
    TemplateData data = { TemplateData::Type::List };

    for (auto *enewm : m_enums)
        data << enewm->CompileTemplate( );

    return data;
}