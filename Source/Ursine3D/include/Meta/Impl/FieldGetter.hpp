namespace ursine
{
    namespace meta
    {
        ///////////////////////////////////////////////////////////////////////
        // Getter from Method
        ///////////////////////////////////////////////////////////////////////

        template<typename ClassType, typename FieldType>
        class FieldGetter<ClassType, FieldType, true> : public FieldGetterBase
        {
        public:
            typedef typename std::remove_const<FieldType>::type NonConstFieldType;

            typedef NonConstFieldType (ClassType::*Signature)(void);
            typedef NonConstFieldType (ClassType::*SignatureConst)(void) const;
            typedef NonConstFieldType &(ClassType::*RefSignature)(void);
            typedef NonConstFieldType &(ClassType::*RefSignatureConst)(void) const;
            typedef const NonConstFieldType &(ClassType::*ConstRefSignature)(void);
            typedef const NonConstFieldType &(ClassType::*ConstRefSignatureConst)(void) const;

            FieldGetter(Signature method)
                : m_method( reinterpret_cast<ConstRefSignatureConst>( method ) ) { }

            FieldGetter(SignatureConst method)
                : m_method( reinterpret_cast<ConstRefSignatureConst>( method ) ) { }

            FieldGetter(RefSignature method)
                : m_method( reinterpret_cast<ConstRefSignatureConst>( method ) ) { }

            FieldGetter(RefSignatureConst method)
                : m_method( reinterpret_cast<ConstRefSignatureConst>( method ) ) { }

            FieldGetter(ConstRefSignature method)
                : m_method( reinterpret_cast<ConstRefSignatureConst>( method ) ) { }

            FieldGetter(ConstRefSignatureConst method)
                : m_method( method ) { }

            Variant GetValue(const Variant &obj) override
            {
                auto &instance = obj.GetValue<ClassType>( );

                return (instance.*m_method)( );
            }

        private:
            ConstRefSignatureConst m_method;
        };

        ///////////////////////////////////////////////////////////////////////
        // Getter from Direct Field
        ///////////////////////////////////////////////////////////////////////

        template<typename ClassType, typename FieldType>
        class FieldGetter<ClassType, FieldType, false> : public FieldGetterBase
        {
        public:
            typedef FieldType (ClassType::*Signature);

            FieldGetter(Signature field)
                : m_field( field ) { }

            Variant GetValue(const Variant &obj) override
            {
                auto &instance = obj.GetValue<ClassType>( );

                return instance.*m_field;
            }

        private:
            Signature m_field;
        };
    }
}