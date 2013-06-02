/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/
/*
 * IdentifierTree.h
 *
 *  Created on: May 7, 2013
 *      Author: steve
 */

#ifndef IDENTIFIERTREE_H_
#define IDENTIFIERTREE_H_


namespace GCCInternalsTools
{

	class IdentifierTree
	{
	public :

		IdentifierTree( const tree&		identifierTree )
			: m_tree( identifierTree )
		{
			assert( TREE_CODE( identifierTree ) == IDENTIFIER_NODE );
		}


		IdentifierTree&	operator= ( const tree&		identifierTree )
		{
			assert( TREE_CODE( identifierTree ) == IDENTIFIER_NODE );

			(tree&)m_tree = identifierTree;

			return( *this );
		}

		operator const tree&() const
		{
			return( m_tree );
		}

		operator const char*() const
		{
			return( IDENTIFIER_POINTER( m_tree ) );
		}

		CPPModel::AccessSpecifier			AccessSpecifier()
		{
			//	This is pretty straightforward.  If there is no access specifier node, then we'll default to public.

			if( m_tree == 0 || m_tree == access_public_node )
			{
				return( CPPModel::AccessSpecifier::PUBLIC );
			}
			else if( m_tree == access_protected_node )
			{
				return( CPPModel::AccessSpecifier::PROTECTED );
			}

			return( CPPModel::AccessSpecifier::PRIVATE );
		}

	private :

		const tree&			m_tree;
	};
}



#endif /* IDENTIFIERTREE_H_ */
