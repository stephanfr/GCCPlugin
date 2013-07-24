/*
 * NamespaceTests.cpp
 *
 *  Created on: Jul 12, 2013
 *      Author: steve
 */



#include "gtest/gtest.h"

#include "../src/HelperFunctions.h"



namespace
{
  TEST(TestNamespaces, BasicTests)
  {
	  ForkGCCTestCase( "Namespaces",
			  	  	   "NamespaceBasicTestCase",
			  	  	   "namespaces=SimpleNamespace::,NestedNamespaceBase::",
			  	  	   "testExtension=libTestExtensions.so:AddNamespaceTest" );
  }
}




