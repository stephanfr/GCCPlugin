/*
 * SimpleTestCaseWithIntrinsic.cpp
 *
 *  Created on: Jun 12, 2013
 *      Author: steve
 */



#define _GLIBCXX_GTHREAD_USE_WEAK 0

#include <string>
#include <iostream>


namespace TestNamespace
{
	void			FunctionReturningVoid()
	{};

	int				FunctionReturningInt()
	{
		return( 5 );
	};

	std::string		FunctionReturningString()
	{
		return( "ReturnedString" );
	};
}

