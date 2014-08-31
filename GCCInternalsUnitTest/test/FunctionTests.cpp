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
	  ForkGCCTestCase( "-c", "Functions", "SimpleFunctionTestCase", "namespaces=TestNamespace::", "list-all-namespaces" );
	  ForkGCCTestCase( "-c", "Functions", "FunctionsWithParametersTestCase", "namespaces=TestNamespace::", "list-all-namespaces" );
	  ForkGCCTestCase( "-c", "Functions", "FunctionPrototypeTestCase", "namespaces=TestNamespace::", "list-all-namespaces" );
  }
}

