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
#include "AttributeParser.h"




namespace GCCInternalsTools
{

	CPPModel::ConstListPtr<CPPModel::Attribute>			DeclTree::attributes()
	{
		return( GetAttributes( PurposeValueList( DECL_ATTRIBUTES( m_tree )) ));
	}


	const std::string				DeclTree::Namespace() const
	{
		std::string			declNamespace = "";

		//	Test to see if this is the std namespace.  If yes, then we are done.

		if( DECL_NAMESPACE_STD_P( m_tree ))
		{
			declNamespace = "std::";

			return( declNamespace );
		}

		//	Functions have to be handled distinctly from Types

		if( !isAFunctionDeclaration() )
		{
			//	Types first, they are pretty straightforward.  Use CP_DECL_CONTEXT.

			for( tree& currentContext = CP_DECL_CONTEXT( m_tree ); currentContext != global_namespace; currentContext = CP_DECL_CONTEXT( currentContext ) )
			{
				declNamespace += std::string( DeclTree( currentContext ).identifier() + NAMESPACE_SEPARATOR );
			}
		}
		else
		{
			//	For functions, use DECL_CONTEXT.
			//
			//	Best I can tell, the DECL_CONTEXT for a function is a linked list which has the translation unit decl for the function
			//		as the last element in the list.  If a function is globally scoped, then the context just points to the translation unit.
			//		If the function is within a namespace, the namespace decl(s) will precede the translation unit decl.

			if( TREE_CODE( DECL_CONTEXT( m_tree )) == NAMESPACE_DECL )
			{
				for( tree& currentContext = DECL_CONTEXT( m_tree ); TREE_CODE( currentContext ) == NAMESPACE_DECL; currentContext = DECL_CONTEXT( currentContext ) )
				{
					declNamespace += std::string( DeclTree( currentContext ).identifier() + NAMESPACE_SEPARATOR );
				}
			}
		}

		return( declNamespace );
	}

}




