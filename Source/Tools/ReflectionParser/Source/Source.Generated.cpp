#include "Precompiled.h"

#include "C:/Game Development/Ursine3D/Source/Tools/ReflectionParser/Source/Header.Generated.h"

namespace m = ursine::meta;

m::ReflectionDatabase::Initializer TestInitializer([] {
    auto &db = m::ReflectionDatabase::Instance( );

    ///////////////////////////////////////////////////////////////////////////
    // Class Allocations
    ///////////////////////////////////////////////////////////////////////////
    
    {
        { // Base Type
            auto id = db.AllocateType( "BaseClass" );
            auto &type = db.types[ id ];

            m::TypeInfo<BaseClass>::Register( id, type, false );
        }
        
        { // Pointer Type
            auto id = db.AllocateType( "BaseClass*" );
            auto &type = db.types[ id ];

            m::TypeInfo<BaseClass*>::Register( id, type, false );
        }
        
        
        { // Const Pointer Type
            auto id = db.AllocateType( "const BaseClass*" );
            auto &type = db.types[ id ];

            m::TypeInfo<const BaseClass*>::Register( id, type, false );
        }
        
    }
    
    {
        { // Base Type
            auto id = db.AllocateType( "Test" );
            auto &type = db.types[ id ];

            m::TypeInfo<Test>::Register( id, type, true );
        }
        
        { // Pointer Type
            auto id = db.AllocateType( "Test*" );
            auto &type = db.types[ id ];

            m::TypeInfo<Test*>::Register( id, type, false );
        }
        
        
        { // Const Pointer Type
            auto id = db.AllocateType( "const Test*" );
            auto &type = db.types[ id ];

            m::TypeInfo<const Test*>::Register( id, type, false );
        }
        
    }
    
    {
        { // Base Type
            auto id = db.AllocateType( "Disabled" );
            auto &type = db.types[ id ];

            m::TypeInfo<Disabled>::Register( id, type, false );
        }
        
        { // Pointer Type
            auto id = db.AllocateType( "Disabled*" );
            auto &type = db.types[ id ];

            m::TypeInfo<Disabled*>::Register( id, type, false );
        }
        
        
        { // Const Pointer Type
            auto id = db.AllocateType( "const Disabled*" );
            auto &type = db.types[ id ];

            m::TypeInfo<const Disabled*>::Register( id, type, false );
        }
        
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // Enum Allocations
    ///////////////////////////////////////////////////////////////////////////
    
    {
        { // Base Type
            auto id = db.AllocateType( "Enum" );
            auto &type = db.types[ id ];

            m::TypeInfo<Enum>::Register( id, type, true );
        }
        
        { // Pointer Type
            auto id = db.AllocateType( "Enum*" );
            auto &type = db.types[ id ];

            m::TypeInfo<Enum*>::Register( id, type, false );
        }
        
        
        { // Const Pointer Type
            auto id = db.AllocateType( "const Enum*" );
            auto &type = db.types[ id ];

            m::TypeInfo<const Enum*>::Register( id, type, false );
        }
        
    }
    
    {
        { // Base Type
            auto id = db.AllocateType( "EnumClass" );
            auto &type = db.types[ id ];

            m::TypeInfo<EnumClass>::Register( id, type, true );
        }
        
        { // Pointer Type
            auto id = db.AllocateType( "EnumClass*" );
            auto &type = db.types[ id ];

            m::TypeInfo<EnumClass*>::Register( id, type, false );
        }
        
        
        { // Const Pointer Type
            auto id = db.AllocateType( "const EnumClass*" );
            auto &type = db.types[ id ];

            m::TypeInfo<const EnumClass*>::Register( id, type, false );
        }
        
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // Global Definitions
    ///////////////////////////////////////////////////////////////////////////
    
    db.AddGlobal<int>( "Global",
        [](void)
        {
            return m::Variant { Global };
        },
        [](const m::Variant &value)
        {
            Global = value.GetValue<int>( );
        },
        {
            
        }
    );
    
    ///////////////////////////////////////////////////////////////////////////
    // Global Functions
    ///////////////////////////////////////////////////////////////////////////
    
    db.AddGlobalFunction( "GlobalFunction",
        static_cast<int(*)(int, double, float)>( GlobalFunction ),
        [](m::ArgumentList &args)
        { 
            return m::Variant {
                GlobalFunction( args[ 0 ].GetValue<int>( ), args[ 1 ].GetValue<double>( ), args[ 2 ].GetValue<float>( ) )
            };
        },
        {
            { "A", "B" }, { "OK" }, { "C", "D" }
        }
    );

    auto test = m::Type::GetGlobalFunction( "GlobalFunction" );

    test.GetMeta( ).GetProperty( "Test" )->ToDouble( );
    
    ///////////////////////////////////////////////////////////////////////////
    // Enum Definitions
    ///////////////////////////////////////////////////////////////////////////
    
    {
        auto typeID = m::TypeInfo<Enum>::ID;

        if (typeID != m::Type::Invalid && !m::TypeInfo<Enum>::Defined)
        {
            auto &type = db.types[ typeID ];

            type.meta = {
                
            };

            type.SetEnum<Enum>( "Enum", {
                 { "One", Enum::One }, 
                 { "Two", Enum::Two }, 
                 { "Three", Enum::Three } 
                
            } );

            m::TypeInfo<Enum>::Defined = true;
        }
    }
    
    {
        auto typeID = m::TypeInfo<EnumClass>::ID;

        if (typeID != m::Type::Invalid && !m::TypeInfo<EnumClass>::Defined)
        {
            auto &type = db.types[ typeID ];

            type.meta = {
                
            };

            type.SetEnum<EnumClass>( "EnumClass", {
                 { "Four", EnumClass::Four }, 
                 { "Five", EnumClass::Five }, 
                 { "Six", EnumClass::Six } 
                
            } );

            m::TypeInfo<EnumClass>::Defined = true;
        }
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // Class Definitions
    ///////////////////////////////////////////////////////////////////////////
    
    /**
     * Test
     */
    {
        auto typeID = m::TypeInfo<Test>::ID;

        if (typeID != m::Type::Invalid && !m::TypeInfo<Test>::Defined)
        {
            auto &type = db.types[ typeID ];

            type.meta = {
                
            };

            type.LoadBaseClasses( db, typeID, { 
                m::Type::Get<BaseClass>( )  
            } );

            // Constructors
            
            type.AddConstructor<Test>(
                [](m::ArgumentList &args)
                {
                    return m::Variant { 
                        Test(  )
                    };
                }, 
                {
                    
                }
            );
            
            type.AddConstructor<Test, int, double, float>(
                [](m::ArgumentList &args)
                {
                    return m::Variant { 
                        Test( args[ 0 ].GetValue<int>( ), args[ 1 ].GetValue<double>( ), args[ 2 ].GetValue<float>( ) )
                    };
                }, 
                {
                    
                }
            );
            
            // Fields
            
            type.AddField<Test, double>( "publicField",
                [](const m::Variant &obj)
                {
                    auto &instance = obj.GetValue<Test>( );

                    return m::Variant { instance.publicField };
                },
                [](m::Variant &obj, const m::Variant &value)
                {
                    auto &instance = obj.GetValue<Test>( );

                    instance.publicField = value.GetValue<double>( );
                },
                {
                    
                }
            );
            
            // Static Fields
            
            type.AddStaticField<Test, const int>( "StaticField",
                [](void)
                {
                    return m::Variant { Test::StaticField };
                },
                nullptr,
                {
                    
                }
            );
            
            // Methods
            
            type.AddMethod( "Method",
                static_cast<void(Test::*)()>( &Test::Method ),
                [](m::Variant &obj, m::ArgumentList &args)
                { 
                    auto &instance = obj.GetValue<Test>( );
                    
                    instance.Method(  );

                    return m::Variant { };
                },
                {
                    
                }
            );
            
            type.AddMethod( "ConstMethod",
                static_cast<void(Test::*)() const>( &Test::ConstMethod ),
                [](m::Variant &obj, m::ArgumentList &args)
                { 
                    auto &instance = obj.GetValue<Test>( );
                    
                    instance.ConstMethod(  );

                    return m::Variant { };
                },
                {
                    
                }
            );
            
            type.AddMethod( "OverloadedMethod",
                static_cast<void(Test::*)()>( &Test::OverloadedMethod ),
                [](m::Variant &obj, m::ArgumentList &args)
                { 
                    auto &instance = obj.GetValue<Test>( );
                    
                    instance.OverloadedMethod(  );

                    return m::Variant { };
                },
                {
                    
                }
            );
            
            type.AddMethod( "OverloadedMethod",
                static_cast<void(Test::*)(int)>( &Test::OverloadedMethod ),
                [](m::Variant &obj, m::ArgumentList &args)
                { 
                    auto &instance = obj.GetValue<Test>( );
                    
                    instance.OverloadedMethod( args[ 0 ].GetValue<int>( )  );

                    return m::Variant { };
                },
                {
                    
                }
            );
            
            type.AddMethod( "OverloadedMethod",
                static_cast<void(Test::*)(int, int)>( &Test::OverloadedMethod ),
                [](m::Variant &obj, m::ArgumentList &args)
                { 
                    auto &instance = obj.GetValue<Test>( );
                    
                    instance.OverloadedMethod( args[ 0 ].GetValue<int>( ) , args[ 1 ].GetValue<int>( )  );

                    return m::Variant { };
                },
                {
                    
                }
            );
            
            type.AddMethod( "OverloadedMethod",
                static_cast<void(Test::*)(int, int) const>( &Test::OverloadedMethod ),
                [](m::Variant &obj, m::ArgumentList &args)
                { 
                    auto &instance = obj.GetValue<Test>( );
                    
                    instance.OverloadedMethod( args[ 0 ].GetValue<int>( ) , args[ 1 ].GetValue<int>( )  );

                    return m::Variant { };
                },
                {
                    
                }
            );
            
            // Static Methods
            
            type.AddStaticMethod<Test>( "StaticMethod",
                static_cast<int(*)()>( Test::StaticMethod ),
                [](m::ArgumentList &args)
                { 
                    return m::Variant {
                        Test::StaticMethod(  )
                    };
                },
                {
                    
                }
            );

            m::TypeInfo<Test>::Defined = true;
        }
    }
});