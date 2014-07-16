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
		ForkGCCTestCase( "-c", "GlobalVars", "IntrinsicTestCase", "namespaces=TestNamespace::" );
		ForkGCCTestCase( "-c", "GlobalVars", "NonIntrinsicTestCase", "namespaces=TestNamespace::" );
		ForkGCCTestCase( "-c", "GlobalVars", "LocallyUserDefinedClassTestCase", "namespaces=LocalNamespace::,TestNamespace::" );
		ForkGCCTestCase( "-c", "GlobalVars", "DerivedTypesTestCase", "namespaces=TestNamespace::" );
		ForkGCCTestCase( "-c", "GlobalVars", "AttributesTestCase", "namespaces=TestNamespace::" );

		ForkGCCTestCase( "",
					     "GlobalVars",
						 "AddFundamentalValueGlobalVarTest",
						 "namespaces=TestCreatedNamespace::,LocalTestNamespace::",
						 "test-extension=libTestExtensions.so:AddFundamentalValueGlobalVarTest" );

		ForkGCCTestCase( "",
				 	  	 "GlobalVars",
						 "AddGlobalVarClassInstanceTest",
						 "namespaces=TestCreatedNamespace::,LocalTestNamespace::",
						 "test-extension=libTestExtensions.so:AddGlobalVarClassInstanceTest" );

	}
}

