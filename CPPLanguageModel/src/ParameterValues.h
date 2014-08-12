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


	TypeSpecifier	AsTypeSpecifier( const ParameterType&	paramType );


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

	extern const ParameterType		ARRAY_PARAM;



	class ParameterArrayValueBase : public ParameterValueBase
	{
	public :

		virtual ~ParameterArrayValueBase()
		{}

		virtual std::unique_ptr<ParameterValueBase>			deepCopy() const = 0;

		const ParameterType&								type() const
		{
			return( ARRAY_PARAM );
		}

		virtual const ParameterType&						elementType() const = 0;

		virtual const unsigned int							size() const = 0;
	};



	template<class T, const ParameterType& paramType> class ParameterArrayValue : public ParameterArrayValueBase
	{
	public :

		ParameterArrayValue( int				numElements,
							 const T			value[] )
		: m_elementParamType( paramType ),
		  m_value( value, value + numElements )
		{}

		ParameterArrayValue( const std::vector<T>&			value )
		: m_elementParamType( paramType ),
		  m_value( value )
		{}


		ParameterArrayValue( const ParameterArrayValue&		valueToCopy )
		: m_value( valueToCopy.m_value ),
		  m_elementParamType( valueToCopy.m_elementParamType )
		{}


		std::unique_ptr<ParameterValueBase>			deepCopy() const
		{
			return( std::unique_ptr<ParameterValueBase>( new ParameterArrayValue<T,paramType>( m_value ) ));
		}



		const ParameterType&						elementType() const
		{
			return( m_elementParamType );
		}


		const unsigned int							size() const
		{
			return( m_value.size() );
		}

		const std::vector<T>&						value() const
		{
			return( m_value );
		}


	private :

		const ParameterType&		m_elementParamType;
		const std::vector<T>		m_value;
	};


	typedef ParameterValue<bool, BOOLEAN_PARAM>								ParameterBooleanValue;
	typedef ParameterValue<char, CHAR_PARAM>								ParameterCharValue;
	typedef ParameterValue<std::string, STRING_PARAM>						ParameterStringValue;
	typedef ParameterValue<int, INT_PARAM>									ParameterIntValue;
	typedef ParameterValue<long, LONG_PARAM>								ParameterLongValue;
	typedef ParameterValue<float, FLOAT_PARAM>								ParameterFloatValue;
	typedef ParameterValue<double, DOUBLE_PARAM>							ParameterDoubleValue;
	typedef ParameterValue<const ASTDictionaryEntry&, CLASS_PARAM>			ParameterClassValue;


	typedef ParameterArrayValue<bool, BOOLEAN_PARAM>						ParameterBoolArrayValue;
	typedef ParameterArrayValue<char, CHAR_PARAM>							ParameterCharArrayValue;
	typedef ParameterArrayValue<std::string, STRING_PARAM>					ParameterStringArrayValue;
	typedef ParameterArrayValue<int, INT_PARAM>								ParameterIntArrayValue;
	typedef ParameterArrayValue<long, LONG_PARAM>							ParameterLongArrayValue;
	typedef ParameterArrayValue<float, FLOAT_PARAM>							ParameterFloatArrayValue;
	typedef ParameterArrayValue<double, DOUBLE_PARAM>						ParameterDoubleArrayValue;
	typedef ParameterArrayValue<const ASTDictionaryEntry&, CLASS_PARAM>		ParameterClassArrayValue;



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
