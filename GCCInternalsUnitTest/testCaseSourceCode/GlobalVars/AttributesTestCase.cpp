/*
 * SimpleTestCaseWithIntrinsic.cpp
 *
 *  Created on: Jun 12, 2013
 *      Author: steve
 */




namespace TestNamespace
{
	static int		globalInt [[unit_test::attribute1( "arg1" )]];

	double			globaldoubleWithoutStaticKeyword [[unit_test::attribute1( "arg2", "arg3" )]];
}




