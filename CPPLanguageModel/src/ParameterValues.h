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



namespace CPPModel
{


	class ParameterValueBase
	{
	public :

		virtual ~ParameterValueBase()
		{}

		virtual std::unique_ptr<ParameterValueBase>			deepCopy() const = 0;

		virtual const TypeSpecifier							typeSpecifier() const = 0;
	};




	template<class T, TypeSpecifier typeSpec> class ParameterValue : public ParameterValueBase
	{
	public :

		ParameterValue( T		value )
		: m_value( value )
		{}


		ParameterValue( const ParameterValue&		valueToCopy )
		: m_value( valueToCopy.m_value )
		{}


		std::unique_ptr<ParameterValueBase>			deepCopy() const
		{
			return( std::unique_ptr<ParameterValueBase>( new ParameterValue<T,typeSpec>( m_value ) ));
		}



		const TypeSpecifier			typeSpecifier() const
		{
			return( typeSpec );
		}


		const T&		value() const
		{
			return( m_value );
		}


	private :

		const T			m_value;

	};


	typedef ParameterValue<bool,TypeSpecifier::BOOLEAN>				ParameterBooleanValue;
	typedef ParameterValue<char,TypeSpecifier::CHAR>				ParameterCharValue;
	typedef ParameterValue<std::string,TypeSpecifier::STRING>		ParameterStringValue;
	typedef ParameterValue<int,TypeSpecifier::INT>					ParameterIntValue;
	typedef ParameterValue<long,TypeSpecifier::LONG_INT>			ParameterLongValue;
	typedef ParameterValue<float,TypeSpecifier::FLOAT>				ParameterFloatValue;
	typedef ParameterValue<double,TypeSpecifier::DOUBLE>			ParameterDoubleValue;




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
