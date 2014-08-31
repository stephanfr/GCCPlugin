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
#include <boost/optional.hpp>

#include "Serialization.h"
#include "Static.h"
#include "SourceElement.h"
#include "Attribute.h"
#include "Namespace.h"
#include "Types.h"
#include "ParameterValues.h"




namespace CPPModel
{

	class DictionaryFunctionEntry;
	class DictionaryClassEntry;
	class DictionaryUnionEntry;




	class ArrayDimensions
	{
	public :

		ArrayDimensions()
			: m_dimensions( -1 )
		{}

		ArrayDimensions( const ArrayDimensions&		dimensions )
			: m_dimensions( dimensions.m_dimensions )
		{}

		ArrayDimensions( long		dimension )
			: m_dimensions( dimension )
		{}


		const long 			dimensions() const
		{
			return( m_dimensions );
		}

	private :

		const long			m_dimensions;

	};


	class DeclarationBase : public NamedEntity, public NamespaceScoped, public Static, public IAttributes
	{
	public :

		DeclarationBase() = delete;
		DeclarationBase( DeclarationBase& ) = delete;
		DeclarationBase( const DeclarationBase& ) = delete;

		DeclarationBase( const char*				name,
					     const Namespace&			namespaceScope,
					     bool						isStatic )
			: NamedEntity( name ),
			  NamespaceScoped( namespaceScope ),
			  Static( isStatic )
		{}

		DeclarationBase( const char*				name,
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

		enum class Kind { FUNDAMENTAL, FUNCTION, CLASS, UNION };
		enum class Modifier { VALUE, POINTER, ARRAY };

		virtual ~IDeclarationType() {};


		virtual const Kind&					kind() const = 0;
		virtual const Modifier&				modifier() const = 0;


		virtual const TypeSpecifier&		typeSpecifier() const = 0;
	};




	class GlobalVarDeclaration : public DeclarationBase, public IDeclarationType
	{
	public :

		GlobalVarDeclaration() = delete;
		GlobalVarDeclaration( GlobalVarDeclaration& ) = delete;
		GlobalVarDeclaration( const GlobalVarDeclaration& 	declToCopy ) = delete;

		GlobalVarDeclaration( const Kind					kind,
							  const Modifier				modifier,
							  const TypeSpecifier 			typeSpec,
							  const char*					name,
							  const Namespace&				namespaceScope )
			: DeclarationBase( name, namespaceScope, true ),
			  m_kind( kind ),
			  m_modifier( modifier ),
			  m_type( typeSpec )
		{}

		GlobalVarDeclaration( const Kind					kind,
				  	  	  	  const Modifier				modifier,
				  	  	  	  const TypeSpecifier 			typeSpec,
				  	  	  	  const char*					name,
				  	  	  	  const Namespace&				namespaceScope,
				  	  	  	  const Attributes&				attributes )
			: DeclarationBase( name, namespaceScope, true, attributes ),
			  m_kind( kind ),
			  m_modifier( modifier ),
			  m_type( typeSpec )
		{}


		virtual ~GlobalVarDeclaration() {};



		const Kind&								kind() const
		{
			return( m_kind );
		}

		const Modifier&							modifier() const
		{
			 return( m_modifier );
		}


		const TypeSpecifier&					typeSpecifier() const
		{
			return( m_type );
		}



		std::ostream&	toXML( std::ostream&			outputStream,
							   SerializationOptions		options ) const;


	private :

		const Kind							m_kind;

		const Modifier						m_modifier;

		const TypeSpecifier					m_type;
	};



	class FundamentalGlobalVarDeclarationBase : public GlobalVarDeclaration
	{
	public :

		FundamentalGlobalVarDeclarationBase() = delete;
		FundamentalGlobalVarDeclarationBase( FundamentalGlobalVarDeclarationBase& ) = delete;
		FundamentalGlobalVarDeclarationBase( const FundamentalGlobalVarDeclarationBase& ) = delete;

		FundamentalGlobalVarDeclarationBase( const Modifier				modifier,
											 const TypeSpecifier 		typeSpec,
											 const char*				name,
											 const Namespace&			namespaceScope )
			: GlobalVarDeclaration(  IDeclarationType::Kind::FUNDAMENTAL, modifier, typeSpec, name, namespaceScope )
		{}

		FundamentalGlobalVarDeclarationBase( const Modifier				modifier,
											 const TypeSpecifier 		typeSpec,
											 const char*				name,
											 const Namespace&			namespaceScope,
											 const Attributes&			attributes )
			: GlobalVarDeclaration(  IDeclarationType::Kind::FUNDAMENTAL, modifier, typeSpec, name, namespaceScope, attributes )
		{}


		virtual ~FundamentalGlobalVarDeclarationBase() {};


		virtual bool							hasInitialValue() const = 0;

		virtual const ParameterValueBase&		initialValue() const = 0;

		virtual const ArrayDimensions&			arrayDimensions() const = 0;




		std::ostream&	toXML( std::ostream&			outputStream,
							   SerializationOptions		options ) const;

	};



	template <IDeclarationType::Modifier typeModifier, TypeSpecifier typeSpec, class I, class F> class FundamentalGlobalVarDeclaration : public FundamentalGlobalVarDeclarationBase
	{
	public :

		FundamentalGlobalVarDeclaration() = delete;
		FundamentalGlobalVarDeclaration( FundamentalGlobalVarDeclaration< typeModifier, typeSpec,I,F>& ) = delete;
		FundamentalGlobalVarDeclaration( const FundamentalGlobalVarDeclaration< typeModifier, typeSpec,I,F>& ) = delete;


		FundamentalGlobalVarDeclaration( const char*					name,
							  	  	     const Namespace&				namespaceScope )
			: FundamentalGlobalVarDeclarationBase( typeModifier, typeSpec, name, namespaceScope ),
			  m_arrayDimensions()
		{}

		FundamentalGlobalVarDeclaration( const char*					name,
							  	  	     const Namespace&				namespaceScope,
							  	  	     const Attributes&				attributes )
			: FundamentalGlobalVarDeclarationBase( typeModifier, typeSpec, name, namespaceScope, attributes ),
			  m_arrayDimensions()
		{}

		FundamentalGlobalVarDeclaration( const char*					name,
							  	  	     const Namespace&				namespaceScope,
							  	  	     I								initialValue )
			: FundamentalGlobalVarDeclarationBase( typeModifier, typeSpec, name, namespaceScope ),
			  m_arrayDimensions(),
			  m_initialValue( new F( initialValue ))
		{}

		FundamentalGlobalVarDeclaration( const char*					name,
							  	  	     const Namespace&				namespaceScope,
							  	  	     const Attributes&				attributes,
							  	  	     I								initialValue )
			: FundamentalGlobalVarDeclarationBase( typeModifier, typeSpec, name, namespaceScope, attributes ),
			  m_arrayDimensions(),
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

		const ArrayDimensions&					arrayDimensions() const
		{
			 return( m_arrayDimensions );
		}



		std::ostream&	toXML( std::ostream&			outputStream,
							   SerializationOptions		options ) const;


	protected :

		FundamentalGlobalVarDeclaration( const char*					name,
							  	  	     const Namespace&				namespaceScope,
							  	  	     const ArrayDimensions&			arrayDimensions )
			: FundamentalGlobalVarDeclarationBase( typeModifier, typeSpec, name, namespaceScope ),
			  m_arrayDimensions( arrayDimensions )
		{}

		FundamentalGlobalVarDeclaration( const char*					name,
							  	  	     const Namespace&				namespaceScope,
							  	  	     const ArrayDimensions&			arrayDimensions,
							  	  	     const Attributes&				attributes )
			: FundamentalGlobalVarDeclarationBase( typeModifier, typeSpec, name, namespaceScope, attributes ),
			  m_arrayDimensions( arrayDimensions )
		{}

		FundamentalGlobalVarDeclaration( const char*					name,
							  	  	     const Namespace&				namespaceScope,
							  	  	     const ArrayDimensions&			arrayDimensions,
							  	  	     I								initialValue )
			: FundamentalGlobalVarDeclarationBase( typeModifier, typeSpec, name, namespaceScope ),
			  m_arrayDimensions( arrayDimensions ),
			  m_initialValue( new F( initialValue ))
		{}

		FundamentalGlobalVarDeclaration( const char*					name,
							  	  	     const Namespace&				namespaceScope,
							  	  	     const ArrayDimensions&			arrayDimensions,
							  	  	     const Attributes&				attributes,
							  	  	     I								initialValue )
			: FundamentalGlobalVarDeclarationBase( typeModifier, typeSpec, name, namespaceScope, attributes ),
			  m_arrayDimensions( arrayDimensions ),
			  m_initialValue( new F( initialValue ))
		{}


	private :

		const ArrayDimensions							m_arrayDimensions;

		std::unique_ptr<ParameterValueBase>				m_initialValue;
	};




	template <IDeclarationType::Modifier typeModifier, TypeSpecifier typeSpec, class I, class F> class FundamentalGlobalArrayVarDeclaration : public FundamentalGlobalVarDeclaration<typeModifier, typeSpec, I, F>
	{
	public :

		FundamentalGlobalArrayVarDeclaration() = delete;
		FundamentalGlobalArrayVarDeclaration( FundamentalGlobalArrayVarDeclaration< typeModifier, typeSpec,I,F>& ) = delete;
		FundamentalGlobalArrayVarDeclaration( const FundamentalGlobalArrayVarDeclaration< typeModifier, typeSpec,I,F>& ) = delete;


		FundamentalGlobalArrayVarDeclaration( const char*					name,
							  	  	     	  const Namespace&				namespaceScope,
							  	  	     	  const ArrayDimensions&		arrayDimensions )
			: FundamentalGlobalVarDeclaration<typeModifier, typeSpec, I, F>( name, namespaceScope, arrayDimensions )
		{}

		FundamentalGlobalArrayVarDeclaration( const char*					name,
							  	  	     	  const Namespace&				namespaceScope,
							  	  	     	  const ArrayDimensions&		arrayDimensions,
							  	  	     	  const Attributes&				attributes )
			: FundamentalGlobalVarDeclaration<typeModifier, typeSpec, I, F>( name, namespaceScope, arrayDimensions, attributes )
		{}

		FundamentalGlobalArrayVarDeclaration( const char*					name,
							  	  	     	  const Namespace&				namespaceScope,
//							  	  	     	  const ArrayDimensions&		arrayDimensions,
							  	  	     	  I								initialValue )
			: FundamentalGlobalVarDeclaration<typeModifier, typeSpec, I, F>( name, namespaceScope, ArrayDimensions( initialValue.size() ), initialValue )
		{}

		FundamentalGlobalArrayVarDeclaration( const char*					name,
							  	  	     	  const Namespace&				namespaceScope,
//							  	  	     	  const ArrayDimensions&		arrayDimensions,
							  	  	     	  const Attributes&				attributes,
							  	  	     	  I								initialValue )
			: FundamentalGlobalVarDeclaration<typeModifier, typeSpec, I, F>( name, namespaceScope, ArrayDimensions( initialValue.size() ), attributes, initialValue )
		{}


		virtual ~FundamentalGlobalArrayVarDeclaration()
		{}



		std::ostream&	toXML( std::ostream&			outputStream,
							   SerializationOptions		options ) const;
	};





	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Modifier::VALUE, CPPModel::TypeSpecifier::BOOLEAN, bool, BooleanConstantParameter> BooleanGlobalVarDeclaration;
	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Modifier::VALUE, CPPModel::TypeSpecifier::CHAR, char, CharConstantParameter> CharGlobalVarDeclaration;
	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Modifier::VALUE, CPPModel::TypeSpecifier::STRING, char*, StringConstantParameter> StringGlobalVarDeclaration;
	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Modifier::VALUE, CPPModel::TypeSpecifier::INT, int, IntConstantParameter> IntGlobalVarDeclaration;
	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Modifier::VALUE, CPPModel::TypeSpecifier::LONG_INT, long, LongConstantParameter> LongGlobalVarDeclaration;
	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Modifier::VALUE, CPPModel::TypeSpecifier::FLOAT, float, FloatConstantParameter> FloatGlobalVarDeclaration;
	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Modifier::VALUE, CPPModel::TypeSpecifier::DOUBLE, double, DoubleConstantParameter> DoubleGlobalVarDeclaration;

	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Modifier::POINTER, CPPModel::TypeSpecifier::BOOLEAN, UID&, BooleanPointerParameter> BooleanPointerGlobalVarDeclaration;
	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Modifier::POINTER, CPPModel::TypeSpecifier::CHAR, UID&, CharPointerParameter> CharPointerGlobalVarDeclaration;
	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Modifier::POINTER, CPPModel::TypeSpecifier::STRING, UID&, StringPointerParameter> StringPointerGlobalVarDeclaration;
	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Modifier::POINTER, CPPModel::TypeSpecifier::INT, UID&, IntPointerParameter> IntPointerGlobalVarDeclaration;
	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Modifier::POINTER, CPPModel::TypeSpecifier::LONG_INT, UID&, LongPointerParameter> LongPointerGlobalVarDeclaration;
	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Modifier::POINTER, CPPModel::TypeSpecifier::FLOAT, UID&, FloatPointerParameter> FloatPointerGlobalVarDeclaration;
	typedef FundamentalGlobalVarDeclaration< IDeclarationType::Modifier::POINTER, CPPModel::TypeSpecifier::DOUBLE, UID&, DoublePointerParameter> DoublePointerGlobalVarDeclaration;

	typedef FundamentalGlobalArrayVarDeclaration< IDeclarationType::Modifier::ARRAY, CPPModel::TypeSpecifier::BOOLEAN, std::vector<bool>, BooleanArrayParameter> BoolArrayGlobalVarDeclaration;
	typedef FundamentalGlobalArrayVarDeclaration< IDeclarationType::Modifier::ARRAY, CPPModel::TypeSpecifier::CHAR, std::vector<char>, CharArrayParameter> CharArrayGlobalVarDeclaration;
	typedef FundamentalGlobalArrayVarDeclaration< IDeclarationType::Modifier::ARRAY, CPPModel::TypeSpecifier::STRING, std::vector<std::string>, StringArrayParameter> StringArrayGlobalVarDeclaration;
	typedef FundamentalGlobalArrayVarDeclaration< IDeclarationType::Modifier::ARRAY, CPPModel::TypeSpecifier::INT, std::vector<int>, IntArrayParameter> IntArrayGlobalVarDeclaration;
	typedef FundamentalGlobalArrayVarDeclaration< IDeclarationType::Modifier::ARRAY, CPPModel::TypeSpecifier::LONG_INT, std::vector<long>, LongArrayParameter> LongArrayGlobalVarDeclaration;
	typedef FundamentalGlobalArrayVarDeclaration< IDeclarationType::Modifier::ARRAY, CPPModel::TypeSpecifier::FLOAT, std::vector<float>, FloatArrayParameter> FloatArrayGlobalVarDeclaration;
	typedef FundamentalGlobalArrayVarDeclaration< IDeclarationType::Modifier::ARRAY, CPPModel::TypeSpecifier::DOUBLE, std::vector<double>, DoubleArrayParameter> DoubleArrayGlobalVarDeclaration;




	class ClassGlobalVarDeclaration : public GlobalVarDeclaration
	{
	public :

		ClassGlobalVarDeclaration() = delete;
		ClassGlobalVarDeclaration( ClassGlobalVarDeclaration& ) = delete;
		ClassGlobalVarDeclaration( const ClassGlobalVarDeclaration& ) = delete;

		ClassGlobalVarDeclaration( const char*						name,
			  	   	   	   	   	   const DictionaryClassEntry&		classType,
							  	   const Namespace&					namespaceScope )
			: GlobalVarDeclaration( Kind::CLASS, IDeclarationType::Modifier::VALUE, TypeSpecifier::CLASS, name, namespaceScope ),
			  m_classType( classType )
		{}

		ClassGlobalVarDeclaration( const char*						name,
			  	   	   	   	   	   const DictionaryClassEntry&		classType,
							  	   const Namespace&					namespaceScope,
							  	   const Attributes&				attributes )
			: GlobalVarDeclaration( Kind::CLASS, IDeclarationType::Modifier::VALUE, TypeSpecifier::CLASS, name, namespaceScope, attributes ),
			  m_classType( classType )
		{}

		ClassGlobalVarDeclaration( const char*						name,
			  	   	   	   	   	   const DictionaryClassEntry&		classType,
							  	   const Namespace&					namespaceScope,
							  	   const Attributes&				attributes,
							  	   const ParameterValueList&		initialValues )
			: GlobalVarDeclaration( Kind::CLASS, IDeclarationType::Modifier::VALUE, TypeSpecifier::CLASS, name, namespaceScope, attributes ),
			  m_classType( classType ),
			  m_initialValues( initialValues )
		{}

		ClassGlobalVarDeclaration( const char*						name,
			  	   	   	   	   	   const DictionaryClassEntry&		classType,
							  	   const Namespace&					namespaceScope,
							  	   const ParameterValueList&		initialValues )
			: GlobalVarDeclaration( Kind::CLASS, IDeclarationType::Modifier::VALUE, TypeSpecifier::CLASS, name, namespaceScope ),
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




	class FunctionPrototype
	{
	public :

		FunctionPrototype( const Type&		returnType )
			: m_returnType( returnType.deepCopy() )
		{}

		FunctionPrototype( const Type&		returnType,
						   const TypeList&	arguments )
			: m_returnType( returnType.deepCopy() )
		{
			for( TypeList::const_iterator itrArg = arguments.begin(); itrArg != arguments.end(); itrArg++ )
			{
				Type*		argCopy = (*itrArg).deepCopy();
				m_arguments.push_back( argCopy );
			}
		}


		FunctionPrototype( const FunctionPrototype&		prototypeToCopy )
			: m_returnType( prototypeToCopy.m_returnType->deepCopy() )
		{
			for( TypeList::const_iterator itrArg = prototypeToCopy.m_arguments.begin(); itrArg != prototypeToCopy.m_arguments.end(); itrArg++ )
			{
				Type*		argCopy = (*itrArg).deepCopy();
				m_arguments.push_back( argCopy );
			}
		}



		const Type&				returnType() const
		{
			return( *m_returnType );
		}

		const TypeList&			arguments() const
		{
			return( m_arguments );
		}


	private :

		const std::unique_ptr<Type>		m_returnType;
		TypeList						m_arguments;
	};



	class FunctionPointerGlobalVarDeclaration : public GlobalVarDeclaration
	{
	public :

		FunctionPointerGlobalVarDeclaration() = delete;
		FunctionPointerGlobalVarDeclaration( FunctionPointerGlobalVarDeclaration& ) = delete;
		FunctionPointerGlobalVarDeclaration( const FunctionPointerGlobalVarDeclaration& ) = delete;

		FunctionPointerGlobalVarDeclaration( const char*						name,
 	   	   	     	 	 	 	 	 	 	 const FunctionPrototype&			prototype,
							  	   	   	     const Namespace&					namespaceScope )
			: GlobalVarDeclaration( Kind::FUNCTION, IDeclarationType::Modifier::POINTER, TypeSpecifier::FUNCTION, name, namespaceScope ),
			  m_prototype( prototype )
		{}

		FunctionPointerGlobalVarDeclaration( const char*						name,
 	   	   	   	 	 	 	 	 	 	 	 const FunctionPrototype&			prototype,
							  	   	   	   	 const Namespace&					namespaceScope,
							  	   	   	   	 const Attributes&					attributes )
			: GlobalVarDeclaration( Kind::FUNCTION, IDeclarationType::Modifier::POINTER, TypeSpecifier::FUNCTION, name, namespaceScope, attributes ),
			  m_prototype( prototype )
		{}

		FunctionPointerGlobalVarDeclaration( const char*						name,
			     	 	 	 	 	 	 	 const FunctionPrototype&			prototype,
										     const Namespace&					namespaceScope,
										     const Attributes&					attributes,
										     const UID&							initialValue )
			: GlobalVarDeclaration( Kind::FUNCTION, IDeclarationType::Modifier::POINTER, TypeSpecifier::FUNCTION, name, namespaceScope, attributes ),
			  m_prototype( prototype ),
			  m_initialValue( initialValue )
		{}

		FunctionPointerGlobalVarDeclaration( const char*						name,
			     	 	 	 	 	 	 	 const FunctionPrototype&			prototype,
										     const Namespace&					namespaceScope,
										     const UID&							initialValue )
			: GlobalVarDeclaration( Kind::FUNCTION, IDeclarationType::Modifier::POINTER, TypeSpecifier::FUNCTION, name, namespaceScope ),
			  m_prototype( prototype ),
			  m_initialValue( initialValue )
		{}



		virtual ~FunctionPointerGlobalVarDeclaration() {};





		const FunctionPrototype&				prototype() const
		{
			return( m_prototype );
		}


		bool									hasInitialValue() const
		{
			return( m_initialValue.is_initialized() );
		}

		const UID&								initialValue() const
		{
			return( m_initialValue.get() );
		}


	private :

		const FunctionPrototype&			m_prototype;

		const boost::optional<UID>			m_initialValue;
	};


}


#endif /* DECLARATIONS_H_ */


