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
#include "NamedEntity.h"
#include "Attribute.h"
#include "UID.h"
#include "SourceLocation.h"
#include "Namespace.h"
#include "Static.h"
#include "Access.h"
#include "SourceElement.h"
#include "ASTEntry.h"
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

	bool			AddGlobalVar( CPPModel::ASTDictionary&						dictionary,
			  	  	  	  	  	  const CPPModel::GlobalVarDeclaration&			globalDecl )
	{
		//	TODO add proper error reporting

		//	Lookup the namespace

		const CPPModel::NamespaceEntry*			namespaceEntry;

		if( !dictionary.GetNamespace( "TestNamespace", namespaceEntry ))
		{
			return( false );
		}

		//	TODO	Finish building out this function

		tree __glob_id = get_identifier( globalDecl.name().c_str() );
		tree __glob_decl = build_decl(UNKNOWN_LOCATION, VAR_DECL, __glob_id, unsigned_type_node);

		/* allocate static storage for this variable */
		TREE_STATIC(__glob_decl) = true;

		/* static: internal linkage */
		TREE_PUBLIC(__glob_decl) = false;

		/* the context of this declaration: file scope */
		DECL_CONTEXT(__glob_decl) = ((const GCCInternalsTools::NamespaceEntryImpl*)namespaceEntry)->getTree();

		/* this declaration is used in its scope */
		TREE_USED(__glob_decl) = true;

		/* initialization to 0 */
		tree __glob_init_val = build_int_cstu(unsigned_type_node, 1);
		DECL_INITIAL(__glob_decl) = __glob_init_val;

		layout_decl(__glob_decl, false);
		rest_of_decl_compilation(__glob_decl, 1, 0);

		GCCInternalsTools::GlobalVarDecoder().Decode( __glob_decl, dictionary );

		return __glob_decl;

	}

}

