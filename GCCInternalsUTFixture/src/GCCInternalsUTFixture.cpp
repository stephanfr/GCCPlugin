//============================================================================
// Name        : GCCInternalsUTFixture.cpp
// Author      : Stephan Friedl
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================



#include "GCCInternalsTools.h"

#include "plugin.h"
#include "tree-pass.h"


#include <iostream>
#include <fstream>

#include <boost/tokenizer.hpp>






int	plugin_is_GPL_compatible;


std::string						outputFilename = "UnitTestResults.xml";

bool							listAllNamespaces = false;

std::list<std::string>			namespacesToScan;

void*							testExtensionHandle = NULL;

typedef							 bool (*EntryPointPtr)(CPPModel::ASTDictionary *);

typedef struct EntryPointRecord
{
	EntryPointRecord( std::string		epName,
					  EntryPointPtr		epPtr )
		: name( epName ),
		  entryPoint( epPtr )
	{};

	std::string		name;
	EntryPointPtr	entryPoint;
}
EntryPointRecord;

std::list<EntryPointRecord>		entryPointsToCall;



static tree HandleAttribute( tree*		node,
							 tree		attrName,
							 tree		attrArguments,
							 int		flags,
							 bool*		no_add_attrs )
{
	//	Just return a null tree now.

	return( NULL_TREE );
}


static struct attribute_spec g_Attribute1 =
{
	"attribute1", 0, -1, false, false, false, HandleAttribute, false
};


static struct attribute_spec g_Attribute2 =
{
	"attribute2", 0, -1, false, false, false, HandleAttribute, false
};


attribute_spec scopedAttributes[] = { g_Attribute1, g_Attribute2, NULL };


static void RegisterAttributes( void*		eventData,
						 	    void*		userData )
{
	register_scoped_attributes( scopedAttributes, "unit_test" );
}



static void GateCallback( void*		eventData,
		  	   	   	      void*		userData )
{
	//	If there has been an error, fall through and let the compiler handle it

	if( errorcount || sorrycount )
	{
		return;
	}

	std::cerr << "Processing File: " << main_input_filename << std::endl;

	std::ofstream		resultsFile;

	resultsFile.open( outputFilename );


	std::shared_ptr<GCCInternalsTools::ASTDictionaryImpl>	astDict( new GCCInternalsTools::ASTDictionaryImpl() );

	astDict->Build();

	for( EntryPointRecord currentEntryPoint : entryPointsToCall )
	{
		if( !(*currentEntryPoint.entryPoint)( astDict.get() ) )
		{
			std::cerr << "Error calling entry point: " << currentEntryPoint.name << std::endl;
			exit( 3 );
		}
	}


	if( listAllNamespaces )
	{
		for( CPPModel::ASTDictionary::NamespaceMapConstIterator itrNamespace = astDict->namespaces().begin(); itrNamespace != astDict->namespaces().end(); itrNamespace++ )
		{
			itrNamespace->second->toXML( resultsFile, CPPModel::SerializationOptions::NONE );
		}
	}


	for( std::string currentNamespace : namespacesToScan )
	{

		for( CPPModel::ASTDictionary::NamespaceIndexConstIterator namespaceIndex = (*astDict).NamespaceIdx().lower_bound( currentNamespace ); namespaceIndex != astDict->NamespaceIdx().upper_bound( currentNamespace ); namespaceIndex++ )
		{
			(*namespaceIndex)->toXML( resultsFile, CPPModel::SerializationOptions::NONE );
		}

		CPPModel::ParseOptions		parseOptions;

		for( CPPModel::ASTDictionary::NamespaceIndexConstIterator namespaceIndex = astDict->NamespaceIdx().lower_bound( currentNamespace ); namespaceIndex != astDict->NamespaceIdx().upper_bound( currentNamespace ); namespaceIndex++ )
		{
			if( (*namespaceIndex)->entryKind() == CPPModel::DictionaryEntry::EntryKind::CLASS )
			{
				std::unique_ptr<const CPPModel::ClassDefinition>		classDef;

				((CPPModel::DictionaryClassEntry*)&(**namespaceIndex))->GetClassDefinition( parseOptions, classDef );

				classDef->toXML( resultsFile, CPPModel::SerializationOptions::NONE );
			}
			else if( (*namespaceIndex)->entryKind() == CPPModel::DictionaryEntry::EntryKind::UNION )
			{
				std::unique_ptr<const CPPModel::UnionDefinition>		unionDef;

				((CPPModel::DictionaryUnionEntry*)&(**namespaceIndex))->GetUnionDefinition( parseOptions, unionDef );

				unionDef->toXML( resultsFile, CPPModel::SerializationOptions::NONE );
			}
			else if( (*namespaceIndex)->entryKind() == CPPModel::DictionaryEntry::EntryKind::FUNCTION )
			{
				std::unique_ptr<const CPPModel::FunctionDefinition>		functionDef;

				if( ((CPPModel::DictionaryFunctionEntry*)&(**namespaceIndex))->GetFunctionDefinition( parseOptions, functionDef ))
				{
					functionDef->toXML( resultsFile, CPPModel::SerializationOptions::NONE );
				}
			}
			else if( (*namespaceIndex)->entryKind() == CPPModel::DictionaryEntry::EntryKind::GLOBAL_VAR )
			{
				std::unique_ptr<const CPPModel::GlobalVarEntry>		globalVarDef;

				((CPPModel::DictionaryGlobalVarEntry*)&(**namespaceIndex))->GetGlobalVarEntry( parseOptions, globalVarDef );

				globalVarDef->toXML( resultsFile, CPPModel::SerializationOptions::NONE );
			}
		}
	}

	resultsFile.flush();
	resultsFile.close();
}




int plugin_init( plugin_name_args*		info,
			 	 plugin_gcc_version*	ver )
{
	std::cerr << "Starting Plugin: "<< info->base_name << std::endl;

	if( info->argc > 0 )
	{
		for( int i = 0; i < info->argc; i++ )
		{
			if( strcmp( info->argv[i].key, "output-filename" ) == 0 )
			{
				outputFilename = info->argv[i].value;
			}
			else if( strcmp( info->argv[i].key, "list-all-namespaces" ) == 0 )
			{
				listAllNamespaces = true;
			}
			else if( strcmp( info->argv[i].key, "namespaces" ) == 0 )
			{
				namespacesToScan.clear();

				std::string		allNamespaces = info->argv[i].value;

				typedef boost::tokenizer< boost::escaped_list_separator<char> >		Tokenizer;

				Tokenizer 	parser( allNamespaces );

				for( Tokenizer::iterator itrNamespace = parser.begin(); itrNamespace != parser.end(); itrNamespace++ )
				{
					namespacesToScan.push_back( *itrNamespace );
				}
			}
			else if( strcmp( info->argv[i].key, "test-extension" ) == 0 )
			{
				std::string			extensionsInfo = info->argv[i].value;

				std::string			soName = extensionsInfo.substr( 0, extensionsInfo.find( ":" ) );

				std::string			allEntryPoints = extensionsInfo.substr( extensionsInfo.find( ":" ) + 1 );

				typedef boost::tokenizer< boost::escaped_list_separator<char> >		Tokenizer;

				Tokenizer 	parser( allEntryPoints );

				testExtensionHandle = dlopen( soName.c_str(), RTLD_LAZY );

				if( !testExtensionHandle )
				{
					std::cerr << "Unable to load extension library: " << info->argv[i].value << std::endl;
					exit( 1 );
				}

				for( Tokenizer::iterator itrEntryPoint = parser.begin(); itrEntryPoint != parser.end(); itrEntryPoint++ )
				{
					EntryPointPtr		entryPoint;
					char *dlsymError;

					dlerror();
					entryPoint = (EntryPointPtr)dlsym( testExtensionHandle, (*itrEntryPoint).c_str() );
					if ((dlsymError = dlerror()) != NULL)
					{
					   fprintf(stderr, "%s\n", dlsymError );
					   exit( 2 );
					}

					entryPointsToCall.push_back( EntryPointRecord( *itrEntryPoint, entryPoint ));
				}
			}
		}
	}



	register_callback( info->base_name, PLUGIN_ATTRIBUTES, &RegisterAttributes, NULL );

	register_callback( info->base_name, PLUGIN_ALL_IPA_PASSES_START, &GateCallback, NULL );

	std::cerr << "Plugin Initialized, attribute registered" << std::endl;

	return( 0 );
}



