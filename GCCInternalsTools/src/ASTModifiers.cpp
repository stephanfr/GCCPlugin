/*
 * ASTModifiers.cpp
 *
 *  Created on: Jun 19, 2013
 *      Author: steve
 */

#include <cassert>

#include <algorithm>
#include <cstdlib>
#include <gmp.h>
#include <map>
#include <memory>
#include <ostream>
#include <stdlib.h>
#include <string>
#include <utility>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/ptr_container/ptr_map.hpp>

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
#include "CompilerSpecific.h"
#include "NamedEntity.h"
#include "Attribute.h"
#include "UID.h"
#include "SourceLocation.h"
#include "Static.h"
#include "Access.h"
#include "SourceElement.h"
#include "Namespace.h"
#include "NamespaceScoped.h"
#include "ASTEntry.h"
#include "Types.h"
#include "Union.h"
#include "Function.h"
#include "GlobalVar.h"
#include "Class.h"
#include "ASTDictionary.h"

#include "Result.h"

#include "ConstantTree.h"
#include "IdentifierTree.h"
#include "DeclOrTypeBaseTree.h"
#include "TypeTree.h"
#include "DeclTree.h"
#include "TreeList.h"
#include "AttributeParser.h"

#include "GCCInternalsTools.h"

#include "ASTModifiers.h"

#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "tree-dump.h"
#include "tree-iterator.h"
#include "gimple.h"
#include "function.h"
#include "flags.h"
#include "output.h"
#include "ggc.h"
#include "toplev.h"
//#include "varray.h"
//#include "langhooks-def.h"
#include "langhooks.h"
//#include "target.h"

#include "cgraph.h"
//#include "sfe1.h"
#include "opts.h"
#include "input.h"




namespace GCCInternalsTools
{

	//	Function to split a fully qualified namespace into an ordered list of individual scopes

	std::list<std::string>		ParseFQNamespace( const std::string& 		fqNamespace )
	{
		std::list<std::string>		individualNamespaces;

		std::string					workingNamespace = fqNamespace;

		for( size_t itrScopeOperator = workingNamespace.find( CPPModel::SCOPE_RESOLUTION_OPERATOR ); ( itrScopeOperator != 0 ) && ( itrScopeOperator != std::string::npos); itrScopeOperator = workingNamespace.find( CPPModel::SCOPE_RESOLUTION_OPERATOR ))
		{
			individualNamespaces.push_back( workingNamespace.substr( 0, itrScopeOperator ) );

			workingNamespace.erase( 0, itrScopeOperator + CPPModel::SCOPE_RESOLUTION_OPERATOR.size() );
		}

		return( individualNamespaces );
	}



	Result			AddNamespace( CPPModel::ASTDictionary&			dictionary,
								  const std::string&				namespaceToAdd )
	{
		//	Return an error now if the namespace already exists

		if( dictionary.ContainsNamespace( namespaceToAdd ) )
		{
			return( Result::Failed( ResultCode::NAMESPACE_ALREADY_EXISTS, "Namespace Already Exists"  ) );
		}

		//	If this is a nested namespace, get the individual nested elements

		const std::list<std::string>		individualNamespaces( ParseFQNamespace( namespaceToAdd ) );

		//	Create an empty string that we will use to re-build the nested namespace.
		//		Initialize the parent namespace to the global namespace.

		std::string							currentFQNamespace = "";

  		const CPPModel::Namespace*			parentNamespace;

  		dictionary.GetNamespace( "::", parentNamespace );

		//	Iterate over each namespace nesting level

  		for( std::list<std::string>::const_iterator itrIndividualNamespace = individualNamespaces.begin(); itrIndividualNamespace != individualNamespaces.end(); itrIndividualNamespace++ )
		{
			//	Rebuild the fully qualified name and just loop again if the current fq namespace already exists.  Update the parent namespace accordingly.

  			currentFQNamespace += *itrIndividualNamespace;
			currentFQNamespace += CPPModel::SCOPE_RESOLUTION_OPERATOR;

			if( dictionary.ContainsNamespace( currentFQNamespace ) )
			{
				dictionary.GetNamespace( currentFQNamespace, parentNamespace );

				continue;
			}

			//	Pretty simple in GCC to create a new namespace.
			//		Create an identifier (it is not fully qualified), push the namespace then pop it.

			tree 	newNamespaceID = get_identifier( itrIndividualNamespace->c_str() );

			push_namespace( newNamespaceID );

			pop_namespace();

			//	Lookup the new namespace node.

			tree namespaceNode = lookup_name_prefer_type( newNamespaceID, 1 );

			//	Build the namespace object for the CPP model and add it to the dictionary

			DeclTree	namespaceTree( namespaceNode );

			CPPModel::ConstListPtr<CPPModel::Attribute>			noAttributes( CPPModel::Attributes::emptyList() );

			CPPModel::Namespace*	newNamespace = new NestedNamespaceImpl( namespaceTree.uid(),
																			namespaceTree.identifier(),
																			*parentNamespace,
																			namespaceTree.sourceLocation(),
																			namespaceTree.compilerSpecificAttr(),
																			noAttributes,
																			namespaceNode );

			dictionary.AddNamespace( newNamespace );

			//	Update the parent namespace

			parentNamespace = newNamespace;
		}

		//	If we are down here, all went well so return SUCCESS

  		return( Result::Success() );
	}



	Result			AddGlobalVar( CPPModel::ASTDictionary&						dictionary,
			  	  	  	  	  	  const CPPModel::GlobalVarDeclaration&			globalDecl )
	{
		//	TODO add proper error reporting

		//	Lookup the namespace

		const CPPModel::Namespace*			namespaceEntry = &globalDecl.namespaceScope();
		//	TODO	Finish building out this function

		tree __glob_id = get_identifier( globalDecl.name().c_str() );
		tree __glob_decl = build_decl(UNKNOWN_LOCATION, VAR_DECL, __glob_id, unsigned_type_node);

		/* allocate static storage for this variable */
		TREE_STATIC(__glob_decl) = true;

		/* static: internal linkage */
		TREE_PUBLIC(__glob_decl) = false;

		/* the context of this declaration: namespace scope */

		DECL_CONTEXT(__glob_decl) = (dynamic_cast<const GCCInternalsTools::DictionaryTreeMixin*>(namespaceEntry))->getTree();

		/* this declaration is used in its scope */
		TREE_USED(__glob_decl) = true;

		/* initialization to 0 */
		tree __glob_init_val = build_int_cstu(unsigned_type_node, 1);
		DECL_INITIAL(__glob_decl) = __glob_init_val;

		layout_decl(__glob_decl, false);
		rest_of_decl_compilation(__glob_decl, 1, 0);

		GCCInternalsTools::GlobalVarDecoder().Decode( __glob_decl, dictionary );


		//	If we are down here, all went well so return SUCCESS

  		return( Result::Success() );
	}

}

