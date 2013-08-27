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
	bool AddNamespaceTest( CPPModel::ASTDictionary*		astDictionary );
}



bool	AddNamespaceTest( CPPModel::ASTDictionary*		astDictionary )
{
	std::cerr << "In Extension" << std::endl;

	//	This first call should succeed and create a total of 3 namespaces

	CPPModel::Result		cnResult = astDictionary->CreateNamespace( "TestCreatedNamespace::NestedNamespace::SecondNestedNamespace::" );

	if( !cnResult.isSuccess() )
	{
		return( false );
	}

	//	This call should fail as the namespace has already been created

	cnResult = astDictionary->CreateNamespace( "TestCreatedNamespace::" );

	if( cnResult.code() != CPPModel::ResultCode::NAMESPACE_ALREADY_EXISTS )
	{
		return( false );
	}

	return( true );
};



