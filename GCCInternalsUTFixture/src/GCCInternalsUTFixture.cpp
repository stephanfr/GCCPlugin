//============================================================================
// Name        : GCCInternalsUTFixture.cpp
// Author      : Stephan Friedl
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================



#include "GCCInternalsTools.h"
#include "DeclOrTypeBaseTree.h"

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


std::unique_ptr<GCCInternalsTools::ASTDictionaryImpl>	astDict( new GCCInternalsTools::ASTDictionaryImpl() );



static void DeclFinishedGateCallback( void*		eventData,
		  	   	   	      	  	  	  void*		userData )
{
	//	If there has been an error, fall through and let the compiler handle it

	if( errorcount || sorrycount )
	{
		return;
	}

	if( eventData == NULL )
	{
		return;
	}

//	std::cerr << "Processing File: " << main_input_filename << std::endl;

	tree type = (tree)eventData;

	if( type == error_mark_node )
	{
		std::cerr << "Got an error mark node" << std::endl;

		return;
	}


	std::unique_ptr<GCCInternalsTools::DeclOrTypeBaseTree>		typeAsTree = GCCInternalsTools::DeclOrTypeBaseTree::convert( type );

//	const char *type_name;
//
//	if (TREE_CODE (DECL_NAME (type)) == IDENTIFIER_NODE) {
//			type_name = IDENTIFIER_POINTER (DECL_NAME (type));
//		} else {
//			type_name = IDENTIFIER_POINTER (DECL_NAME (TYPE_NAME (type)));
//		}

	std::cerr << "Type: " << typeAsTree->identifier() << "    " << typeAsTree->uid().uidValue() << std::endl;


	if( DECL_P( (tree)eventData ) )
	{
		if( TREE_CODE ((tree)eventData) == NAMESPACE_DECL )
		{
			std::cerr << "Found a Namespace" << std::endl;
		}
		else
		{
			GCCInternalsTools::DecodeNodeResult			decodedNode = astDict->DecodeASTNode( (tree)eventData );

			if( decodedNode.Succeeded() )
			{
				astDict->Insert( decodedNode.ReturnPtr().release() );
			}
		}
	}
}



static void TypeFinishedGateCallback( void*		eventData,
		  	   	   	      	  	  	  void*		userData )
{
	//	If there has been an error, fall through and let the compiler handle it

	if( errorcount || sorrycount )
	{
		return;
	}

	if( eventData == NULL )
	{
		return;
	}

//	std::cerr << "Processing File: " << main_input_filename << std::endl;

//	tree type = (tree)eventData;
//
//	if( type == error_mark_node )
//	{
//		std::cerr << "Got an error mark node" << std::endl;
//
//		return;
//	}


//	std::unique_ptr<GCCInternalsTools::DeclOrTypeBaseTree>		typeAsTree = GCCInternalsTools::DeclOrTypeBaseTree::convert( type );

//	const char *type_name;
//
//	if (TREE_CODE (DECL_NAME (type)) == IDENTIFIER_NODE) {
//			type_name = IDENTIFIER_POINTER (DECL_NAME (type));
//		} else {
//			type_name = IDENTIFIER_POINTER (DECL_NAME (TYPE_NAME (type)));
//		}

//	std::cerr << "Type: " << typeAsTree->identifier() << "    " << typeAsTree->uid().uidValue() << std::endl;


	tree type = (tree)eventData;

	if( type == error_mark_node )
	{
		std::cerr << "Got an error mark node" << std::endl;

		return;
	}

	std::unique_ptr<GCCInternalsTools::DeclOrTypeBaseTree>		typeAsTree = GCCInternalsTools::DeclOrTypeBaseTree::convert( type );

	std::cerr << "Type: " << typeAsTree->identifier() << "    " << typeAsTree->uid().uidValue() << std::endl;


	if( TYPE_P( (tree)eventData ) )
	{
		GCCInternalsTools::DecodeNodeResult			decodedNode = astDict->DecodeASTNode( (tree)eventData );

		if( decodedNode.Succeeded() )
		{
			astDict->Insert( decodedNode.ReturnPtr().release() );
		}
	}
}



bool	dictionaryBuilt = false;


static void PreGenericizeGateCallback( void*		eventData,
		  	   	   	      	  	  	   void*		userData )
{
	//	If there has been an error, fall through and let the compiler handle it

	if( errorcount || sorrycount )
	{
		return;
	}

	if( eventData == NULL )
	{
		return;
	}


	std::cerr << "In PreGenericize:    ";


//	astDict->FixupNamespaceTree();

//	for( EntryPointRecord currentEntryPoint : entryPointsToCall )
//	{
//		if( !(*currentEntryPoint.entryPoint)( astDict.get() ) )
//		{
//			std::cerr << "Error calling entry point: " << currentEntryPoint.name << std::endl;
//			exit( 3 );
//		}
//	}


//	std::cerr << "Processing File: " << main_input_filename << std::endl;

//	tree type = (tree)eventData;
//
//	if( type == error_mark_node )
//	{
//		std::cerr << "Got an error mark node" << std::endl;
//
//		return;
//	}


	std::unique_ptr<GCCInternalsTools::DeclOrTypeBaseTree>		typeAsTree = GCCInternalsTools::DeclOrTypeBaseTree::convert( (tree)eventData );

//	const char *type_name;
//
//	if (TREE_CODE (DECL_NAME (type)) == IDENTIFIER_NODE) {
//			type_name = IDENTIFIER_POINTER (DECL_NAME (type));
//		} else {
//			type_name = IDENTIFIER_POINTER (DECL_NAME (TYPE_NAME (type)));
//		}

	std::cerr << "Type: " << typeAsTree->identifier() << "    " << typeAsTree->uid().uidValue() << std::endl;

	if( typeAsTree->identifier().find( "static_initialization_and_destruction" ) != std::string::npos )
	{
		if( !dictionaryBuilt )
		{
			astDict->Build();

			dictionaryBuilt = true;

			std::cerr << "Built Dictionary" << std::endl;
		}

		std::cerr << "Injecting code" << std::endl;

		for( EntryPointRecord currentEntryPoint : entryPointsToCall )
		{
			if( !(*currentEntryPoint.entryPoint)( astDict.get() ) )
			{
				std::cerr << "Error calling entry point: " << currentEntryPoint.name << std::endl;
				exit( 3 );
			}
		}

	}


//	GCCInternalsTools::DecodeNodeResult			decodedNode = astDict->DecodeASTNode( (tree)eventData );
//
//	if( decodedNode.Succeeded() )
//	{
//		astDict->Insert( decodedNode.ReturnPtr().release() );
//	}
}



static void GateCallback( void*		eventData,
		  	   	   	      void*		userData )
{
	//	If there has been an error, fall through and let the compiler handle it

	if( errorcount || sorrycount )
	{
		return;
	}

//	if( eventData == NULL )
//	{
//		return;
//	}

	std::cerr << "Processing File: " << main_input_filename << std::endl;

	tree type = (tree)eventData;

	if( type == error_mark_node )
	{
		std::cerr << "Got an error mark node" << std::endl;

		return;
	}


//	std::unique_ptr<GCCInternalsTools::DeclOrTypeBaseTree>		typeAsTree = GCCInternalsTools::DeclOrTypeBaseTree::convert( type );

//	const char *type_name;
//
//	if (TREE_CODE (DECL_NAME (type)) == IDENTIFIER_NODE) {
//			type_name = IDENTIFIER_POINTER (DECL_NAME (type));
//		} else {
//			type_name = IDENTIFIER_POINTER (DECL_NAME (TYPE_NAME (type)));
//		}

//	std::cerr << "Type: " << typeAsTree->identifier() << "    " << typeAsTree->uid().uidValue() << std::endl;

	std::ofstream		resultsFile;

	resultsFile.open( outputFilename );


//	astDict->FixupNamespaceTree();

//	for( EntryPointRecord currentEntryPoint : entryPointsToCall )
//	{
//		if( !(*currentEntryPoint.entryPoint)( astDict.get() ) )
//		{
//			std::cerr << "Error calling entry point: " << currentEntryPoint.name << std::endl;
//			exit( 3 );
//		}
//	}

	astDict->DumpASTXMLByNamespaces( resultsFile, namespacesToScan, CPPModel::ASTDictionary::XMLOutputOptions::PREPEND_NAMESPACES );

	resultsFile.flush();
	resultsFile.close();
}



static void OverrideGateCallback( void*		eventData,
		  	   	   	      	  	  void*		userData )
{
	std::string		passName = current_pass->name;

	std::cerr << "In Override Gate callback: " << passName << std::endl;

	if( !dictionaryBuilt )
	{
		astDict->Build();

		dictionaryBuilt = true;

		std::cerr << "Built Dictionary" << std::endl;
	}
}


bool codeInjected = false;

static unsigned int CodeInjectionCallback( void )
{
	std::unique_ptr<GCCInternalsTools::DeclOrTypeBaseTree>		typeAsTree = GCCInternalsTools::DeclOrTypeBaseTree::convert( cfun->decl );

	std::cerr << "Function: " << typeAsTree->identifier() << std::endl;

	if( typeAsTree->identifier().compare( "__static_initialization_and_destruction_0" ) != 0 )
	{
		return( 0 );
	}

	if( codeInjected )
	{
		return( 0 );
	}

	codeInjected = true;

	std::cerr << "Injecting code" << std::endl;

	for( EntryPointRecord currentEntryPoint : entryPointsToCall )
	{
		if( !(*currentEntryPoint.entryPoint)( astDict.get() ) )
		{
			std::cerr << "Error calling entry point: " << currentEntryPoint.name << std::endl;
			exit( 3 );
		}
	}

	return( 0 );
}



/*
static unsigned int IPAPassCallback( void )
{
	std::cerr << "In IPA Pass" << std::endl;

	return( 0 );
}
*/





//Pass information structure
static struct gimple_opt_pass code_injection_pass =
{
   {
       GIMPLE_PASS,
       "injectcode",
       NULL,
       NULL,
       CodeInjectionCallback,
       NULL,
       NULL,
       0,
       TV_NONE,
       PROP_gimple_any,
       0,
       0,
       0,
       0
   }
};


/*
//Pass information structure
static struct simple_ipa_opt_pass ipa_pass =
{
   {
       IPA_PASS,
       "steve-ipa-pass",
       NULL,
       NULL,
       IPAPassCallback,
       NULL,
       NULL,
       0,
       TV_NONE,
       0,
       0,
       0,
       0,
       0
   }
};
*/




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



	struct register_pass_info replace_pass_info;

	   replace_pass_info.pass = &code_injection_pass.pass;
	   replace_pass_info.reference_pass_name = "cfg";
	   replace_pass_info.ref_pass_instance_number = 0;
	   replace_pass_info.pos_op = PASS_POS_INSERT_BEFORE;

	register_callback ( info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL, &replace_pass_info);

//	struct register_pass_info ipa_pass_info;
//
//	ipa_pass_info.pass = &ipa_pass.pass;
//	ipa_pass_info.reference_pass_name = "static-var";
//	ipa_pass_info.ref_pass_instance_number = 0;
//	ipa_pass_info.pos_op = PASS_POS_INSERT_BEFORE;

//	register_callback ( info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL, &ipa_pass_info);



	register_callback( info->base_name, PLUGIN_ATTRIBUTES, &RegisterAttributes, NULL );

//	register_callback( info->base_name, PLUGIN_FINISH_DECL, &DeclFinishedGateCallback, NULL );

//	register_callback( info->base_name, PLUGIN_FINISH_TYPE, &TypeFinishedGateCallback, NULL );

	register_callback( info->base_name, PLUGIN_OVERRIDE_GATE, &OverrideGateCallback, NULL );

//	register_callback( info->base_name, PLUGIN_PRE_GENERICIZE, &PreGenericizeGateCallback, NULL );

	register_callback( info->base_name, PLUGIN_FINISH_UNIT, &GateCallback, NULL );

	std::cerr << "Plugin Initialized, attribute registered" << std::endl;

	return( 0 );
}



