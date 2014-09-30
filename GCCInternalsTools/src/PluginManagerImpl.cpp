/*
 * PassManager.cpp
 *
 *  Created on: May 31, 2014
 *      Author: steve
 */



#include "PluginManagerImpl.h"

#include <iostream>




namespace GCCInternalsTools
{

	PluginManagerImpl		g_PluginManager;



	bool					BuildASTPassGate()
	{
		return( !g_PluginManager.dictionaryBuilt() );
	}


	static tree HandleAttribute( tree*		node,
								 tree		attrName,
								 tree		attrArguments,
								 int		flags,
								 bool*		no_add_attrs )
	{
		//	Just return a null tree now.

		return( NULL_TREE );
	}



	static void RegisterAttributesGCCCallback( void*		eventData,
							 	    		   void*		userData )
	{
		g_PluginManager.RegisterAttributes();
	}

	unsigned int 			BuildASTCallback( void )
	{
		return( g_PluginManager.BuildASTCallback() );
	}

	unsigned int 			GlobalDeclarationCallback( void )
	{
		return( g_PluginManager.GlobalDeclarationCallback() );
	}



	static const struct gimple_opt_pass 	g_BuildASTPass =
	{
	   {
		   GIMPLE_PASS,
		   "BuildAST",
		   (unsigned int)NULL,
		   &BuildASTPassGate,
		   &BuildASTCallback,
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


	static const struct gimple_opt_pass 	g_GlobalDeclarationPass =
	{
	   {
		   GIMPLE_PASS,
		   "DeclareGlobals",
		   (unsigned int)NULL,
		   NULL,
		   &GlobalDeclarationCallback,
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



	attribute_spec			*g_attributeSpecs;




	PluginManagerImpl::PluginManagerImpl()
		: m_ASTBuilt( false ),
		  m_globalsGenerated( false ),
		  m_userCallbacks( NULL )
	{}


	PluginManagerImpl::~PluginManagerImpl()
	{}




	void		PluginManagerImpl::Initialize( const char* 					pluginName,
			   	   	   	   	   	   	  	  	   CPPModel::CallbackIfx*		callbacks )
	{
		m_pluginName = pluginName;

		m_userCallbacks = callbacks;

		RegisterCallbacks( pluginName );
	}



	void		PluginManagerImpl::RegisterCallbacks( const char* 		pluginName )
	{
		register_callback( pluginName, PLUGIN_ATTRIBUTES, &RegisterAttributesGCCCallback, NULL );



		struct register_pass_info buildASTPassInfo;

		buildASTPassInfo.pass = const_cast<opt_pass*>( &g_BuildASTPass.pass );
		buildASTPassInfo.reference_pass_name = pass_warn_unused_result.pass.name;
		buildASTPassInfo.ref_pass_instance_number = 0;
		buildASTPassInfo.pos_op = PASS_POS_INSERT_BEFORE;

		register_callback ( pluginName, PLUGIN_PASS_MANAGER_SETUP, NULL, &buildASTPassInfo );



		struct register_pass_info globalDeclarationPassInfo;

		globalDeclarationPassInfo.pass = const_cast<opt_pass*>( &g_GlobalDeclarationPass.pass );
		globalDeclarationPassInfo.reference_pass_name = "cfg";
		globalDeclarationPassInfo.ref_pass_instance_number = 0;
		globalDeclarationPassInfo.pos_op = PASS_POS_INSERT_BEFORE;

		register_callback ( pluginName, PLUGIN_PASS_MANAGER_SETUP, NULL, &globalDeclarationPassInfo );
	}





	void		PluginManagerImpl::RegisterAttributes( void )
	{
		std::unique_ptr<CPPModel::AttributeSpecList>		attributes = std::move( m_userCallbacks->RegisterAttributes() );

		if( attributes->empty() )
		{
			g_attributeSpecs = NULL;
			return;
		}

		g_attributeSpecs = new attribute_spec[ attributes->size() + 1 ];

		int 	i = 0;

		for( CPPModel::AttributeSpec currentSpec : *attributes )
		{
			g_attributeSpecs[i].name = currentSpec.name().c_str();
			g_attributeSpecs[i].min_length = currentSpec.minNumArguments();
			g_attributeSpecs[i].max_length = currentSpec.maxNumArguments();

			switch( currentSpec.elementRequired() )
			{
				case CPPModel::AttributeSpec::RequiredElement::DECL_REQUIRED :
				{
					g_attributeSpecs[i].decl_required = true;
					g_attributeSpecs[i].type_required = false;
					g_attributeSpecs[i].function_type_required = false;
				}
				break;

				case CPPModel::AttributeSpec::RequiredElement::TYPE_REQUIRED :
				{
					g_attributeSpecs[i].decl_required = false;
					g_attributeSpecs[i].type_required = true;
					g_attributeSpecs[i].function_type_required = false;
				}
				break;

				case CPPModel::AttributeSpec::RequiredElement::FUNCTION_TYPE_REQUIRED :
				{
					g_attributeSpecs[i].decl_required = false;
					g_attributeSpecs[i].type_required = false;
					g_attributeSpecs[i].function_type_required = true;
				}
				break;

				case CPPModel::AttributeSpec::RequiredElement::ANY :
				{
					g_attributeSpecs[i].decl_required = false;
					g_attributeSpecs[i].type_required = false;
					g_attributeSpecs[i].function_type_required = false;
				}
				break;
			}

			g_attributeSpecs[i].affects_type_identity = currentSpec.affectsTypeIdentity();

			g_attributeSpecs[i].handler = HandleAttribute;

			i++;
		}

		g_attributeSpecs[i].name = NULL;

		register_scoped_attributes( g_attributeSpecs, m_pluginName.c_str() );
	}



	bool		PluginManagerImpl::BuildASTGate( void )
	{
		return( m_ASTBuilt );
	}


	unsigned int PluginManagerImpl::BuildASTCallback( void )
	{
		//	Build the dictionary

		m_ASTDictionary.Build();

		m_ASTBuilt = true;

		//	Let the user program know the AST is ready

		m_userCallbacks->ASTReady();

		//	Now is the time to add namespaces

		m_userCallbacks->CreateNamespaces();

		return( 0 );
	}


	unsigned int PluginManagerImpl::GlobalDeclarationCallback( void )
	{
		std::unique_ptr<GCCInternalsTools::DeclOrTypeBaseTree>		typeAsTree = GCCInternalsTools::DeclOrTypeBaseTree::convert( cfun->decl );

//		std::cerr << "Function: " << typeAsTree->identifier() << std::endl;

		if( typeAsTree->identifier().compare( "__static_initialization_and_destruction_0" ) != 0 )
		{
			return( 0 );
		}

		if( m_globalsGenerated )
		{
			return( 0 );
		}

		m_globalsGenerated = true;

		std::cerr << "Declaring Globals" << std::endl;

		m_userCallbacks->InjectCode();

		return( 0 );
	}

} /* namespace GCCInternalsTools */




namespace CPPModel
{
	PluginManager&		GetPluginManager()
	{
		return( GCCInternalsTools::g_PluginManager );
	}
}


