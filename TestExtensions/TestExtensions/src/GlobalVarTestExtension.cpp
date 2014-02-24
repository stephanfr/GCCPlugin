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
	bool 	AddStringGlobalVarTest( CPPModel::ASTDictionary*				astDictionary );
	bool	AddGlobalVarClassInstanceTest( CPPModel::ASTDictionary*		astDictionary );
}



bool	AddStringGlobalVarTest( CPPModel::ASTDictionary*		astDictionary )
{
	//	Start by getting the global namespace

	const CPPModel::Namespace		*globalNamespace;

	if( !astDictionary->GetNamespace( "::", globalNamespace ))
	{
		std::cerr << "In AddGlobalVarsTest: Get global Namesapce failed." << std::endl;
		return( false );
	}

	//	Create an empty attribute list

//	CPPModel::Attributes				emptyAttributeList;

	//	Declare and create a global boolean

	{
		CPPModel::BooleanGlobalVarDeclaration		globalVarDec( "testBoolVar",
														  	  	  *globalNamespace,
														  	  	   true );

		CPPModel::CreateGlobalVarResult		gvResult = astDictionary->CreateGlobalVar( globalVarDec );

		if( !gvResult.Succeeded() )
		{
			std::cerr << "In AddGlobalVarsTest: Boolean CreateGlobalVar failed." << std::endl;
			return( false );
		}
	}


	//	Declare and create a global char

	{
		CPPModel::CharGlobalVarDeclaration		globalVarDec( "testCharVar",
														  	  *globalNamespace,
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
														  	  	  *globalNamespace,
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
														  	  *globalNamespace,
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
														  	  *globalNamespace,
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
														  	  *globalNamespace,
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
														  	  	  *globalNamespace,
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
	//	Next for an class type

	{
		CPPModel::ASTDictionary::FQNameIndexConstIterator		itrTestClassType = astDictionary->FQNameIdx().find( "LocalTestNamespace::CTestClass" );

		if( itrTestClassType == astDictionary->FQNameIdx().end() )
		{
			std::cerr << "Could not find CTestClass type" << std::endl;
			return( false );
		}

		const CPPModel::DictionaryClassEntry&			testClassEntry = dynamic_cast<const CPPModel::DictionaryClassEntry&>( **itrTestClassType );

		const CPPModel::Namespace		*localNamespace;

		if( !astDictionary->GetNamespace( "::", localNamespace ))
		{
			std::cerr << "In AddGlobalVarsTest: Get LocalTestNamesapce failed." << std::endl;
			return( false );
		}

		CPPModel::ClassGlobalVarDeclaration		globalTestClassVarDec( testClassEntry,
																	   std::string( "testCTestClassVar" ),
																   	   *localNamespace );

		CPPModel::CreateGlobalVarResult		gvResult2 = astDictionary->CreateGlobalVar( globalTestClassVarDec );
	}

	{
		CPPModel::ASTDictionary::FQNameIndexConstIterator		itrTestClassType = astDictionary->FQNameIdx().find( "LocalTestNamespace::CTestClass" );

		if( itrTestClassType == astDictionary->FQNameIdx().end() )
		{
			std::cerr << "Could not find CTestClass type" << std::endl;
			return( false );
		}

		const CPPModel::DictionaryClassEntry&			testClassEntry = dynamic_cast<const CPPModel::DictionaryClassEntry&>( **itrTestClassType );

		const CPPModel::Namespace		*localNamespace;

		if( !astDictionary->GetNamespace( "::", localNamespace ))
		{
			std::cerr << "In AddGlobalVarsTest: Get LocalTestNamesapce failed." << std::endl;
			return( false );
		}

		CPPModel::ParameterValueList		paramValues;

		paramValues.push_back ( new CPPModel::ParameterStringValue( "Injected String Value." ) );

		CPPModel::ClassGlobalVarDeclaration		globalTestClassVarDec( testClassEntry,
																	   std::string( "testCTestClassVarWithStringInit" ),
																   	   *localNamespace,
																   	   paramValues );

		CPPModel::CreateGlobalVarResult		gvResult2 = astDictionary->CreateGlobalVar( globalTestClassVarDec );
	}

	{
		CPPModel::ASTDictionary::FQNameIndexConstIterator		itrTestClassType = astDictionary->FQNameIdx().find( "LocalTestNamespace::CTestClass" );

		if( itrTestClassType == astDictionary->FQNameIdx().end() )
		{
			std::cerr << "Could not find CTestClass type" << std::endl;
			return( false );
		}

		const CPPModel::DictionaryClassEntry&			testClassEntry = dynamic_cast<const CPPModel::DictionaryClassEntry&>( **itrTestClassType );

		const CPPModel::Namespace		*localNamespace;

		if( !astDictionary->GetNamespace( "::", localNamespace ))
		{
			std::cerr << "In AddGlobalVarsTest: Get LocalTestNamesapce failed." << std::endl;
			return( false );
		}

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
																   	   *localNamespace,
																   	   paramValues );

		CPPModel::CreateGlobalVarResult		gvResult2 = astDictionary->CreateGlobalVar( globalTestClassVarDec );
	}

	//	Finished with success

	return( true );
};




bool	AddGlobalVarClassInstanceTest( CPPModel::ASTDictionary*		astDictionary )
{
	std::cerr << "In Extension" << std::endl;

	//	Start by creating a namespace for our new global

	CPPModel::CreateNamespaceResult		cnResult = astDictionary->CreateNamespace( "TestCreatedNamespace::" );

	if( !cnResult.Succeeded() )
	{
		std::cerr << "In AddGlobalVarsTest: Create TestCreatedNamesapce failed." << std::endl;
		return( false );
	}

	const CPPModel::Namespace		*testNamespace;

	if( !astDictionary->GetNamespace( "TestCreatedNamespace::", testNamespace ))
	{
		std::cerr << "In AddGlobalVarsTest: Get TestCreatedNamesapce failed." << std::endl;
		return( false );
	}

	//	Create an empty attribute list

	CPPModel::Attributes				emptyAttributeList;

	//	Next for an std::string type

	CPPModel::ASTDictionary::FQNameIndexConstIterator		itrClassType = astDictionary->FQNameIdx().find( "::CTestClass" );

	if( itrClassType == astDictionary->FQNameIdx().end() )
	{
		std::cerr << "Could not find CTestClass type" << std::endl;
		return( false );
	}

	const CPPModel::DictionaryClassEntry&			stdClassEntry = dynamic_cast<const CPPModel::DictionaryClassEntry&>( **itrClassType );

//	CPPModel::GlobalVarDeclaration		globalClassVarDec( std::string( "testCLassInstanceVar" ),
//															*testNamespace,
//															emptyAttributeList,
//															stdClassEntry );

//	CPPModel::CreateGlobalVarResult		gvResult2 = astDictionary->CreateGlobalVar( globalClassVarDec );

	//	Finished with success

	return( true );
};




