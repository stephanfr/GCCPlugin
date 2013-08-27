/*
 * MiscCode.cpp
 *
 *  Created on: Aug 23, 2013
 *      Author: steve
 */


#include "Access.h"
#include "ConstantValue.h"


#include <boost/lexical_cast.hpp>



namespace CPPModel
{

	const char*		toString( AccessSpecifier	accessSpec )
	{
		switch( accessSpec )
		{
			case AccessSpecifier::PUBLIC :
				return( "PUBLIC" );
				break;

			case AccessSpecifier::PROTECTED :
				return( "PROTECTED" );
				break;

			case AccessSpecifier::PRIVATE :
				return( "PRIVATE" );
				break;
		}

		return( "PRIVATE" );
	}


	ConstantValue*		ConstantValue::Copy( const ConstantValue&		valueToCopy )
	{
		switch( valueToCopy.kind() )
		{
			case Kind::STRING :
				return( new StringConstant( ((const StringConstant&)valueToCopy).value() ) );
				break;

			case Kind::INTEGER :
				return( new IntegerConstant( ((const IntegerConstant&)valueToCopy).value() ) );
				break;

			case Kind::REAL :
				return( new RealConstant( ((const RealConstant&)valueToCopy).value() ) );
				break;

			case Kind::UNRECOGNIZED :
				return( new UnrecognizedConstant() );
				break;
		}

		return( new UnrecognizedConstant() );

	}

}

