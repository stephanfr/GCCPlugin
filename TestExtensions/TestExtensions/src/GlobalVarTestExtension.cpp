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
	bool 	AddFundamentalValueGlobalVarTest( int								callbackType,
			  	  	  	  	  	  			  CPPModel::ASTDictionary*			astDictionary );

	bool 	AddFundamentalArrayValueGlobalVarTest( int							callbackType,
			  	  	  	  	  	  			  	  CPPModel::ASTDictionary*		astDictionary );

	bool	AddGlobalVarClassInstanceTest( int									callbackType,
			  	  	  	  	  	  	  	   CPPModel::ASTDictionary*				astDictionary );
}



bool	AddFundamentalValueGlobalVarTest( int								callbackType,
		  	  	  	  	  	    		  CPPModel::ASTDictionary*			astDictionary )
{
	std::cerr << "In AddFundamentalValueGlobalVarTest Extension" << std::endl;

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


			CPPModel::BooleanPointerGlobalVarDeclaration		globalPointerVarDec( "testBoolPointerVar",
																  	  	  	  	 	 *testCreatedNamespace,
																  	  	  	  	 	 gvResult.ReturnValue() );

			gvResult = astDictionary->CreateGlobalVar( globalPointerVarDec );

			if( !gvResult.Succeeded() )
			{
				std::cerr << "In AddGlobalVarsTest: Bool Pointer CreateGlobalVar failed." << std::endl;
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

			CPPModel::CharPointerGlobalVarDeclaration		globalPointerVarDec( "testCharPointerVar",
																  	  	  	  	 *testCreatedNamespace,
																  	  	  	  	 gvResult.ReturnValue() );

			gvResult = astDictionary->CreateGlobalVar( globalPointerVarDec );

			if( !gvResult.Succeeded() )
			{
				std::cerr << "In AddGlobalVarsTest: Char Pointer CreateGlobalVar failed." << std::endl;
				return( false );
			}
		}

		//	Declare and create a global char*

		{
			CPPModel::StringGlobalVarDeclaration		globalVarDec( "testStringVar",
																	  *testCreatedNamespace,
																	  "Test String" );

			CPPModel::CreateGlobalVarResult		gvResult = astDictionary->CreateGlobalVar( globalVarDec );

			if( !gvResult.Succeeded() )
			{
				std::cerr << "In AddGlobalVarsTest: String CreateGlobalVar failed." << std::endl;
				return( false );
			}

			CPPModel::StringPointerGlobalVarDeclaration		globalPointerVarDec( "testStringPointerVar",
																  	  	  	  	 *testCreatedNamespace,
																  	  	  	  	 gvResult.ReturnValue() );

			gvResult = astDictionary->CreateGlobalVar( globalPointerVarDec );

			if( !gvResult.Succeeded() )
			{
				std::cerr << "In AddGlobalVarsTest: String Pointer CreateGlobalVar failed." << std::endl;
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

			CPPModel::IntPointerGlobalVarDeclaration		globalPointerVarDec( "testIntPointerVar",
																  	  	  	  	 *testCreatedNamespace,
																  	  	  	  	 gvResult.ReturnValue() );

			gvResult = astDictionary->CreateGlobalVar( globalPointerVarDec );

			if( !gvResult.Succeeded() )
			{
				std::cerr << "In AddGlobalVarsTest: Int Pointer CreateGlobalVar failed." << std::endl;
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

			CPPModel::LongPointerGlobalVarDeclaration		globalPointerVarDec( "testLongPointerVar",
																  	  	  	  	 *testCreatedNamespace,
																  	  	  	  	 gvResult.ReturnValue() );

			gvResult = astDictionary->CreateGlobalVar( globalPointerVarDec );

			if( !gvResult.Succeeded() )
			{
				std::cerr << "In AddGlobalVarsTest: Long Pointer CreateGlobalVar failed." << std::endl;
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

			CPPModel::FloatPointerGlobalVarDeclaration		globalPointerVarDec( "testFloatPointerVar",
																  	  	  	  	 *testCreatedNamespace,
																  	  	  	  	 gvResult.ReturnValue() );

			gvResult = astDictionary->CreateGlobalVar( globalPointerVarDec );

			if( !gvResult.Succeeded() )
			{
				std::cerr << "In AddGlobalVarsTest: Float Pointer CreateGlobalVar failed." << std::endl;
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

			CPPModel::DoublePointerGlobalVarDeclaration		globalPointerVarDec( "testDoublePointerVar",
																  	  	  	  	 *testCreatedNamespace,
																  	  	  	  	 gvResult.ReturnValue() );

			gvResult = astDictionary->CreateGlobalVar( globalPointerVarDec );

			if( !gvResult.Succeeded() )
			{
				std::cerr << "In AddGlobalVarsTest: Double Pointer CreateGlobalVar failed." << std::endl;
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



bool 	AddFundamentalArrayValueGlobalVarTest( int							callbackType,
		  	  	  	  	  	  			  	  CPPModel::ASTDictionary*		astDictionary )
{

	if( callbackType == 3 )
	{
		const CPPModel::Namespace		*testCreatedNamespace;

		if( !astDictionary->GetNamespace( "TestCreatedNamespace::", testCreatedNamespace ))
		{
			std::cerr << "In AddFundamentalArrayValueGlobalVarTest: Get TestCreatedNamesapce failed." << std::endl;
			return( false );
		}

		//	Create an array of booleans

		{
			std::vector<bool>			initializerArray = std::vector<bool>( { true, false, true, true, false, false, true, true, true, false, false, false } );

			CPPModel::BoolArrayGlobalVarDeclaration		globalVarDec( "testBoolArray",
																	  *testCreatedNamespace,
																	  initializerArray );

			CPPModel::CreateGlobalVarResult			gvResult = astDictionary->CreateGlobalVar( globalVarDec );

			if( !gvResult.Succeeded() )
			{
				std::cerr << "In AddGlobalVarsTest: Boolean Array AddFundamentalArrayValueGlobalVarTest failed." << std::endl;
				return( false );
			}
		}

		//	Create an array of chars

		{
			std::vector<char>			initializerArray = std::vector<char>( { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' } );

			CPPModel::CharArrayGlobalVarDeclaration		globalVarDec( "testCharArray",
																	  *testCreatedNamespace,
																	  initializerArray );

			CPPModel::CreateGlobalVarResult			gvResult = astDictionary->CreateGlobalVar( globalVarDec );

			if( !gvResult.Succeeded() )
			{
				std::cerr << "In AddGlobalVarsTest: Character Array AddFundamentalArrayValueGlobalVarTest failed." << std::endl;
				return( false );
			}
		}

		//	Create an array of ints

		{
			std::vector<int>			initializerArray = std::vector<int>( { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 } );

			CPPModel::IntArrayGlobalVarDeclaration		globalVarDec( "testIntArray",
																	  *testCreatedNamespace,
																	  initializerArray );

			CPPModel::CreateGlobalVarResult			gvResult = astDictionary->CreateGlobalVar( globalVarDec );

			if( !gvResult.Succeeded() )
			{
				std::cerr << "In AddGlobalVarsTest: Int Array AddFundamentalArrayValueGlobalVarTest failed." << std::endl;
				return( false );
			}
		}

		//	Create an array of longs

		{
			std::vector<long>			initializerArray = std::vector<long>( { 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112 } );

			CPPModel::LongArrayGlobalVarDeclaration		globalVarDec( "testLongArray",
																	  *testCreatedNamespace,
																	  initializerArray );

			CPPModel::CreateGlobalVarResult			gvResult = astDictionary->CreateGlobalVar( globalVarDec );

			if( !gvResult.Succeeded() )
			{
				std::cerr << "In AddGlobalVarsTest: Long Array AddFundamentalArrayValueGlobalVarTest failed." << std::endl;
				return( false );
			}
		}

		//	Create an array of floats

		{
			std::vector<float>			initializerArray = std::vector<float>( { 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.1 } );

			CPPModel::ArrayDimensions		dimensions( 10 );

			CPPModel::FloatArrayGlobalVarDeclaration		globalVarDec( "testFloatArray",
																	  	  *testCreatedNamespace,
																	  	  initializerArray );

			CPPModel::CreateGlobalVarResult			gvResult = astDictionary->CreateGlobalVar( globalVarDec );

			if( !gvResult.Succeeded() )
			{
				std::cerr << "In AddGlobalVarsTest: Float Array AddFundamentalArrayValueGlobalVarTest failed." << std::endl;
				return( false );
			}
		}

		//	Create an array of doubles

		{
			std::vector<double>			initializerArray = std::vector<double>( { DBL_MIN,  DBL_MAX,  double( 3.003 ),  double( 4.004 ),  double( 5.005 ),  double( 6.006 ),  double( 7.007 ),  double( 8.008 ),  double( 9.009 ),  double( 10.001 ) } );

			CPPModel::DoubleArrayGlobalVarDeclaration		globalVarDec( "testDoubleArray",
																	  	  *testCreatedNamespace,
																	  	  initializerArray );

			CPPModel::CreateGlobalVarResult			gvResult = astDictionary->CreateGlobalVar( globalVarDec );

			if( !gvResult.Succeeded() )
			{
				std::cerr << "In AddGlobalVarsTest: Double Array AddFundamentalArrayValueGlobalVarTest failed." << std::endl;
				return( false );
			}
		}

		//	Create an array of strings

		{
			std::vector<std::string>			initializerArray = std::vector<std::string>( { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", } );

			CPPModel::StringArrayGlobalVarDeclaration		globalVarDec( "testStringArray",
																	  	  *testCreatedNamespace,
																	  	  initializerArray );

			CPPModel::CreateGlobalVarResult			gvResult = astDictionary->CreateGlobalVar( globalVarDec );

			if( !gvResult.Succeeded() )
			{
				std::cerr << "In AddGlobalVarsTest: String Array AddFundamentalArrayValueGlobalVarTest failed." << std::endl;
				return( false );
			}
		}

	}

	//	Finished with success

	return( true );
}



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
																			   "testCTestClassVar",
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

				paramValues.push_back ( new CPPModel::StringConstantParameter( "Injected String Value." ) );

				CPPModel::ClassGlobalVarDeclaration		globalTestClassVarDec( testClassEntry,
																			   "testCTestClassVarWithStringInit",
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

				paramValues.push_back ( new CPPModel::StringConstantParameter( "Second Injected String Value." ) );
				paramValues.push_back ( new CPPModel::BooleanConstantParameter( false ) );
				paramValues.push_back ( new CPPModel::CharConstantParameter( 'b' ) );
				paramValues.push_back ( new CPPModel::IntConstantParameter( 30 ) );
				paramValues.push_back ( new CPPModel::LongConstantParameter( 40 ) );
				paramValues.push_back ( new CPPModel::FloatConstantParameter( 50.0 ) );
				paramValues.push_back ( new CPPModel::DoubleConstantParameter( 60.0 ) );

				CPPModel::ClassGlobalVarDeclaration		globalTestClassVarDec( testClassEntry,
																			   "testCTestClassVarWithAllValuesInit",
																			   *testCreatedNamespace,
																			   paramValues );

				CPPModel::CreateGlobalVarResult		gvResult2 = astDictionary->CreateGlobalVar( globalTestClassVarDec );
			}

			//	Now for the class taking arrays

/*
			{
				CPPModel::ASTDictionary::FQNameIndexConstIterator		itrTestClassType = astDictionary->FQNameIdx().find( "LocalTestNamespace::CTestClassTakingPointer" );

				if( itrTestClassType == astDictionary->FQNameIdx().end() )
				{
					std::cerr << "Could not find CTestClass type" << std::endl;
					return( false );
				}

				const CPPModel::DictionaryClassEntry&			testClassEntry = dynamic_cast<const CPPModel::DictionaryClassEntry&>( **itrTestClassType );


				const CPPModel::Namespace		*testCreatedNamespace;

				if( !astDictionary->GetNamespace( "LocalTestNamespace::", testCreatedNamespace ))
				{
					std::cerr << "In AddGlobalVarsTest: Get LocalTestNamespace failed." << std::endl;
					return( false );
				}

				//	Now, declare and create a global int

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
*/

		}


		//	Finished with success

	return( true );
};




