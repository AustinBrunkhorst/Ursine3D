namespace ursine
{
    namespace meta
    {
        template<typename ClassType, typename ... Args>
        void TypeData::AddConstructor(
            Constructor::Invoker invoker,
            const MetaManager::Initializer &meta,
            bool isDynamic
        )
        {
            InvokableSignature signature =
                Invokable::CreateSignature<Args...>( );

            Constructor ctor {
                typeof( ClassType ),
                signature,
                invoker,
                isDynamic
            };

            ctor.m_meta = meta;

            if (isDynamic)
                dynamicConstructors.emplace( signature, ctor );
            else
                constructors.emplace( signature, ctor );
        }

        ///////////////////////////////////////////////////////////////////////

        template<typename ClassType>
        void TypeData::SetArrayConstructor(void)
        {
            arrayConstructor = Constructor {
                typeof( ClassType ),
                { },
                [](ArgumentList &args)
                {
                    return Array<ClassType>( );
                },
                false
            };
        }

        ///////////////////////////////////////////////////////////////////////

        template<typename ClassType, typename FieldType, typename MethodGetterType, typename MethodSetterType>
        void TypeData::AddField(
            const std::string &name,
            MethodGetterType methodGetter,
            MethodSetterType methodSetter,
            const MetaManager::Initializer &meta
        )
        {
            typedef FieldGetter<ClassType, FieldType, true> GetterType;
            typedef FieldSetter<ClassType, FieldType, true> SetterType;

            Field field(
                name,
                typeof( FieldType ),
                typeof( ClassType ),
                !methodGetter ? nullptr : new GetterType( methodGetter ),
                !methodSetter ? nullptr : new SetterType( methodSetter )
            );

            field.m_meta = meta;

            fields.emplace( name, field );
        }

        ///////////////////////////////////////////////////////////////////////

        template<typename ClassType, typename FieldType, typename MethodGetterType>
        void TypeData::AddField(
            const std::string &name,
            MethodGetterType methodGetter,
            typename FieldSetter<ClassType, FieldType, false>::Signature fieldSetter,
            const MetaManager::Initializer &meta
        )
        {
            typedef FieldGetter<ClassType, FieldType, true> GetterType;

            Field field(
                name,
                typeof( FieldType ),
                typeof( ClassType ),
                !methodGetter ? nullptr : new GetterType( methodGetter ),
                !fieldSetter ? nullptr : new FieldSetter<ClassType, FieldType, false>( fieldSetter )
            );

            field.m_meta = meta;

            fields.emplace( name, field );
        }

        ///////////////////////////////////////////////////////////////////////

        template<typename ClassType, typename FieldType, typename MethodSetterType>
        void TypeData::AddField(
            const std::string &name,
            typename FieldGetter<ClassType, FieldType, false>::Signature fieldGetter,
            MethodSetterType methodSetter,
            const MetaManager::Initializer &meta
        )
        {
            typedef FieldSetter<ClassType, FieldType, true> SetterType;

            Field field(
                name,
                typeof( FieldType ),
                typeof( ClassType ),
                !fieldGetter ? nullptr : new FieldGetter<ClassType, FieldType, false>( fieldGetter ),
                !methodSetter ? nullptr : new SetterType( methodSetter )
            );

            field.m_meta = meta;

            fields.emplace( name, field );
        }

        ///////////////////////////////////////////////////////////////////////

        template<typename ClassType, typename FieldType>
        void TypeData::AddField(
            const std::string &name,
            typename FieldGetter<ClassType, FieldType, false>::Signature fieldGetter,
            typename FieldSetter<ClassType, FieldType, false>::Signature fieldSetter,
            const MetaManager::Initializer &meta
        )
        {
            Field field(
                name,
                typeof( FieldType ),
                typeof( ClassType ),
                !fieldGetter ? nullptr : new FieldGetter<ClassType, FieldType, false>( fieldGetter ),
                !fieldSetter ? nullptr : new FieldSetter<ClassType, FieldType, false>( fieldSetter )
            );

            field.m_meta = meta;

            fields.emplace( name, field );
        }

        ///////////////////////////////////////////////////////////////////////

        template<typename ClassType, typename FieldType, typename GetterType, typename SetterType>
        void TypeData::AddStaticField(const std::string &name, GetterType getter, SetterType setter, const MetaManager::Initializer &meta)
        {
            typedef GlobalGetter<FieldType, true> GlobalGetterType;
            typedef GlobalSetter<FieldType, true> GlobalSetterType;

            Global global {
                name,
                typeof( FieldType ),
                !getter ? nullptr : new GlobalGetterType(
                    reinterpret_cast<typename GlobalGetterType::Signature>( getter )
                ),
                !setter ? nullptr : new GlobalSetterType(
                    reinterpret_cast<typename GlobalSetterType::Signature>( setter )
                ),
                typeof( ClassType ),
            };

            global.m_meta = meta;

            staticFields.emplace( name, global );
        }

        ///////////////////////////////////////////////////////////////////////

        template<typename ClassType, typename FieldType, typename GetterType>
        void TypeData::AddStaticField(const std::string &name, GetterType getter, FieldType *fieldSetter, const MetaManager::Initializer &meta)
        {
            typedef GlobalGetter<FieldType, true> GlobalGetterType;
            typedef GlobalSetter<FieldType, false> GlobalSetterType;

            Global global {
                name,
                typeof( FieldType ),
                !getter ? nullptr : new GlobalGetterType(
                    reinterpret_cast<typename GlobalGetterType::Signature>( getter )
                ),
                !fieldSetter ? nullptr : new GlobalSetterType( fieldSetter ),
                typeof( ClassType ),
            };

            global.m_meta = meta;

            staticFields.emplace( name, global );
        }

        ///////////////////////////////////////////////////////////////////////

        template<typename ClassType, typename FieldType, typename SetterType>
        void TypeData::AddStaticField(const std::string &name, FieldType *fieldGetter, SetterType setter, const MetaManager::Initializer &meta)
        {
            typedef GlobalGetter<FieldType, false> GlobalGetterType;
            typedef GlobalSetter<FieldType, true> GlobalSetterType;

             Global global {
                name,
                typeof( FieldType ),
                !fieldGetter ? nullptr : new GlobalGetterType( fieldGetter ),
                !setter ? nullptr : new GlobalSetterType(
                    reinterpret_cast<typename GlobalSetterType::Signature>( setter )
                ),
                typeof( ClassType ),
            };

            global.m_meta = meta;

            staticFields.emplace( name, global );
        }

        ///////////////////////////////////////////////////////////////////////

        template<typename ClassType, typename FieldType>
        void TypeData::AddStaticField(const std::string &name, FieldType *fieldGetter, FieldType *fieldSetter, const MetaManager::Initializer &meta)
        {
            typedef GlobalGetter<FieldType, false> GlobalGetterType;
            typedef GlobalSetter<FieldType, false> GlobalSetterType;

             Global global {
                name,
                typeof( FieldType ),
                !fieldGetter ? nullptr : new GlobalGetterType( fieldGetter ),
                !fieldSetter ? nullptr : new GlobalSetterType( fieldSetter ),
                typeof( ClassType ),
            };

            global.m_meta = meta;

            staticFields.emplace( name, global );
        }

        ///////////////////////////////////////////////////////////////////////

        template<typename ClassType>
        void TypeData::SetDestructor(void)
        {
            destructor = {
                typeof( ClassType ),
                [](Variant &instance)
                {
                    instance.GetValue<ClassType>( ).~ClassType( );
                }
            };
        }

        ///////////////////////////////////////////////////////////////////////

        template<typename MethodType>
        void TypeData::AddMethod(
            const std::string &name,
            MethodType method,
            const MetaManager::Initializer &meta
        )
        {
            Method meth( name, method );

            meth.m_meta = meta;

            methods[ name ].emplace( meth.GetSignature( ), meth );
        }

        ///////////////////////////////////////////////////////////////////////

        template<typename ClassType, typename FunctionType>
        void TypeData::AddStaticMethod(
            const std::string &name,
            FunctionType function,
            const MetaManager::Initializer &meta
        )
        {
            Function fn( name, function, typeof( ClassType ) );

            fn.m_meta = meta;

            staticMethods[ name ].emplace( fn.GetSignature( ), fn );
        }

        ///////////////////////////////////////////////////////////////////////

        template<typename EnumType>
        void TypeData::SetEnum(
            const std::string &name,
            const typename EnumContainer<EnumType>::Table &table
        )
        {
            enumeration = new EnumContainer<EnumType>( name, table );
        }
    }
}
