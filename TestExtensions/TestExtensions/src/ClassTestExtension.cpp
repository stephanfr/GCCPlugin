/*
 * ClassTestExtension.cpp
 *
 *  Created on: Aug 26, 2014
 *      Author: steve
 */



#include "ASTDictionary.h"

#include <limits.h>
#include <float.h>





extern "C"
{
	bool	WriteToFieldByOffsetTest( int									callbackType,
			  	  	  	  	  	  	  CPPModel::ASTDictionary*				astDictionary );
}





bool 	WriteToFieldByOffsetTest( int							callbackType,
		  	  	  	  	  	  	  CPPModel::ASTDictionary*		astDictionary )
{

	if( callbackType == 3 )
	{
		const CPPModel::Namespace		*testCreatedNamespace;

		if( !astDictionary->GetNamespace( "TestNamespace::", testCreatedNamespace ))
		{
			std::cerr << "In AddFundamentalArrayValueGlobalVarTest: Get TestCreatedNamesapce failed." << std::endl;
			return( false );
		}

		//	Get the dictionary entry for the TestNamespace::SimpleClass class


		CPPModel::ASTDictionary::FQNameIndexConstIterator		itrClass = astDictionary->FQNameIdx().find( "TestNamespace::SimpleClass" );


		if( itrClass == astDictionary->FQNameIdx().end() )
		{
			return( false );
		}

		if( (*itrClass)->entryKind() != CPPModel::DictionaryEntry::EntryKind::CLASS )
		{
			return( false );
		}

		const CPPModel::DictionaryClassEntry&		classEntry = dynamic_cast<const CPPModel::DictionaryClassEntry&>( *(*itrClass) );

		CPPModel::GetClassDefinitionResult			classDef = classEntry.GetClassDefinition( CPPModel::ParseOptions() );


		std::vector<int>					offsetInitializerArray;
		std::vector<std::string>			nameInitializerArray;

		for( boost::ptr_list<const CPPModel::FieldDeclaration>::const_iterator itrField = classDef.ReturnPtr()->fields().begin(); itrField != classDef.ReturnPtr()->fields().end(); itrField++ )
		{
			offsetInitializerArray.push_back( itrField->offsetInfo().totalOffsetInBytes() );
			nameInitializerArray.push_back( std::string( itrField->name().c_str() ));
		}

		//	Create an array of ints

		CPPModel::IntArrayGlobalVarDeclaration		offsetGlobalVarDec( "fieldOffsets",
																  *testCreatedNamespace,
																  offsetInitializerArray );

		CPPModel::CreateGlobalVarResult			gvResult = astDictionary->CreateGlobalVar( offsetGlobalVarDec );

		if( !gvResult.Succeeded() )
		{
			std::cerr << "In WriteToFieldByOffsetTest: Creating integer array of field offsets failed." << std::endl;
			return( false );
		}

		//	Create an array of ints

		CPPModel::StringArrayGlobalVarDeclaration		nameGlobalVarDec( "fieldNames",
																  *testCreatedNamespace,
																  nameInitializerArray );

		gvResult = astDictionary->CreateGlobalVar( nameGlobalVarDec );

		if( !gvResult.Succeeded() )
		{
			std::cerr << "In WriteToFieldByOffsetTest: Creating integer array of field offsets failed." << std::endl;
			return( false );
		}

	}

	return( true );
}

