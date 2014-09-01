/*
 * GlobalVarTests.cpp
 *
 *  Created on: Jun 9, 2013
 *      Author: steve
 */



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

