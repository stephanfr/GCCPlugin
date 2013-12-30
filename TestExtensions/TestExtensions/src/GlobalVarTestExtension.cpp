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



extern "C"
{
	bool AddStringGlobalVarTest( CPPModel::ASTDictionary*				astDictionary );
	bool	AddGlobalVarClassInstanceTest( CPPModel::ASTDictionary*		astDictionary );
}



bool	AddStringGlobalVarTest( CPPModel::ASTDictionary*		astDictionary )
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

	//	Now, declare and create a global int

	CPPModel::GlobalVarDeclaration		globalVarDec( "testVar",
													  *testNamespace,
													  true,
													  emptyAttributeList,
													  std::unique_ptr<const CPPModel::Type>( new CPPModel::FundamentalType( CPPModel::TypeSpecifier::INT ) ));

	CPPModel::CreateGlobalVarResult		gvResult = astDictionary->CreateGlobalVar( globalVarDec );

	if( !gvResult.Succeeded() )
	{
		std::cerr << "In AddGlobalVarsTest: CreateGlobalVar failed." << std::endl;
		return( false );
	}

	//	Next for an std::string type

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

	CPPModel::GlobalVarDeclaration		globalClassVarDec( std::string( "testCLassInstanceVar" ),
															*testNamespace,
															true,
															emptyAttributeList,
															stdClassEntry );

	CPPModel::CreateGlobalVarResult		gvResult2 = astDictionary->CreateGlobalVar( globalClassVarDec );

	//	Finished with success

	return( true );
};




