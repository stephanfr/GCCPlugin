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


	class ParameterValueBase
	{
	public :

		virtual ~ParameterValueBase()
		{}

		virtual std::unique_ptr<ParameterValueBase>			deepCopy() const = 0;

		virtual const ParameterType&						type() const = 0;
	};




	template<class T, const ParameterType& paramType> class ParameterValue : public ParameterValueBase
	{
	public :

		ParameterValue( const T&				value )
		: m_paramType( paramType ),
		  m_value( value )
		{}


		ParameterValue( const ParameterValue&		valueToCopy )
		: m_value( valueToCopy.m_value ),
		  m_paramType( valueToCopy.m_paramType )
		{}


		std::unique_ptr<ParameterValueBase>			deepCopy() const
		{
			return( std::unique_ptr<ParameterValueBase>( new ParameterValue<T,paramType>( m_value ) ));
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

		const ParameterType&	m_paramType;
		const T					m_value;

	};


	extern const ParameterType		BOOLEAN_PARAM;
	extern const ParameterType		CHAR_PARAM;
	extern const ParameterType		STRING_PARAM;
	extern const ParameterType		INT_PARAM;
	extern const ParameterType		LONG_PARAM;
	extern const ParameterType		FLOAT_PARAM;
	extern const ParameterType		DOUBLE_PARAM;
	extern const ParameterType		CLASS_PARAM;


	typedef ParameterValue<bool, BOOLEAN_PARAM>							ParameterBooleanValue;
	typedef ParameterValue<char, CHAR_PARAM>							ParameterCharValue;
	typedef ParameterValue<std::string, STRING_PARAM>					ParameterStringValue;
	typedef ParameterValue<int, INT_PARAM>								ParameterIntValue;
	typedef ParameterValue<long, LONG_PARAM>							ParameterLongValue;
	typedef ParameterValue<float, FLOAT_PARAM>							ParameterFloatValue;
	typedef ParameterValue<double, DOUBLE_PARAM>						ParameterDoubleValue;
	typedef ParameterValue<const ASTDictionaryEntry&, CLASS_PARAM>		ParameterClassValue;


/*
	class ParameterPointer : public ParameterValueBase
	{
	public :

		ParameterPointer( std::unique_ptr<ParameterValueBase>		value )
		: m_value( value.release() )
		{}


		ParameterPointer( const ParameterPointer&		valueToCopy )
		: m_value( valueToCopy.deepCopy() )
		{}


		std::unique_ptr<ParameterValueBase>			deepCopy() const
		{
			return( std::unique_ptr<ParameterValueBase>( new ParameterPointer( m_value->deepCopy() ) ));
		}


		const TypeSpecifier			typeSpecifier() const
		{
			return( TypeSpecifier::POINTER );
		}

		const TypeSpecifier							pointeeTypeSpecifier() const
		{
			return( m_value->typeSpecifier() );
		}


		const ParameterValueBase&					value() const
		{
			return( *m_value );
		}


	private :

		std::unique_ptr<ParameterValueBase>			m_value;
	};
*/





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
