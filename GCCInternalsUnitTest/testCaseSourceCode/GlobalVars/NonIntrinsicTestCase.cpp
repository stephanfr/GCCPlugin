/*
 * NonIntrisicTestCase.cpp
 *
 *  Created on: Jun 16, 2013
 *      Author: steve
 */


//	Without the symbol defined below, this example will not compile with gcc 4.8.0
//		I expect that might change in the future.


#define _GLIBCXX_GTHREAD_USE_WEAK 0

#include <string>


namespace TestNamespace
{
	static std::string		globalString;

	std::string				globalStringWithoutStaticKeyword;
}



