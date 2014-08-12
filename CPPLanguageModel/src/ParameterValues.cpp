/*
 * ParameterValues.cpp
 *
 *  Created on: Feb 26, 2014
 *      Author: steve
 */


#include "ParameterValues.h"


namespace CPPModel
{

	const ParameterType		BOOLEAN_PARAM = { TypeSpecifier::BOOLEAN };
	const ParameterType		CHAR_PARAM = { TypeSpecifier::CHAR };
	const ParameterType		STRING_PARAM = { TypeSpecifier::STRING };
	const ParameterType		INT_PARAM = { TypeSpecifier::INT };
	const ParameterType		LONG_PARAM = { TypeSpecifier::LONG_INT };
	const ParameterType		FLOAT_PARAM = { TypeSpecifier::FLOAT };
	const ParameterType		DOUBLE_PARAM = { TypeSpecifier::DOUBLE };
	const ParameterType		CLASS_PARAM = { TypeSpecifier::CLASS };

	const ParameterType		ARRAY_PARAM = { TypeSpecifier::ARRAY };



	TypeSpecifier	AsTypeSpecifier( const ParameterType&	paramType )
	{
//		return( paramType.at( paramType.size() - 1 ));
		return( paramType.at( 0 ));
	}

}
