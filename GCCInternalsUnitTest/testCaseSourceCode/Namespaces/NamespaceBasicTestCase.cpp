/*
 * NamespaceBasicTestCase.cpp
 *
 *  Created on: Jul 12, 2013
 *      Author: steve
 */

//	Without the symbol defined below, this example will not compile with gcc 4.8.0
//		I expect that might change in the future.


//#define _GLIBCXX_GTHREAD_USE_WEAK 0



namespace SimpleNamespace
{
	static int		globalInt;
}



namespace NestedNamespaceBase
{
	namespace FirstNestedNamespace
	{
		namespace SecondNestedNamespace
		{
			namespace ThirdNestedNamespace
			{
				namespace FourthNestedNamespace
				{
				}

				namespace FourthNestedNamespacePeer
				{
				}
			}
		}

		namespace SecondNestedNamespacePeer
		{
		}
	}
}



