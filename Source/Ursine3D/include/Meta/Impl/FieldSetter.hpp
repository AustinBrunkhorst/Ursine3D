namespace ursine
{
    namespace meta
    {
        ///////////////////////////////////////////////////////////////////////
        // Setter from Method
        ///////////////////////////////////////////////////////////////////////

        template<typename ClassType, typename FieldType>
        class FieldSetter<ClassType, FieldType, true> : public FieldSetterBase
        {
        public:
            typedef typename std::remove_const<FieldType>::type NonConstFieldType;

            typedef void (ClassType::*Signature)(FieldType);
            typedef void (ClassType::*SignatureConst)(FieldType) const;
            typedef void (ClassType::*RefSignature)(NonConstFieldType &);
            typedef void (ClassType::*RefSignatureConst)(NonConstFieldType &) const;
            typedef void (ClassType::*ConstRefSignature)(const NonConstFieldType &);
            typedef void (ClassType::*ConstRefSignatureConst)(const NonConstFieldType &) const;

            FieldSetter(Signature method)
                : m_method( reinterpret_cast<ConstRefSignature>( method ) ) { }

            FieldSetter(SignatureConst method)
                : m_method( reinterpret_cast<ConstRefSignature>( method ) ) { }

            FieldSetter(RefSignature method)
                : m_method( reinterpret_cast<ConstRefSignature>( method ) ) { }

            FieldSetter(RefSignatureConst method)
                : m_method( reinterpret_cast<ConstRefSignature>( method ) ) { }

            FieldSetter(ConstRefSignature method)
                : m_method( method ) { }

            FieldSetter(ConstRefSignatureConst method)
                : m_method( reinterpret_cast<ConstRefSignature>( method ) ) { }

            void SetValue(Variant &obj, const Variant &value) override
            {
                auto &instance = obj.GetValue<ClassType>( );

                (instance.*m_method)( value.GetValue<FieldType>( ) );
            }

        private:
            ConstRefSignature m_method;
        };

        ///////////////////////////////////////////////////////////////////////
        // Setter from Direct Field
        ///////////////////////////////////////////////////////////////////////

        template<typename ClassType, typename FieldType>
        class FieldSetter<ClassType, FieldType, false> : public FieldSetterBase
        {
        public:
            typedef FieldType (ClassType::*Signature);

            FieldSetter(Signature field)
                : m_field( field ) { }

            void SetValue(Variant &obj, const Variant &value) override
            {
                auto &instance = obj.GetValue<ClassType>( );

                const_cast<typename std::remove_const<FieldType>::type&>( instance.*m_field ) = value.GetValue<FieldType>( );
            }

        private:
            Signature m_field;
        };
    }
}