/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/
#ifndef TYPES_H_
#define TYPES_H_




namespace CPPModel
{

	class Type : public IXMLSerializable, public IAttributes
	{
	public :

		enum class Kind { FUNDAMENTAL, USER_DEFINED, DERIVED, UNION, UNRECOGNIZED };


		Type() = delete;
		Type( Type& ) = delete;
		Type( const Type& ) = delete;

		Type( TypeInfo::Specifier		typeSpec )
			: m_typeSpecifier( typeSpec ),
			  m_attributes()
		{}

		Type( TypeInfo::Specifier		typeSpec,
			  ConstListPtr<Attribute>&	attributes )
			: m_typeSpecifier( typeSpec ),
			  m_attributes( attributes )
		{}


		virtual ~Type()
		{}



		TypeInfo::Specifier				type() const
		{
			return( m_typeSpecifier );
		}

		const Attributes&				attributes() const
		{
			return( m_attributes );
		}

		virtual Kind					kind() const = 0;


		virtual std::ostream&	toXML( std::ostream&			outputStream,
							   	   	   int						indentLevel,
							   	   	   SerializationOptions		options ) const = 0;


	private :

		const TypeInfo::Specifier		m_typeSpecifier;

		const Attributes				m_attributes;
	};


	class UnrecognizedType : public Type
	{
	public :

		UnrecognizedType( UnrecognizedType& ) = delete;
		UnrecognizedType( const UnrecognizedType& ) = delete;

		UnrecognizedType()
			: Type( TypeInfo::Specifier::UNRECOGNIZED )
		{}

		virtual ~UnrecognizedType()
		{}


		Kind	kind() const
		{
			return( Kind::UNRECOGNIZED );
		}

		std::ostream&	toXML( std::ostream&			outputStream,
							   int						indentLevel,
							   SerializationOptions		options ) const;
	};



	class FundamentalType : public Type
	{
	public :

		FundamentalType() = delete;
		FundamentalType( FundamentalType& ) = delete;
		FundamentalType( const FundamentalType& ) = delete;

		FundamentalType( TypeInfo::Specifier		typeSpec )
			: Type( typeSpec )
		{}

		virtual ~FundamentalType()
		{}


		Kind	kind() const
		{
			return( Kind::FUNDAMENTAL );
		}


		std::ostream&	toXML( std::ostream&			outputStream,
							   int						indentLevel,
							   SerializationOptions		options ) const;
	};



	class UserDefinedType : public Type, public SourceElement, public NamespaceScoped
	{
	public :

		UserDefinedType() = delete;
		UserDefinedType( UserDefinedType& ) = delete;
		UserDefinedType( const UserDefinedType& ) = delete;

		UserDefinedType( TypeInfo::Specifier			typeSpec,
						 const std::string&				name,
						 const UID&						uid,
						 const Namespace&				namespaceScope,
						 const SourceLocation&			sourceLocation,
						 ConstListPtr<Attribute>&		attributes )
			: Type( typeSpec, attributes ),
			  SourceElement( name, uid, sourceLocation ),
			  NamespaceScoped( namespaceScope )
		{
			assert( uid.uidType() == UID::UIDType::TYPE );
		}


		virtual ~UserDefinedType()
		{}


		Kind									kind() const
		{
			return( Kind::USER_DEFINED );
		}


		std::ostream&	toXML( std::ostream&			outputStream,
							   int						indentLevel,
							   SerializationOptions		options ) const;
	};




	class DerivedType : public Type
	{
	public :

		DerivedType() = delete;
		DerivedType( DerivedType& ) = delete;
		DerivedType( const DerivedType& ) = delete;

		DerivedType( TypeInfo::Specifier			typeSpec,
					 ConstListPtr<Attribute>&		attributes,
					 std::unique_ptr<const Type>	baseType )
			: Type( typeSpec, attributes ),
			  m_baseType( std::move( baseType ))
		{}


		virtual ~DerivedType()
		{}


		Kind			kind() const
		{
			return( Kind::DERIVED );
		}

		const Type&		baseType() const
		{
			return( *m_baseType );
		}

		std::ostream&	toXML( std::ostream&			outputStream,
							   int						indentLevel,
							   SerializationOptions		options ) const;

	private :

		const std::unique_ptr<const Type>			m_baseType;
	};



	class UnionType : public Type, public SourceElement, public NamespaceScoped
	{
	public :

		UnionType() = delete;
		UnionType( UnionType& ) = delete;
		UnionType( const UnionType& ) = delete;

		UnionType( TypeInfo::Specifier			typeSpec,
				   const std::string&			name,
				   const UID&					uid,
				   const Namespace&				namespaceScope,
				   const SourceLocation&		sourceLocation,
				   ConstListPtr<Attribute>&		attributes )
			: Type( typeSpec, attributes ),
			  SourceElement( name, uid, sourceLocation ),
			  NamespaceScoped( namespaceScope )
		{
			assert( uid.uidType() == UID::UIDType::TYPE );
		}


		virtual ~UnionType()
		{}


		Kind										kind() const
		{
			return( Kind::UNION );
		}


		std::ostream&	toXML( std::ostream&			outputStream,
							   int						indentLevel,
							   SerializationOptions		options ) const;
	};

}


#endif /* TYPES_H_ */
