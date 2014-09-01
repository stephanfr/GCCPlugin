/*
 * AllTests.cpp
 *
 *  Created on: Jun 9, 2013
 *      Author: steve
 */


#include "gtest/gtest.h"

#include "../src/HelperFunctions.h"


#include "NamespaceTests.cpp"
#include "GlobalVarTests.cpp"
#include "FunctionTests.cpp"
#include "ClassTests.cpp"
#include "UnionTests.cpp"



int main( int argc, char** argv)
{
	::testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}

