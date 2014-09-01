/*
 * NamespaceTests.cpp
 *
 *  Created on: Jul 12, 2013
 *      Author: steve
 */





namespace
{
	TEST(TestNamespaces, BasicTests)
	{
		ForkGCCTestCase( "-c",
						 "Namespaces",
					     "NamespaceBasicTestCase",
					     "namespaces=SimpleNamespace::,NestedNamespaceBase::",
					     "list-all-namespaces" );

		ForkGCCTestCase( "-c",
						 "Namespaces",
					     "CreateNamespaceTest",
					     "namespaces=NestedNamespaceBase::",
					     "test-extension=libTestExtensions.so:AddNamespaceTest",
					     "list-all-namespaces" );
	}
}




