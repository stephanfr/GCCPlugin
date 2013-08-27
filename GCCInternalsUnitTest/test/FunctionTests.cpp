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
  TEST(TestFunctions, BasicTests)
  {
	  ForkGCCTestCase( "Functions", "SimpleFunctionTestCase", "namespaces=TestNamespace::" );
	  ForkGCCTestCase( "Functions", "FunctionsWithParametersTestCase", "namespaces=TestNamespace::" );
  }
}

