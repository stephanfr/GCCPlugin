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
	extern bool		testBoolArray[];
	extern char		testCharArray[];
	extern int		testIntArray[];
	extern long		testLongArray[];
	extern float	testFloatArray[];
	extern double	testDoubleArray[];
	extern char*	testStringArray[];

	extern bool*	testBoolArrayPointer[];
	extern char*	testCharArrayPointer[];
	extern int*		testIntArrayPointer[];
	extern long*	testLongArrayPointer[];
	extern float*	testFloatArrayPointer[];
	extern double*	testDoubleArrayPointer[];
	extern char**	testStringArrayPointer[];
}





int main()
{
	std::cout << "AddFundamentalArrayValueGlobalVarTest" << std::endl << std::endl;

	for( int i = 0; i < 12; ++i )
	{
		std::cout << "Value of test bool array " << i << ": " << TestCreatedNamespace::testBoolArray[i] << std::endl;
	}

	std::cout << std::endl;

	for( int i = 0; i < 26; ++i )
	{
		std::cout << "Value of test char array " << i << ": " << TestCreatedNamespace::testCharArray[i] << std::endl;
	}

	std::cout << std::endl;

	for( int i = 0; i < 12; ++i )
	{
		std::cout << "Value of test int array " << i << ": " << TestCreatedNamespace::testIntArray[i] << std::endl;
	}

	std::cout << std::endl;

	for( int i = 0; i < 12; ++i )
	{
		std::cout << "Value of test long array " << i << ": " << TestCreatedNamespace::testLongArray[i] << std::endl;
	}

	std::cout << std::endl;

	for( int i = 0; i < 10; ++i )
	{
		std::cout << "Value of test float array " << i << ": " << TestCreatedNamespace::testFloatArray[i] << std::endl;
	}

	std::cout << std::endl;

	for( int i = 0; i < 10; ++i )
	{
		std::cout << "Value of test double array " << i << ": " << TestCreatedNamespace::testDoubleArray[i] << std::endl;
	}

	std::cout << std::endl;

	for( int i = 0; i < 10; ++i )
	{
		std::cout << "Value of test string array " << i << ": " << TestCreatedNamespace::testStringArray[i] << std::endl;
	}


	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;


	for( int i = 0; i < 12; ++i )
	{
		std::cout << "Value of test bool array pointer " << i << ": " << (*TestCreatedNamespace::testBoolArrayPointer)[i] << std::endl;
	}

	std::cout << std::endl;

	for( int i = 0; i < 26; ++i )
	{
		std::cout << "Value of test char array pointer " << i << ": " << (*TestCreatedNamespace::testCharArrayPointer)[i] << std::endl;
	}

	std::cout << std::endl;

	for( int i = 0; i < 12; ++i )
	{
		std::cout << "Value of test int array pointer " << i << ": " << (*TestCreatedNamespace::testIntArrayPointer)[i] << std::endl;
	}

	std::cout << std::endl;

	for( int i = 0; i < 12; ++i )
	{
		std::cout << "Value of test long array pointer " << i << ": " << (*TestCreatedNamespace::testLongArrayPointer)[i] << std::endl;
	}

	std::cout << std::endl;

	for( int i = 0; i < 10; ++i )
	{
		std::cout << "Value of test float array pointer " << i << ": " << (*TestCreatedNamespace::testFloatArrayPointer)[i] << std::endl;
	}

	std::cout << std::endl;

	for( int i = 0; i < 10; ++i )
	{
		std::cout << "Value of test double array pointer " << i << ": " << (*TestCreatedNamespace::testDoubleArrayPointer)[i] << std::endl;
	}

	std::cout << std::endl;

	for( int i = 0; i < 10; ++i )
	{
		std::cout << "Value of test string array pointer " << i << ": " << (*TestCreatedNamespace::testStringArrayPointer)[i] << std::endl;
	}


	return( 1 );
}
