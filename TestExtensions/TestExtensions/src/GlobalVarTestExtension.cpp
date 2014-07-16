/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/



#include "ASTDictionary.h"

#include <limits.h>
#include <float.h>



extern "C"
{
	bool 	AddFundamentalValueGlobalVarTest( int							callbackType,
			  	  	  	  	  	  			  CPPModel::ASTDictionary*		astDictionary );

	bool	AddGlobalVarClassInstanceTest( int								callbackType,
			  	  	  	  	  	  	  	   CPPModel::ASTDictionary*			astDictionary );
}



bool	AddFundamentalValueGlobalVarTest( int								callbackType,
		  	  	  	  	  	    		  CPPModel::ASTDictionary*			astDictionary )
{
	std::cerr << "In AddFundamentalValueGlobalVarTest Extension" << std::endl;

	//	Start by getting the test namespace namespace

//	if( callbackType == 2 )
//	{
//		CPPModel::CreateNamespaceResult		cnResult = astDictionary->CreateNamespace( "TestCreatedNamespace::" );
//
//		if( !cnResult.Succeeded() )
//		{
//			return( false );
//		}
//	}

	if( callbackType == 3 )
	{
		const CPPModel::Namespace		*testCreatedNamespace;

		if( !astDictionary->GetNamespace( "TestCreatedNamespace::", testCreatedNamespace ))
		{
			std::cerr << "In AddGlobalVarsTest: Get TestCreatedNamesapce failed." << std::endl;
			return( false );
		}

		//	Declare and create a global boolean

		{
			const CPPModel::BooleanGlobalVarDeclaration		globalVarDec( "testBoolVar",
																	  	  *testCreatedNamespace,
																	  	  true );

			CPPModel::CreateGlobalVarResult		gvResult = astDictionary->CreateGlobalVar( globalVarDec );

			if( gvResult.Failed() )
			{
				std::cerr << "In AddGlobalVarsTest: Add boolean global failed." << std::endl;
				return( false );
			}

		}


		//	Declare and create a global char

		{
			CPPModel::CharGlobalVarDeclaration		globalVarDec( "testCharVar",
																  *testCreatedNamespace,
																   'a' );

			CPPModel::CreateGlobalVarResult		gvResult = astDictionary->CreateGlobalVar( globalVarDec );

			if( !gvResult.Succeeded() )
			{
				std::cerr << "In AddGlobalVarsTest: Char CreateGlobalVar failed." << std::endl;
				return( false );
			}
		}

		//	Declare and create a global char*

		{
			CPPModel::StringGlobalVarDeclaration		globalVarDec( "testCharPointerVar",
																	  *testCreatedNamespace,
																	  "Test String" );

			CPPModel::CreateGlobalVarResult		gvResult = astDictionary->CreateGlobalVar( globalVarDec );

			if( !gvResult.Succeeded() )
			{
				std::cerr << "In AddGlobalVarsTest: Char CreateGlobalVar failed." << std::endl;
				return( false );
			}
		}

		//	Now, declare and create a global int

		{
			CPPModel::IntGlobalVarDeclaration		globalVarDec( "testIntVar",
																  *testCreatedNamespace,
																  INT_MAX );

			CPPModel::CreateGlobalVarResult		gvResult = astDictionary->CreateGlobalVar( globalVarDec );

			if( !gvResult.Succeeded() )
			{
				std::cerr << "In AddGlobalVarsTest: Int CreateGlobalVar failed." << std::endl;
				return( false );
			}
		}

		//	Now, declare and create a global long

		{
			CPPModel::LongGlobalVarDeclaration		globalVarDec( "testLongVar",
																  *testCreatedNamespace,
																  LONG_MAX );

			CPPModel::CreateGlobalVarResult		gvResult = astDictionary->CreateGlobalVar( globalVarDec );

			if( !gvResult.Succeeded() )
			{
				std::cerr << "In AddGlobalVarsTest: Long CreateGlobalVar failed." << std::endl;
				return( false );
			}
		}

		//	Now, declare and create a global float

		{
			CPPModel::FloatGlobalVarDeclaration		globalVarDec( "testFloatVar",
																  *testCreatedNamespace,
																  FLT_MAX );

			CPPModel::CreateGlobalVarResult		gvResult = astDictionary->CreateGlobalVar( globalVarDec );

			if( !gvResult.Succeeded() )
			{
				std::cerr << "In AddGlobalVarsTest: Float CreateGlobalVar failed." << std::endl;
				return( false );
			}
		}

		//	Now, declare and create a global double

		{
			CPPModel::DoubleGlobalVarDeclaration		globalVarDec( "testDoubleVar",
																	  *testCreatedNamespace,
																	  DBL_MAX );

			CPPModel::CreateGlobalVarResult		gvResult = astDictionary->CreateGlobalVar( globalVarDec );

			if( !gvResult.Succeeded() )
			{
				std::cerr << "In AddGlobalVarsTest: Double CreateGlobalVar failed." << std::endl;
				return( false );
			}
		}

	//	Next for an std::string type
/*
	{
		CPPModel::ASTDictionary::FQNameIndexConstIterator		itrStringType = astDictionary->FQNameIdx().find( "std::string" );

		if( itrStringType == astDictionary->FQNameIdx().end() )
		{
			std::cerr << "Could not find string type" << std::endl;
			return( false );
		}

		const CPPModel::DictionaryClassEntry&			stdStringEntry = dynamic_cast<const CPPModel::DictionaryClassEntry&>( **itrStringType );

		CPPModel::GlobalVarDeclaration		globalStringVarDec( std::string( "testStringVar" ),
																*testNamespace,
																true,
																emptyAttributeList,
																stdStringEntry );

		CPPModel::CreateGlobalVarResult		gvResult2 = astDictionary->CreateGlobalVar( globalStringVarDec );
	}
*/
	}

	//	Finished with success

	return( true );
};




bool	AddGlobalVarClassInstanceTest( int							callbackType,
		  	  	  	  	  	  	  	   CPPModel::ASTDictionary*		astDictionary )
{
	std::cerr << "In AddGlobalVarClassInstanceTest Extension" << std::endl;


	if( callbackType == 2 )
	{
		CPPModel::CreateNamespaceResult		cnResult = astDictionary->CreateNamespace( "TestCreatedNamespace::" );

		if( !cnResult.Succeeded() )
		{
			return( false );
		}
	}


	if( callbackType == 3 )
	{
		const CPPModel::Namespace		*testCreatedNamespace;

		if( !astDictionary->GetNamespace( "TestCreatedNamespace::", testCreatedNamespace ))
		{
			std::cerr << "In AddGlobalVarsTest: Get TestCreatedNamespace failed." << std::endl;
			return( false );
		}

		//	Next for a class type - no parameters passed to the constructor

			{
				CPPModel::ASTDictionary::FQNameIndexConstIterator		itrTestClassType = astDictionary->FQNameIdx().find( "LocalTestNamespace::CTestClass" );

				if( itrTestClassType == astDictionary->FQNameIdx().end() )
				{
					std::cerr << "Could not find CTestClass type" << std::endl;
					return( false );
				}

				const CPPModel::DictionaryClassEntry&			testClassEntry = dynamic_cast<const CPPModel::DictionaryClassEntry&>( **itrTestClassType );

		//		const CPPModel::Namespace		*localNamespace;

		//		if( !astDictionary->GetNamespace( "LocalTestNamespace::", localNamespace ))
		//		{
		//			std::cerr << "In AddGlobalVarsTest: Get LocalTestNamesapce failed." << std::endl;
		//			return( false );
		//		}

				CPPModel::ClassGlobalVarDeclaration		globalTestClassVarDec( testClassEntry,
																			   std::string( "testCTestClassVar" ),
																			   *testCreatedNamespace );

				CPPModel::CreateGlobalVarResult		gvResult2 = astDictionary->CreateGlobalVar( globalTestClassVarDec );
			}


			//	Class type again - string passed to the constructor

			{
				CPPModel::ASTDictionary::FQNameIndexConstIterator		itrTestClassType = astDictionary->FQNameIdx().find( "LocalTestNamespace::CTestClass" );

				if( itrTestClassType == astDictionary->FQNameIdx().end() )
				{
					std::cerr << "Could not find CTestClass type" << std::endl;
					return( false );
				}

				const CPPModel::DictionaryClassEntry&			testClassEntry = dynamic_cast<const CPPModel::DictionaryClassEntry&>( **itrTestClassType );

		//		const CPPModel::Namespace		*localNamespace;
		//
		//		if( !astDictionary->GetNamespace( "LocalTestNamespace::", localNamespace ))
		//		{
		//			std::cerr << "In AddGlobalVarsTest: Get LocalTestNamesapce failed." << std::endl;
		//			return( false );
		//		}

				CPPModel::ParameterValueList		paramValues;

				paramValues.push_back ( new CPPModel::ParameterStringValue( "Injected String Value." ) );

				CPPModel::ClassGlobalVarDeclaration		globalTestClassVarDec( testClassEntry,
																			   std::string( "testCTestClassVarWithStringInit" ),
																			   *testCreatedNamespace,
																			   paramValues );

				CPPModel::CreateGlobalVarResult		gvResult2 = astDictionary->CreateGlobalVar( globalTestClassVarDec );
			}

			//	Class type one last time - a full set of parameters passed

			{
				CPPModel::ASTDictionary::FQNameIndexConstIterator		itrTestClassType = astDictionary->FQNameIdx().find( "LocalTestNamespace::CTestClass" );

				if( itrTestClassType == astDictionary->FQNameIdx().end() )
				{
					std::cerr << "Could not find CTestClass type" << std::endl;
					return( false );
				}

				const CPPModel::DictionaryClassEntry&			testClassEntry = dynamic_cast<const CPPModel::DictionaryClassEntry&>( **itrTestClassType );

		//		const CPPModel::Namespace		*localNamespace;
		//
		//		if( !astDictionary->GetNamespace( "::", localNamespace ))
		//		{
		//			std::cerr << "In AddGlobalVarsTest: Get LocalTestNamesapce failed." << std::endl;
		//			return( false );
		//		}

				CPPModel::ParameterValueList		paramValues;

				paramValues.push_back ( new CPPModel::ParameterStringValue( "Second Injected String Value." ) );
				paramValues.push_back ( new CPPModel::ParameterBooleanValue( false ) );
				paramValues.push_back ( new CPPModel::ParameterCharValue( 'b' ) );
				paramValues.push_back ( new CPPModel::ParameterIntValue( 30 ) );
				paramValues.push_back ( new CPPModel::ParameterLongValue( 40 ) );
				paramValues.push_back ( new CPPModel::ParameterFloatValue( 50.0 ) );
				paramValues.push_back ( new CPPModel::ParameterDoubleValue( 60.0 ) );

				CPPModel::ClassGlobalVarDeclaration		globalTestClassVarDec( testClassEntry,
																			   std::string( "testCTestClassVarWithAllValuesInit" ),
																			   *testCreatedNamespace,
																			   paramValues );

				CPPModel::CreateGlobalVarResult		gvResult2 = astDictionary->CreateGlobalVar( globalTestClassVarDec );
			}
		}


		//	Finished with success

	return( true );
};




