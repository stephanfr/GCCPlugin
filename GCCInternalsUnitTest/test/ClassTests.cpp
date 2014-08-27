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
  TEST(TestClasses, BasicTests)
  {
	  	ForkGCCTestCase( "", "Classes", "SimpleClassTestCase", "namespaces=TestNamespace::", "list-all-namespaces" );

		ForkGCCTestCase( "",
							 "Classes",
							 "WriteToFieldByOffsetTestCase",
							 "namespaces=TestCreatedNamespace::",
							 "test-extension=libTestExtensions.so:WriteToFieldByOffsetTest" );

 }
}

