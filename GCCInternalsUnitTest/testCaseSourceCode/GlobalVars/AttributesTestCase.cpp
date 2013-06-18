/*
 * SimpleTestCaseWithIntrinsic.cpp
 *
 *  Created on: Jun 12, 2013
 *      Author: steve
 */



//	Both of these variables will be marked as 'static' as both have the
//		same storage type wrt compiling, the static keyword should
//		just restrict the scope of the integer to this translation unit.


namespace TestNamespace
{
	static int		globalInt [[unit_test::attribute1( "arg1" )]];

	double			globaldoubleWithoutStaticKeyword [[unit_test::attribute1( "arg2", "arg3" )]];
}




