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

#include "config.h"
#include "gcc-plugin.h"
#include "tree.h"
#include "cp/cp-tree.h"

#include "DeclTree.h"
#include "AttributeParser.h"





namespace GCCInternalsTools
{

	CPPModel::ConstListPtr<CPPModel::Attribute>			DeclTree::attributes()
	{
		return( GetAttributes( PurposeValueList( DECL_ATTRIBUTES( m_tree )) ));
	}


	const std::string									DeclTree::enclosingNamespace() const
	{
		std::string			declScope = "";

		//	Functions have to be handled distinctly from Types

		if( TREE_CODE( m_tree ) == RECORD_TYPE )
		{
			//	Types first, they are pretty straightforward.  Use CP_DECL_CONTEXT.

			tree&			declType = TREE_TYPE( m_tree );

			//	Namespaces can be aliased in the AST, make sure we get the original

			tree&			startingScope = CP_DECL_CONTEXT( declType );

			//	If this is the global namespace, return the scope resolution operator

			if( startingScope == global_namespace )
			{
				return( CPPModel::SCOPE_RESOLUTION_OPERATOR );
			}

			//	If this is the standard library namespace, then return that constant now

			if( DECL_NAMESPACE_STD_P( startingScope ))
			{
				return( CPPModel::STD_NAMESPACE_LABEL );
			}

			//	We have to build the namespace context by context

			for( tree& currentScope = startingScope; currentScope != global_namespace; currentScope = CP_DECL_CONTEXT( currentScope ) )
			{
				if( TREE_CODE( currentScope ) == RECORD_TYPE )
				{
					currentScope = TYPE_NAME( currentScope );
				}

				declScope = DeclTree( currentScope ).identifier() + CPPModel::SCOPE_RESOLUTION_OPERATOR + declScope;
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
				for( tree& currentContext = originalNamespace; ( currentContext != NULL ) && ( TREE_CODE( currentContext ) == NAMESPACE_DECL ); currentContext = DECL_CONTEXT( currentContext ) )
				{
					declScope = DeclTree( currentContext ).identifier() + CPPModel::SCOPE_RESOLUTION_OPERATOR + declScope;
				}
			}
			else
			{
				declScope = CPPModel::SCOPE_RESOLUTION_OPERATOR;
			}
		}

		//	Return the namespace

		return( declScope );
	}

}




