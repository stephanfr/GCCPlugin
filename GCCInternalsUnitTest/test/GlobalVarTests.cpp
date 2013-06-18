/*
 * GlobalVarTests.cpp
 *
 *  Created on: Jun 9, 2013
 *      Author: steve
 */


#include "gtest/gtest.h"

#include "../src/HelperFunctions.h"



namespace
{
  TEST(TestGlobalVariables, BasicTests)
  {
	  ForkGCCTestCase( "GlobalVars", "IntrinsicTestCase" );
	  ForkGCCTestCase( "GlobalVars", "NonIntrinsicTestCase" );
	  ForkGCCTestCase( "GlobalVars", "LocallyUserDefinedClassTestCase" );
	  ForkGCCTestCase( "GlobalVars", "DerivedTypesTestCase" );
	  ForkGCCTestCase( "GlobalVars", "AttributesTestCase" );
  }
}

