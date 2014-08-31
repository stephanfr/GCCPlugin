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


#include "Serialization.h"
#include "Attribute.h"
#include "Namespace.h"


namespace CPPModel
{


	enum class TypeSpecifier
	{
		UNRECOGNIZED = 0,
		VOID,
		ENUM,
		CHAR,
		UNSIGNED_CHAR,
		SHORT_INT,
		UNSIGNED_SHORT_INT,
		INT,
		UNSIGNED_INT,
		LONG_INT,
		UNSIGNED_LONG_INT,
		FLOAT,
		DOUBLE,
		LNG_DOUBLE,
		BOOLEAN,
		FUNCTION,
		METHOD,
		CLASS,
		POINTER,
		REFERENCE,
		ARRAY,
		UNION,
		NULL_POINTER,		//	NULL_POINTER is the last GCC specific type - any after this are not present in GCC AST
		NO_RETURN,
		STRING
	};



	struct TypeInfo
	{
		enum class Classification { FUNDAMENTAL, CLASS, DERIVED, FUNCTION, UNION, UNRECOGNIZED };

		const TypeSpecifier		specifier;
		const Classification	classification;
		const std::string		label;
	};

	static const TypeInfo		CPPTypes[(int)TypeSpecifier::NO_RETURN + 1] =		//	Make sure the size of the array matches the size of the enum above
	{
			{ TypeSpecifier::UNRECOGNIZED, TypeInfo::Classification::UNRECOGNIZED, "unrecognized" },
			{ TypeSpecifier::VOID, TypeInfo::Classification::FUNDAMENTAL, "void" },
			{ TypeSpecifier::ENUM, TypeInfo::Classification::FUNDAMENTAL, "enum" },
			{ TypeSpecifier::CHAR, TypeInfo::Classification::FUNDAMENTAL, "char" },
			{ TypeSpecifier::UNSIGNED_CHAR, TypeInfo::Classification::FUNDAMENTAL, "unsigned char" },
			{ TypeSpecifier::SHORT_INT, TypeInfo::Classification::FUNDAMENTAL, "short int" },
			{ TypeSpecifier::UNSIGNED_SHORT_INT, TypeInfo::Classification::FUNDAMENTAL, "unsigned short int" },
			{ TypeSpecifier::INT, TypeInfo::Classification::FUNDAMENTAL, "int" },
			{ TypeSpecifier::UNSIGNED_INT, TypeInfo::Classification::FUNDAMENTAL, "unsigned int" },
			{ TypeSpecifier::LONG_INT, TypeInfo::Classification::FUNDAMENTAL, "long int" },
			{ TypeSpecifier::UNSIGNED_LONG_INT, TypeInfo::Classification::FUNDAMENTAL, "unsigned long int" },
			{ TypeSpecifier::FLOAT, TypeInfo::Classification::FUNDAMENTAL, "float" },
			{ TypeSpecifier::DOUBLE, TypeInfo::Classification::FUNDAMENTAL, "double" },
			{ TypeSpecifier::LNG_DOUBLE, TypeInfo::Classification::FUNDAMENTAL, "long double" },
			{ TypeSpecifier::BOOLEAN, TypeInfo::Classification::FUNDAMENTAL, "boolean" },
			{ TypeSpecifier::FUNCTION, TypeInfo::Classification::FUNCTION, "function" },
			{ TypeSpecifier::METHOD, TypeInfo::Classification::FUNCTION, "method" },
			{ TypeSpecifier::CLASS, TypeInfo::Classification::CLASS, "class" },
			{ TypeSpecifier::POINTER, TypeInfo::Classification::DERIVED, "pointer" },
			{ TypeSpecifier::REFERENCE, TypeInfo::Classification::DERIVED, "reference" },
			{ TypeSpecifier::ARRAY, TypeInfo::Classification::DERIVED, "array" },
			{ TypeSpecifier::UNION, TypeInfo::Classification::UNION, "union" },
			{ TypeSpecifier::NULL_POINTER, TypeInfo::Classification::FUNDAMENTAL, "null pointer" },
			{ TypeSpecifier::NO_RETURN, TypeInfo::Classification::FUNDAMENTAL, "no return" }
	};




	class Type : public IXMLSerializable, public IAttributes
	{
	public :

		enum class Kind { FUNDAMENTAL, CLASS_OR_STRUCT, DERIVED, UNION, UNRECOGNIZED };


		Type() = delete;
		Type( Type& ) = delete;

		Type( TypeSpecifier				typeSpec )
			: m_typeSpecifier( typeSpec ),
			  m_attributes()
		{}

		Type( TypeSpecifier				typeSpec,
			  ConstListPtr<Attribute>&	attributes )
			: m_typeSpecifier( typeSpec ),
			  m_attributes( attributes )
		{}

		Type( TypeSpecifier				typeSpec,
			  const Attributes&			attributes )
			: m_typeSpecifier( typeSpec ),
			  m_attributes( attributes )
		{}

		Type( const Type&				typeToCopy )
			: m_typeSpecifier( typeToCopy.m_typeSpecifier ),
			  m_attributes( typeToCopy.m_attributes )
		{}


		virtual ~Type()
		{}


		virtual Type*					deepCopy() const = 0;


		TypeSpecifier					typeSpec() const
		{
			return( m_typeSpecifier );
		}

		const Attributes&				attributes() const
		{
			return( m_attributes );
		}

		virtual Kind					kind() const = 0;


		virtual std::ostream&	toXML( std::ostream&			outputStream,
							   	   	   SerializationOptions		options ) const = 0;


	private :

		const TypeSpecifier				m_typeSpecifier;

		const Attributes				m_attributes;
	};



	typedef boost::ptr_list<Type>			TypeList;



	class UnrecognizedType : public Type
	{
	public :

		UnrecognizedType( UnrecognizedType& ) = delete;
		UnrecognizedType( const UnrecognizedType& ) = delete;

		UnrecognizedType()
			: Type( TypeSpecifier::UNRECOGNIZED )
		{}

		virtual ~UnrecognizedType()
		{}


		Type*						deepCopy() const
		{
			return( new UnrecognizedType() );
		}

		Kind	kind() const
		{
			return( Kind::UNRECOGNIZED );
		}

		std::ostream&	toXML( std::ostream&			outputStream,
							   SerializationOptions		options ) const;
	};



	class FundamentalType : public Type
	{
	public :

		FundamentalType() = delete;
		FundamentalType( FundamentalType& ) = delete;

		FundamentalType( TypeSpecifier		typeSpec )
			: Type( typeSpec )
		{}

		FundamentalType( const FundamentalType&		typeToCopy )
			: Type( typeToCopy.typeSpec() )
		{}


		virtual ~FundamentalType()
		{}



		Type*						deepCopy() const
		{
			return( new FundamentalType( *this ) );
		}


		Kind	kind() const
		{
			return( Kind::FUNDAMENTAL );
		}


		std::ostream&	toXML( std::ostream&			outputStream,
							   SerializationOptions		options ) const;
	};



	class ClassOrStructType : public Type, public SourceElement, public NamespaceScoped
	{
	public :

		ClassOrStructType() = delete;
		ClassOrStructType( ClassOrStructType& ) = delete;

		ClassOrStructType( TypeSpecifier				typeSpec,
						   const std::string&			name,
						   const UID&					uid,
						   const Namespace&				namespaceScope,
						   const SourceLocation&		sourceLocation )
			: Type( typeSpec ),
			  SourceElement( name, uid, sourceLocation ),
			  NamespaceScoped( namespaceScope )
		{
			assert( uid.uidType() == UID::UIDType::TYPE );
		}

		ClassOrStructType( TypeSpecifier				typeSpec,
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

		ClassOrStructType( const ClassOrStructType& 	typeToCopy )
			: Type( typeToCopy.typeSpec(), typeToCopy.attributes() ),
			  SourceElement( typeToCopy.sourceElement() ),
			  NamespaceScoped( typeToCopy.namespaceScope() )
		{}



		virtual ~ClassOrStructType()
		{}


		Type*									deepCopy() const
		{
			return( new ClassOrStructType( *this ) );
		}

		Kind									kind() const
		{
			return( Kind::CLASS_OR_STRUCT );
		}

		const std::string						fqName() const
		{
			return( std::string( namespaceScope().fqName() + name() ) );
		}


		std::ostream&	toXML( std::ostream&			outputStream,
							   SerializationOptions		options ) const;
	};




	class DerivedType : public Type
	{
	public :

		DerivedType() = delete;
		DerivedType( DerivedType& ) = delete;

		DerivedType( TypeSpecifier					typeSpec,
					 std::unique_ptr<const Type>	baseType )
			: Type( typeSpec ),
			  m_baseType( std::move( baseType ))
		{}

		DerivedType( TypeSpecifier					typeSpec,
					 ConstListPtr<Attribute>&		attributes,
					 std::unique_ptr<const Type>	baseType )
			: Type( typeSpec, attributes ),
			  m_baseType( std::move( baseType ))
		{}

		DerivedType( TypeSpecifier					typeSpec,
					 const Attributes&				attributes,
					 std::unique_ptr<const Type>	baseType )
			: Type( typeSpec, attributes ),
			  m_baseType( std::move( baseType ))
		{}

		DerivedType( const DerivedType&		typeToCopy )
			: Type( typeToCopy.typeSpec(), typeToCopy.attributes() ),
			  m_baseType( typeToCopy.m_baseType->deepCopy() )
		{

		}


		virtual ~DerivedType()
		{}


		Type*			deepCopy() const
		{
			return( new DerivedType( *this ) );
		}

		Kind			kind() const
		{
			return( Kind::DERIVED );
		}

		const Type&		baseType() const
		{
			return( *m_baseType );
		}

		std::ostream&	toXML( std::ostream&			outputStream,
							   SerializationOptions		options ) const;

	private :

		const std::unique_ptr<const Type>			m_baseType;
	};



	class UnionType : public Type, public SourceElement, public NamespaceScoped
	{
	public :

		UnionType() = delete;
		UnionType( UnionType& ) = delete;

		UnionType( TypeSpecifier				typeSpec,
				   const std::string&			name,
				   const UID&					uid,
				   const Namespace&				namespaceScope,
				   const SourceLocation&		sourceLocation )
			: Type( typeSpec ),
			  SourceElement( name, uid, sourceLocation ),
			  NamespaceScoped( namespaceScope )
		{
			assert( uid.uidType() == UID::UIDType::TYPE );
		}

		UnionType( TypeSpecifier				typeSpec,
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

		UnionType( const UnionType& 	typeToCopy )
			: Type( typeToCopy.typeSpec(), typeToCopy.attributes() ),
			  SourceElement( typeToCopy.sourceElement() ),
			  NamespaceScoped( typeToCopy.namespaceScope() )
		{}


		virtual ~UnionType()
		{}


		Type*									deepCopy() const
		{
			return( new UnionType( *this ) );
		}


		Kind										kind() const
		{
			return( Kind::UNION );
		}


		std::ostream&	toXML( std::ostream&			outputStream,
							   SerializationOptions		options ) const;
	};

}


#endif /* TYPES_H_ */
