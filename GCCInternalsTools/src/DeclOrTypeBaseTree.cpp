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

#include "DeclOrTypeBaseTree.h"

#include "TypeTree.h"
#include "DeclTree.h"




namespace GCCInternalsTools
{


	std::unique_ptr<DeclOrTypeBaseTree>		DeclOrTypeBaseTree::convert( const tree&		treeToConvert )
	{
		if( DECL_P( treeToConvert ) )
		{
			return( std::unique_ptr<DeclOrTypeBaseTree>( new DeclTree( treeToConvert ) ));
		}
		else if( TYPE_P( treeToConvert ) )
		{
			return( std::unique_ptr<DeclOrTypeBaseTree>( new TypeTree( treeToConvert ) ));
		}

		assert( "Request to convert unknown tree to a descendant of TreeBase" );

		return( NULL );
	}

}
