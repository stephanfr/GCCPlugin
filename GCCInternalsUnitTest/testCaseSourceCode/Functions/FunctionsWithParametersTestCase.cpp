/*
 * SimpleTestCaseWithIntrinsic.cpp
 *
 *  Created on: Jun 12, 2013
 *      Author: steve
 */



#define _GLIBCXX_GTHREAD_USE_WEAK 0

#include <string>


namespace TestNamespace
{
	void			FunctionReturningVoid( int		intParameter ) {};

	int				FunctionReturningInt( int		intParameter,
										  float		floatParameter )
	{
		return( intParameter + floatParameter + 5 );
	};

	std::string		FunctionReturningString( std::string	stringParameter,
											 double			doubleParameter,
											 long			longParameter )
	{
		return( "ReturnedString" );
	};
}




