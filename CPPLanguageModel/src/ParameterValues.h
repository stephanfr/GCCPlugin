/*
 * FunctionCallParameters.h
 *
 *  Created on: Jan 31, 2014
 *      Author: steve
 */

#ifndef PARAMETERVALUES_H_
#define PARAMETERVALUES_H_


#include <memory>
#include <typeinfo>

#include <boost/ptr_container/ptr_list.hpp>

#include "Types.h"



namespace CPPModel
{
	class ASTDictionaryEntry;


	typedef std::vector<TypeSpecifier>		ParameterType;


	extern const ParameterType		BOOLEAN_PARAM;
	extern const ParameterType		CHAR_PARAM;
	extern const ParameterType		STRING_PARAM;
	extern const ParameterType		INT_PARAM;
	extern const ParameterType		LONG_PARAM;
	extern const ParameterType		FLOAT_PARAM;
	extern const ParameterType		DOUBLE_PARAM;
	extern const ParameterType		CLASS_PARAM;
	extern const ParameterType		FUNCTION_PARAM;


	enum class ParameterModifier
	{
		CONSTANT,
		POINTER,
		ARRAY
	};





	TypeSpecifier	AsTypeSpecifier( const ParameterType&	paramType );


	class ParameterValueBase
	{
	public :

		virtual ~ParameterValueBase()
		{}

		virtual std::unique_ptr<ParameterValueBase>			deepCopy() const = 0;

		virtual const ParameterModifier&					modifier() const = 0;

		virtual const ParameterType&						type() const = 0;
	};




	template<class T, const ParameterModifier paramModifier, const ParameterType& paramType> class ParameterValue : public ParameterValueBase
	{
	public :

		ParameterValue( const T&				value )
		: m_modifier( paramModifier ),
		  m_paramType( paramType ),
		  m_value( value )
		{}


		ParameterValue( const ParameterValue&		valueToCopy )
		: m_modifier( valueToCopy.m_modifier ),
		  m_value( valueToCopy.m_value ),
		  m_paramType( valueToCopy.m_paramType )
		{}


		std::unique_ptr<ParameterValueBase>			deepCopy() const
		{
			return( std::unique_ptr<ParameterValueBase>( new ParameterValue<T,paramModifier,paramType>( m_value ) ));
		}



		const ParameterModifier&					modifier() const
		{
			return( m_modifier );
		}

		const ParameterType&						type() const
		{
			return( m_paramType );
		}



		const T&		value() const
		{
			return( m_value );
		}


	private :

		const ParameterModifier		m_modifier;
		const ParameterType&		m_paramType;
		T							m_value;

	};




	typedef ParameterValue< bool, ParameterModifier::CONSTANT, BOOLEAN_PARAM>									BooleanConstantParameter;
	typedef ParameterValue< char, ParameterModifier::CONSTANT, CHAR_PARAM>										CharConstantParameter;
	typedef ParameterValue< std::string, ParameterModifier::CONSTANT, STRING_PARAM>								StringConstantParameter;
	typedef ParameterValue< int, ParameterModifier::CONSTANT, INT_PARAM>										IntConstantParameter;
	typedef ParameterValue< long, ParameterModifier::CONSTANT, LONG_PARAM>										LongConstantParameter;
	typedef ParameterValue< float, ParameterModifier::CONSTANT, FLOAT_PARAM>									FloatConstantParameter;
	typedef ParameterValue< double, ParameterModifier::CONSTANT, DOUBLE_PARAM>									DoubleConstantParameter;
	typedef ParameterValue< const ASTDictionaryEntry&, ParameterModifier::CONSTANT, CLASS_PARAM>				ClassParameter;

	typedef ParameterValue< std::vector<bool>, ParameterModifier::ARRAY, BOOLEAN_PARAM>							BooleanArrayParameter;
	typedef ParameterValue< std::vector<char>, ParameterModifier::ARRAY, CHAR_PARAM>							CharArrayParameter;
	typedef ParameterValue< std::vector<std::string>, ParameterModifier::ARRAY, STRING_PARAM>					StringArrayParameter;
	typedef ParameterValue< std::vector<int>, ParameterModifier::ARRAY, INT_PARAM>								IntArrayParameter;
	typedef ParameterValue< std::vector<long>, ParameterModifier::ARRAY, LONG_PARAM>							LongArrayParameter;
	typedef ParameterValue< std::vector<float>, ParameterModifier::ARRAY, FLOAT_PARAM>							FloatArrayParameter;
	typedef ParameterValue< std::vector<double>, ParameterModifier::ARRAY, DOUBLE_PARAM>						DoubleArrayParameter;
	typedef ParameterValue< std::vector<const ASTDictionaryEntry&>, ParameterModifier::ARRAY, CLASS_PARAM>		ClassArrayParameter;




	class ParameterPointerBase
	{
	public :

		virtual ~ParameterPointerBase()
		{}

		virtual const CPPModel::UID&		value() const = 0;
	};


	template<const ParameterType& paramType> class ParameterPointer : public ParameterValue< CPPModel::UID, ParameterModifier::POINTER, paramType>, public ParameterPointerBase
	{
	public :

		ParameterPointer( const CPPModel::UID&				value )
		: ParameterValue< CPPModel::UID, ParameterModifier::POINTER, paramType>( value )
		  {}


		ParameterPointer( const ParameterPointer&		valueToCopy )
		: ParameterValue< CPPModel::UID, ParameterModifier::POINTER, paramType>( valueToCopy )
		{}

		const CPPModel::UID&		value() const
		{
			 return( ParameterValue< CPPModel::UID, ParameterModifier::POINTER, paramType>::value() );
		}
	};


	typedef ParameterPointer<BOOLEAN_PARAM>							BooleanPointerParameter;
	typedef ParameterPointer<CHAR_PARAM>							CharPointerParameter;
	typedef ParameterPointer<STRING_PARAM>							StringPointerParameter;
	typedef ParameterPointer<INT_PARAM>								IntPointerParameter;
	typedef ParameterPointer<LONG_PARAM>							LongPointerParameter;
	typedef ParameterPointer<FLOAT_PARAM>							FloatPointerParameter;
	typedef ParameterPointer<DOUBLE_PARAM>							DoublePointerParameter;

	typedef ParameterPointer<FUNCTION_PARAM>						FunctionPointerParameter;



	class ParameterValueList : public boost::ptr_list<ParameterValueBase>
	{
	public :

		ParameterValueList()
		{}

		ParameterValueList( const ParameterValueList&		listToCopy )
		{
			for( const ParameterValueBase& paramToCopy : listToCopy )
			{
				this->push_back( (paramToCopy.deepCopy()).release() );
			}
		}
	};



	const ParameterValueList									EmptyParameterValueList;

}




#endif /* PARAMETERVALUES_H_ */
