/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/


#ifndef DECLARATIONS_H_
#define DECLARATIONS_H_



#include <memory>

#include "Serialization.h"
#include "Static.h"
#include "SourceElement.h"
#include "Attribute.h"
#include "Namespace.h"
#include "Types.h"
#include "ParameterValues.h"




namespace CPPModel
{

	class DictionaryClassEntry;
	class DictionaryUnionEntry;



	class DeclarationBase : public NamedEntity, public NamespaceScoped, public Static, public IAttributes
	{
	public :

		DeclarationBase() = delete;
		DeclarationBase( DeclarationBase& ) = delete;
		DeclarationBase( const DeclarationBase& ) = delete;

		DeclarationBase( const std::string&			name,
					     const Namespace&			namespaceScope,
					     bool						isStatic )
			: NamedEntity( name ),
			  NamespaceScoped( namespaceScope ),
			  Static( isStatic )
		{}

		DeclarationBase( const std::string&			name,
					     const Namespace&			namespaceScope,
					     bool						isStatic,
					     const Attributes&			attributes )
			: NamedEntity( name ),
			  NamespaceScoped( namespaceScope ),
			  Static( isStatic ),
			  m_attributes( attributes )
		{}

		virtual ~DeclarationBase() {};


		const Attributes&				attributes() const
		{
			return( m_attributes );
		}

	private :

		const Attributes							m_attributes;
	};



	class IDeclarationType
	{
	public :

		enum class Kind { FUNDAMENTAL_VALUE, FUNDAMENTAL_POINTER, CLASS, UNION, ARRAY };

		virtual ~IDeclarationType() {};


		virtual const Kind&					kind() const = 0;


		virtual const TypeSpecifier&		typeSpecifier() const = 0;
	};




	class GlobalVarDeclaration : public DeclarationBase, public IDeclarationType
	{
	public :

		GlobalVarDeclaration() = delete;
		GlobalVarDeclaration( GlobalVarDeclaration& ) = delete;
		GlobalVarDeclaration( const GlobalVarDeclaration& 	declToCopy ) = delete;

		GlobalVarDeclaration( const Kind					kind,
							  const TypeSpecifier 			typeSpec,
							  const std::string&			name,
							  const Namespace&				namespaceScope )
			: DeclarationBase( name, namespaceScope, true ),
			  m_kind( kind ),
			  m_type( typeSpec )
		{}

		GlobalVarDeclaration( const Kind					kind,
				  	  	  	  const TypeSpecifier 			typeSpec,
				  	  	  	  const std::string&			name,
				  	  	  	  const Namespace&				namespaceScope,
				  	  	  	  const Attributes&				attributes )
			: DeclarationBase( name, namespaceScope, true, attributes ),
			  m_kind( kind ),
			  m_type( typeSpec )
		{}


		virtual ~GlobalVarDeclaration() {};



		const Kind&								kind() const
		{
			return( m_kind );
		}


		const TypeSpecifier&					typeSpecifier() const
		{
			return( m_type );
		}



		std::ostream&	toXML( std::ostream&			outputStream,
							   SerializationOptions		options ) const;


	private :

		const Kind							m_kind;

		const TypeSpecifier					m_type;
	};



	class FundamentalGlobalVarDeclarationBase : public GlobalVarDeclaration
	{
	public :

		FundamentalGlobalVarDeclarationBase() = delete;
		FundamentalGlobalVarDeclarationBase( FundamentalGlobalVarDeclarationBase& ) = delete;
		FundamentalGlobalVarDeclarationBase( const FundamentalGlobalVarDeclarationBase& ) = delete;

		FundamentalGlobalVarDeclarationBase( const Kind					kind,
											 const TypeSpecifier 		typeSpec,
											 const std::string&			name,
											 const Namespace&			namespaceScope )
			: GlobalVarDeclaration( kind, typeSpec, name, namespaceScope )
		{}

		FundamentalGlobalVarDeclarationBase( const Kind					kind,
											 const TypeSpecifier 		typeSpec,
											 const std::string&			name,
											 const Namespace&			namespaceScope,
											 const Attributes&			attributes )
			: GlobalVarDeclaration( kind, typeSpec, name, namespaceScope, attributes )
		{}


		virtual ~FundamentalGlobalVarDeclarationBase() {};


		virtual bool							hasInitialValue() const = 0;

		virtual const ParameterValueBase&		initialValue() const = 0;



		std::ostream&	toXML( std::ostream&			outputStream,
							   SerializationOptions		options ) const;

	};



	template <IDeclarationType::Kind typeKind, TypeSpecifier typeSpec, class I, class F> class FundamentalGlobalVarDeclaration : public FundamentalGlobalVarDeclarationBase
	{
	public :

		FundamentalGlobalVarDeclaration() = delete;
		FundamentalGlobalVarDeclaration( FundamentalGlobalVarDeclaration<typeKind, typeSpec,I,F>& ) = delete;
		FundamentalGlobalVarDeclaration( const FundamentalGlobalVarDeclaration<typeKind, typeSpec,I,F>& ) = delete;


		FundamentalGlobalVarDeclaration( const std::string&				name,
							  	  	     const Namespace&				namespaceScope )
			: FundamentalGlobalVarDeclarationBase( typeKind, typeSpec, name, namespaceScope )
		{}

		FundamentalGlobalVarDeclaration( const std::string&				name,
							  	  	     const Namespace&				namespaceScope,
							  	  	     const Attributes&				attributes )
			: FundamentalGlobalVarDeclarationBase( typeKind, typeSpec, name, namespaceScope, attributes )
		{}

		FundamentalGlobalVarDeclaration( const std::string&				name,
							  	  	     const Namespace&				namespaceScope,
							  	  	     I								initialValue )
			: FundamentalGlobalVarDeclarationBase( typeKind, typeSpec, name, namespaceScope ),
			  m_initialValue( new F( initialValue ))
		{}

		FundamentalGlobalVarDeclaration( const std::string&				name,
							  	  	     const Namespace&				namespaceScope,
							  	  	     const Attributes&				attributes,
							  	  	     I								initialValue )
			: FundamentalGlobalVarDeclarationBase( typeKind, typeSpec, name, namespaceScope, attributes ),
			  m_initialValue( new F( initialValue ))
		{}


		virtual ~FundamentalGlobalVarDeclaration()
		{}




		bool									hasInitialValue() const
		{
			return( (bool)m_initialValue );
		}

		const ParameterValueBase&				initialValue() const
		{
			return( *m_initialValue );
		}

		std::ostream&	toXML( std::ostream&			outputStream,
							   SerializationOptions		options ) const;


	private :

		std::unique_ptr<ParameterValueBase>				m_initialValue;
	};




	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Kind::FUNDAMENTAL_VALUE, CPPModel::TypeSpecifier::BOOLEAN, bool, ParameterBooleanValue> BooleanGlobalVarDeclaration;
	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Kind::FUNDAMENTAL_VALUE, CPPModel::TypeSpecifier::CHAR, char, ParameterCharValue> CharGlobalVarDeclaration;
	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Kind::FUNDAMENTAL_VALUE, CPPModel::TypeSpecifier::STRING, char*, ParameterStringValue> StringGlobalVarDeclaration;
	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Kind::FUNDAMENTAL_VALUE, CPPModel::TypeSpecifier::INT, int, ParameterIntValue> IntGlobalVarDeclaration;
	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Kind::FUNDAMENTAL_VALUE, CPPModel::TypeSpecifier::LONG_INT, long, ParameterLongValue> LongGlobalVarDeclaration;
	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Kind::FUNDAMENTAL_VALUE, CPPModel::TypeSpecifier::FLOAT, float, ParameterFloatValue> FloatGlobalVarDeclaration;
	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Kind::FUNDAMENTAL_VALUE, CPPModel::TypeSpecifier::DOUBLE, double, ParameterDoubleValue> DoubleGlobalVarDeclaration;

	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Kind::FUNDAMENTAL_POINTER, CPPModel::TypeSpecifier::BOOLEAN, bool, ParameterBooleanValue> BooleanPointerGlobalVarDeclaration;
//	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Kind::FUNDAMENTAL_POINTER, CPPModel::TypeSpecifier::CHAR, char*, ParameterCharPointerConstValue> CharPointerConstGlobalVarDeclaration;
	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Kind::FUNDAMENTAL_POINTER, CPPModel::TypeSpecifier::INT, int, ParameterIntValue> IntPointerGlobalVarDeclaration;
	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Kind::FUNDAMENTAL_POINTER, CPPModel::TypeSpecifier::LONG_INT, long, ParameterLongValue> LongPointerGlobalVarDeclaration;
	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Kind::FUNDAMENTAL_POINTER, CPPModel::TypeSpecifier::FLOAT, float, ParameterFloatValue> FloatPointerGlobalVarDeclaration;
	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Kind::FUNDAMENTAL_POINTER, CPPModel::TypeSpecifier::DOUBLE, double, ParameterDoubleValue> DoublePointerGlobalVarDeclaration;



	class ClassGlobalVarDeclaration : public GlobalVarDeclaration
	{
	public :

		ClassGlobalVarDeclaration() = delete;
		ClassGlobalVarDeclaration( ClassGlobalVarDeclaration& ) = delete;
		ClassGlobalVarDeclaration( const ClassGlobalVarDeclaration& ) = delete;

		ClassGlobalVarDeclaration( const DictionaryClassEntry&		classType,
							  	   const std::string&				name,
							  	   const Namespace&					namespaceScope )
			: GlobalVarDeclaration( Kind::CLASS, TypeSpecifier::CLASS, name, namespaceScope ),
			  m_classType( classType )
		{}

		ClassGlobalVarDeclaration( const DictionaryClassEntry&		classType,
							  	   const std::string&				name,
							  	   const Namespace&					namespaceScope,
							  	   const Attributes&				attributes )
			: GlobalVarDeclaration( Kind::CLASS, TypeSpecifier::CLASS, name, namespaceScope, attributes ),
			  m_classType( classType )
		{}

		ClassGlobalVarDeclaration( const DictionaryClassEntry&		classType,
							  	   const std::string&				name,
							  	   const Namespace&					namespaceScope,
							  	   const Attributes&				attributes,
							  	   const ParameterValueList&		initialValues )
			: GlobalVarDeclaration( Kind::CLASS, TypeSpecifier::CLASS, name, namespaceScope, attributes ),
			  m_classType( classType ),
			  m_initialValues( initialValues )
		{}

		ClassGlobalVarDeclaration( const DictionaryClassEntry&		classType,
							  	   const std::string&				name,
							  	   const Namespace&					namespaceScope,
							  	   const ParameterValueList&		initialValues )
			: GlobalVarDeclaration( Kind::CLASS, TypeSpecifier::CLASS, name, namespaceScope ),
			  m_classType( classType ),
			  m_initialValues( initialValues )
		{}



		virtual ~ClassGlobalVarDeclaration() {};





		const DictionaryClassEntry&			classType() const
		{
			return( m_classType );
		}


		bool								hasInitialValue() const
		{
			return( true );
		}

		const ParameterValueList&			initialValues() const
		{
			return( m_initialValues );
		}


	private :

		const DictionaryClassEntry&			m_classType;

		const ParameterValueList			m_initialValues;
	};



}


#endif /* DECLARATIONS_H_ */


