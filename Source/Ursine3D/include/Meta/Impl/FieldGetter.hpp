namespace ursine
{
    namespace meta
    {
        ///////////////////////////////////////////////////////////////////////
        // Getter from Method
        ///////////////////////////////////////////////////////////////////////

        template<typename ClassType, typename ReturnType>
        class FieldGetter<ClassType, ReturnType, true> : public FieldGetterBase
        {
        public:
            typedef ReturnType (ClassType::*Signature)(void);
            typedef ReturnType (ClassType::*SignatureConst)(void) const;

            FieldGetter(Signature method)
                : m_method( reinterpret_cast<SignatureConst>( method ) ) { }

            FieldGetter(SignatureConst method)
                : m_method( method ) { }

            Variant GetValue(const Variant &obj) override
            {
                auto &instance = obj.GetValue<ClassType>( );

                return (instance.*m_method)( );
            }

        private:
            SignatureConst m_method;
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