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
	  ForkGCCTestCase( "-c", "Unions", "SimpleUnionTestCase", "namespaces=TestNamespace::", "list-all-namespaces" );
  }
}

