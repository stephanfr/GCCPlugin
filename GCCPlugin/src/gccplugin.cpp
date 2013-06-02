/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/

#include <cassert>

#include <algorithm>
#include <cstdlib>
#include <gmp.h>
#include <map>
#include <memory>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <utility>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/ptr_container/ptr_list.hpp>

#include "config.h"

#include "gcc-plugin.h"

#include "tree.h"
#include "cp/cp-tree.h"
#include "diagnostic.h"
#include "real.h"

#include "ListAliases.h"

#include "Constants.h"
#include "Serialization.h"
#include "ConstantValue.h"
#include "NamedEntity.h"
#include "Attribute.h"
#include "UID.h"
#include "SourceLocation.h"
#include "Namespace.h"
#include "Static.h"
#include "Access.h"
#include "SourceElement.h"
#include "Types.h"
#include "Union.h"
#include "Function.h"
#include "GlobalVar.h"
#include "Class.h"
#include "ASTDictionary.h"

#include "ConstantTree.h"
#include "IdentifierTree.h"
#include "DeclOrTypeBaseTree.h"
#include "TypeTree.h"
#include "DeclTree.h"
#include "TreeList.h"


#include "GCCInternalsTools.h"

#include "plugin.h"
#include "tree-pass.h"







int	plugin_is_GPL_compatible;




static tree HandleSerializerAttribute( tree*	node,
									   tree		attrName,
									   tree		attrArguments,
									   int		flags,
									   bool*	no_add_attrs )
{
	//	Just return a null tree now.

	return( NULL_TREE );
}


static struct attribute_spec g_JSONSerializerAttribute =
{
	"json_serialize", 0, -1, false, true, false, HandleSerializerAttribute, false
};


static struct attribute_spec g_GeneralizedAttribute =
{
	"generalized_attribute", 0, -1, false, false, false, HandleSerializerAttribute, false
};


attribute_spec jsonScopedAttributes[] = { g_GeneralizedAttribute, g_JSONSerializerAttribute, NULL };


static void RegisterAttributes( void*		eventData,
						 	    void*		userData )
{
//	register_attribute( &g_JSONSerializerAttribute );
	register_scoped_attributes( jsonScopedAttributes, "json" );
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

	std::shared_ptr<GCCInternalsTools::ASTDictionaryImpl>	astDict( new GCCInternalsTools::ASTDictionaryImpl() );

	astDict->Build();

	for( CPPModel::ASTDictionary::NamespaceIndexConstIterator namespaceIndex = astDict->NamespaceIdx().lower_bound( "TestNamespace::" ); namespaceIndex != astDict->NamespaceIdx().upper_bound( "TestNamespace::" ); namespaceIndex++ )
	{
		(*namespaceIndex)->toXML( std::cerr, 0, CPPModel::XMLSerializable::SerializationOptions::NONE );
	}

	CPPModel::ParseOptions		parseOptions;

	for( CPPModel::ASTDictionary::NamespaceIndexConstIterator namespaceIndex = astDict->NamespaceIdx().lower_bound( "TestNamespace::" ); namespaceIndex != astDict->NamespaceIdx().upper_bound( "TestNamespace::" ); namespaceIndex++ )
	{
		if( (*namespaceIndex)->entryKind() == CPPModel::DictionaryEntry::EntryKind::CLASS )
		{
			std::unique_ptr<const CPPModel::ClassDefinition>		classDef;

			((CPPModel::DictionaryClassEntry*)&(**namespaceIndex))->GetClassDefinition( parseOptions, classDef );

			classDef->toXML( std::cerr, 0, CPPModel::XMLSerializable::SerializationOptions::NONE );
		}
		else if( (*namespaceIndex)->entryKind() == CPPModel::DictionaryEntry::EntryKind::UNION )
		{
			std::unique_ptr<const CPPModel::UnionDefinition>		unionDef;

			((CPPModel::DictionaryUnionEntry*)&(**namespaceIndex))->GetUnionDefinition( parseOptions, unionDef );

			unionDef->toXML( std::cerr, 0, CPPModel::XMLSerializable::SerializationOptions::NONE );
		}
		else if( (*namespaceIndex)->entryKind() == CPPModel::DictionaryEntry::EntryKind::FUNCTION )
		{
			std::unique_ptr<const CPPModel::FunctionDefinition>		functionDef;

			((CPPModel::DictionaryFunctionEntry*)&(**namespaceIndex))->GetFunctionDefinition( parseOptions, functionDef );

			functionDef->toXML( std::cerr, 0, CPPModel::XMLSerializable::SerializationOptions::NONE );
		}
		else if( (*namespaceIndex)->entryKind() == CPPModel::DictionaryEntry::EntryKind::GLOBAL_VAR )
		{
			std::unique_ptr<const CPPModel::GlobalVarDefinition>		globalVarDef;

			((CPPModel::DictionaryGlobalVarEntry*)&(**namespaceIndex))->GetGlobalVarDefinition( parseOptions, globalVarDef );

			globalVarDef->toXML( std::cerr, 0, CPPModel::XMLSerializable::SerializationOptions::NONE );
		}

	}
}




int plugin_init( plugin_name_args*		info,
			 	 plugin_gcc_version*	ver )
{
	std::cerr << "Starting Plugin: "<< info->base_name << std::endl;

	register_callback( info->base_name, PLUGIN_ATTRIBUTES, &RegisterAttributes, NULL );

	register_callback( info->base_name, PLUGIN_ALL_IPA_PASSES_START, &GateCallback, NULL );

	std::cerr << "Plugin Initialized, attribute registered" << std::endl;

	return( 0 );
}


