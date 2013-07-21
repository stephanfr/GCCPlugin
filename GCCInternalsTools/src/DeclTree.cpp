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

#include "ConstantTree.h"
#include "IdentifierTree.h"
#include "DeclOrTypeBaseTree.h"
#include "TypeTree.h"
#include "DeclTree.h"
#include "TreeList.h"
#include "AttributeParser.h"




namespace GCCInternalsTools
{

	CPPModel::ConstListPtr<CPPModel::Attribute>			DeclTree::attributes()
	{
		return( GetAttributes( PurposeValueList( DECL_ATTRIBUTES( m_tree )) ));
	}


	const std::string				DeclTree::enclosingNamespace() const
	{
		std::string			declNamespace = "";

		//	Functions have to be handled distinctly from Types

		if( !isAFunctionDeclaration() )
		{
			//	Types first, they are pretty straightforward.  Use CP_DECL_CONTEXT.

			//	Namespaces can be aliased in the AST, make sure we get the original

			tree&			originalNamespace = ORIGINAL_NAMESPACE( CP_DECL_CONTEXT( m_tree ) );

			//	If this is the global namespace, return the scope resolution operator

			if( originalNamespace == global_namespace )
			{
				return( CPPModel::SCOPE_RESOLUTION_OPERATOR );
			}

			//	If this is the standard library namespace, then return that constant now

			if( DECL_NAMESPACE_STD_P( originalNamespace ))
			{
				return( CPPModel::STD_NAMESPACE_LABEL );
			}

			//	We have to build the namespace context by context

			for( tree& currentContext = originalNamespace; currentContext != global_namespace; currentContext = CP_DECL_CONTEXT( currentContext ) )
			{
				declNamespace += DeclTree( currentContext ).identifier() + CPPModel::SCOPE_RESOLUTION_OPERATOR;
			}
		}
		else
		{
			//	For functions, use DECL_CONTEXT.
			//
			//	Best I can tell, the DECL_CONTEXT for a function is a linked list which has the translation unit decl for the function
			//		as the last element in the list.  If a function is globally scoped, then the context just points to the translation unit.
			//		If the function is within a namespace, the namespace decl(s) will precede the translation unit decl.


			tree&			originalNamespace = ORIGINAL_NAMESPACE( DECL_CONTEXT( m_tree ) );

			//	If this is the global namespace, return the scope resolution operator

			if( TREE_CODE( originalNamespace ) != NAMESPACE_DECL )
			{
				return( CPPModel::SCOPE_RESOLUTION_OPERATOR );
			}

			//	If this is the standard library namespace, then return that constant now

			if( DECL_NAMESPACE_STD_P( originalNamespace ))
			{
				return( CPPModel::STD_NAMESPACE_LABEL );
			}

			//	Build the namespace up level by level.  For functions, if the original namespace is not
			//		a namespace decl then we have the global namespace

			if( TREE_CODE( originalNamespace ) == NAMESPACE_DECL )
			{
				for( tree& currentContext = originalNamespace; TREE_CODE( currentContext ) == NAMESPACE_DECL; currentContext = DECL_CONTEXT( currentContext ) )
				{
					declNamespace += DeclTree( currentContext ).identifier() + CPPModel::SCOPE_RESOLUTION_OPERATOR;
				}
			}
			else
			{
				declNamespace = CPPModel::SCOPE_RESOLUTION_OPERATOR;
			}
		}

		//	Return the namespace

		return( declNamespace );
	}

}




