#include "Precompiled.h"

#include "NativeToolHandler.h"

JSConstructor(NativeToolHandler)
    : m_tool( nullptr )
{
    if (arguments.size( ) < 1)
        JSThrow( "Invalid constructor called." );

    auto toolName = arguments[ 0 ]->GetStringValue( );

    auto toolType = ursine::meta::Type::GetFromName( toolName );

    if (!toolType.IsValid( ))
        JSThrow( "Invalid or unknown tool type." );

    auto constructorType = ursine::meta::InvokableSignature {
        typeof( CefRefPtr<CefV8Value> ),
        typeof( const CefV8ValueList& ),
        typeof( CefString& )
    };

    auto constructor = toolType.GetDynamicConstructor( constructorType );

    if (!constructor.IsValid( ))
        JSThrow( "Tool does not have valid constructor." );

    m_tool = constructor
        .Invoke( 
            context, 
            CefV8ValueList { arguments.begin( ) + 1, arguments.end( ) }, 
            std::move( exception ) 
        )
        .GetValue<NativeEditorTool*>( );
}

NativeToolHandler::~NativeToolHandler(void)
{
    delete m_tool;

    m_tool = nullptr;
}

JSMethod(NativeToolHandler::Event)
{
    if (arguments.size( ) < 2)
        JSThrow( "Invalid function arguments.", nullptr );

    auto eventName = arguments[ 0 ]->GetStringValue( );

    NativeToolEvent e( eventName, arguments[ 1 ] );

    m_tool->Dispatch( eventName, &e );

    return CefV8Value::CreateBool( true );
}