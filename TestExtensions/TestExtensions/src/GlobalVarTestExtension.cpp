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
	bool AddGlobalVarsTest( CPPModel::ASTDictionary*		astDictionary );
}



bool	AddGlobalVarsTest( CPPModel::ASTDictionary*		astDictionary )
{
	std::cerr << "In Extension" << std::endl;

	//	Start by creating a namespace for our new global

	CPPModel::Result		cnResult = astDictionary->CreateNamespace( "TestCreatedNamespace::" );

	if( !cnResult.isSuccess() )
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

	//	Now, declare and create a global int

	CPPModel::Attributes				emptyAttributeList;

	CPPModel::GlobalVarDeclaration		globalVarDec( "testVar",
													  *testNamespace,
													  true,
													  emptyAttributeList,
													  std::unique_ptr<const CPPModel::Type>( new CPPModel::FundamentalType( CPPModel::TypeSpecifier::INT ) ));

	cnResult = astDictionary->CreateGlobalVar( globalVarDec );

	if( !cnResult.isSuccess() )
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

	cnResult = astDictionary->CreateGlobalVar( globalStringVarDec );

	//	Finished with success

	return( true );
};




