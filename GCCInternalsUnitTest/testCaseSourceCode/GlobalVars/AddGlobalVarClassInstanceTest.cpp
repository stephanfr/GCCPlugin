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



namespace LocalTestNamespace
{
	class CTestClass
	{
	public :

			CTestClass()
			{
					std::cout << "Test Class Initialized." << std::endl;
			}


			CTestClass( const char*		message )
			{
				std::cout << "Message: " << message << std::endl;
			}

			CTestClass( const char*		message,
						bool			booleanValue,
						char			charValue,
						int				integerValue,
						long			longValue,
						float			floatValue,
						double			doubleValue )
			{
				std::cout << "Message: " << message << std::endl;
				std::cout << "Boolean Value: " << booleanValue << std::endl;
				std::cout << "Char Value: " << charValue << std::endl;
				std::cout << "Integer Value: " << integerValue << std::endl;
				std::cout << "Long Value: " << longValue << std::endl;
				std::cout << "Float Value: " << floatValue << std::endl;
				std::cout << "Double Value: " << doubleValue << std::endl;
			}

	};
}



LocalTestNamespace::CTestClass		sourceCodeGlobalTestClass;
LocalTestNamespace::CTestClass		sourceCodeGlobalTestClassWithStringInit( "Source Code Message." );
LocalTestNamespace::CTestClass		sourceCodeGlobalTestClassWithAllValues( "All Values Source Code Message.", true, 'a', 1, 2, 3.0, 4.0 );




int main()
{
	std::cout << "AddGlobalVarClassInstanceTest" << std::endl;

    return( 1 );
}
