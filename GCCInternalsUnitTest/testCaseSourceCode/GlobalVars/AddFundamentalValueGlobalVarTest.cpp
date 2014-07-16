/*
 * NamespaceBasicTestCase.cpp
 *
 *  Created on: Jul 12, 2013
 *      Author: steve
 */


//        Without the symbol defined below, this example will not compile with gcc 4.8.0
//                I expect that might change in the future.


#define _GLIBCXX_GTHREAD_USE_WEAK 0

#include <iostream>

#include <limits.h>
#include <float.h>



namespace TestCreatedNamespace
{
	extern bool testBoolVar;
	extern char testCharVar;
	extern int testIntVar;
	extern long testLongVar;
	extern float testFloatVar;
	extern double testDoubleVar;

	extern char* testCharPointerVar;
}





int main()
{
	std::cout << "AddFundamentalValueGlobalVarTest" << std::endl << std::endl;

	std::cout << "Value of testBoolVar:        " << TestCreatedNamespace::testBoolVar << "                      Should be: 1" << std::endl;
	std::cout << "Value of testCharVar:        " << TestCreatedNamespace::testCharVar << "                      Should be: 'a'" << std::endl;
	std::cout << "Value of testCharPointerVar: " << TestCreatedNamespace::testCharPointerVar << "            Should be: 'Test String'" << std::endl;
	std::cout << "Value of testIntVar:         " << TestCreatedNamespace::testIntVar << "             Should be: " << INT_MAX << std::endl;
	std::cout << "Value of testLongVar:        " << TestCreatedNamespace::testLongVar << "    Should be: " << LONG_MAX << std::endl;
	std::cout << "Value of testFloatVar:       " << TestCreatedNamespace::testFloatVar << "            Should be: " << FLT_MAX << std::endl;
	std::cout << "Value of testDoubleVar:      " << TestCreatedNamespace::testDoubleVar << "           Should be: " << DBL_MAX << std::endl;

    return( 1 );
}
