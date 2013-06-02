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

	class Type : virtual public XMLSerializable, public Attributes
	{
	public :

		enum class Kind { FUNDAMENTAL, USER_DEFINED, DERIVED, UNION, UNRECOGNIZED };


		Type() = delete;
		Type( Type& ) = delete;
		Type( const Type& ) = delete;

		Type( TypeInfo::Specifier		typeSpec )
			: Attributes(),
			  m_typeSpecifier( typeSpec )
		{}

		Type( TypeInfo::Specifier		typeSpec,
			  ConstListPtr<Attribute>&	attributes )
			: Attributes( attributes ),
			  m_typeSpecifier( typeSpec )
		{}


		virtual ~Type()
		{}



		TypeInfo::Specifier				type() const
		{
			return( m_typeSpecifier );
		}

		virtual Kind					kind() const = 0;


	private :

		const TypeInfo::Specifier		m_typeSpecifier;
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



	class UserDefinedType : public Type, public SourceElement, public Namespace
	{
	public :

		UserDefinedType() = delete;
		UserDefinedType( UserDefinedType& ) = delete;
		UserDefinedType( const UserDefinedType& ) = delete;

		UserDefinedType( TypeInfo::Specifier			typeSpec,
						 const std::string&				name,
						 const UID&						uid,
						 const std::string&				enclosingNamespace,
						 const SourceLocation&			sourceLocation,
						 ConstListPtr<Attribute>&		attributes )
			: Type( typeSpec, attributes ),
			  SourceElement( name, uid, sourceLocation ),
			  Namespace( enclosingNamespace )
		{
			assert( uid.type() == UID::UIDType::TYPE );
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



	class UnionType : public Type, public SourceElement, public Namespace
	{
	public :

		UnionType() = delete;
		UnionType( UnionType& ) = delete;
		UnionType( const UnionType& ) = delete;

		UnionType( TypeInfo::Specifier			typeSpec,
				   const std::string&			name,
				   const UID&					uid,
				   const std::string&			enclosingNamespace,
				   const SourceLocation&		sourceLocation,
				   ConstListPtr<Attribute>&		attributes )
			: Type( typeSpec, attributes ),
			  SourceElement( name, uid, sourceLocation ),
			  Namespace( enclosingNamespace )
		{
			assert( uid.type() == UID::UIDType::TYPE );
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
