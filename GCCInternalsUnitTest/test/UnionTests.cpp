/*
 * GlobalVarTests.cpp
 *
 *  Created on: Jun 9, 2013
 *      Author: steve
 */




namespace
{
  TEST(TestUnions, BasicTests)
  {
	  ForkGCCTestCase( "-c", "Unions", "SimpleUnionTestCase", "namespaces=TestNamespace::", "list-all-namespaces" );
  }
}

