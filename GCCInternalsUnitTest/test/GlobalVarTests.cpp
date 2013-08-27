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
		ForkGCCTestCase( "GlobalVars", "IntrinsicTestCase", "namespaces=TestNamespace::" );
		ForkGCCTestCase( "GlobalVars", "NonIntrinsicTestCase", "namespaces=TestNamespace::" );
		ForkGCCTestCase( "GlobalVars", "LocallyUserDefinedClassTestCase", "namespaces=TestNamespace::" );
		ForkGCCTestCase( "GlobalVars", "DerivedTypesTestCase", "namespaces=TestNamespace::" );
		ForkGCCTestCase( "GlobalVars", "AttributesTestCase", "namespaces=TestNamespace::" );

		ForkGCCTestCase( "GlobalVars",
						 "CreateGlobalVarsTestCase",
						 "namespaces=TestCreatedNamespace::,bob::",
						 "test-extension=libTestExtensions.so:AddGlobalVarsTest" );
	}
}

