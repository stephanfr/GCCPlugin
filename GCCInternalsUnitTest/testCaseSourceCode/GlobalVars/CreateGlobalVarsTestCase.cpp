/*
 * NamespaceBasicTestCase.cpp
 *
 *  Created on: Jul 12, 2013
 *      Author: steve
 */


//	Without the symbol defined below, this example will not compile with gcc 4.8.0
//		I expect that might change in the future.


#define _GLIBCXX_GTHREAD_USE_WEAK 0

#include <iostream>
#include <string>



class CTestClass
{
public :

	CTestClass()
	{
		std::cout << "Test Class Initialized." << std::endl;
	}

};

namespace testJunk
{
	CTestClass		testClass;
}

CTestClass		secondTestClass;


int main()
{

	std::cout << "Test Case Ran" << std::endl;

	return( 1 );
}
