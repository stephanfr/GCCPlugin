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

	extern char* testStringVar;

	extern bool* testBoolPointerVar;
	extern char* testCharPointerVar;
	extern int* testIntPointerVar;
	extern long* testLongPointerVar;
	extern float* testFloatPointerVar;
	extern double* testDoublePointerVar;

	extern char** testStringPointerVar;

}





int main()
{
	std::cout << "AddFundamentalValueGlobalVarTest" << std::endl << std::endl;

	std::cout << "Value of testBoolVar:        " << TestCreatedNamespace::testBoolVar << "                      Should be: 1" << std::endl;
	std::cout << "Value of testCharVar:        " << TestCreatedNamespace::testCharVar << "                      Should be: 'a'" << std::endl;
	std::cout << "Value of testStringVar:      " << TestCreatedNamespace::testStringVar << "            Should be: 'Test String'" << std::endl;
	std::cout << "Value of testIntVar:         " << TestCreatedNamespace::testIntVar << "             Should be: " << __INT_MAX__ << std::endl;
	std::cout << "Value of testLongVar:        " << TestCreatedNamespace::testLongVar << "    Should be: " << __LONG_MAX__ << std::endl;
	std::cout << "Value of testFloatVar:       " << TestCreatedNamespace::testFloatVar << "            Should be: " << FLT_MAX << std::endl;
	std::cout << "Value of testDoubleVar:      " << TestCreatedNamespace::testDoubleVar << "           Should be: " << DBL_MAX << std::endl;

	std::cout << std::endl;

	std::cout << "Value pointed to by testBoolPointerVar: " << *TestCreatedNamespace::testBoolPointerVar << std::endl;
	std::cout << "Value pointed to by testCharPointerVar: " << *TestCreatedNamespace::testCharPointerVar << std::endl;
	std::cout << "Value pointed to by testStringPointerVar: " << *TestCreatedNamespace::testStringPointerVar << std::endl;
	std::cout << "Value pointed to by testIntPointerVar: " << *TestCreatedNamespace::testIntPointerVar << std::endl;
	std::cout << "Value pointed to by testLongPointerVar: " << *TestCreatedNamespace::testLongPointerVar << std::endl;
	std::cout << "Value pointed to by testFloatPointerVar: " << *TestCreatedNamespace::testFloatPointerVar << std::endl;
	std::cout << "Value pointed to by testDoublePointerVar: " << *TestCreatedNamespace::testDoublePointerVar << std::endl;

    return( 1 );
}
