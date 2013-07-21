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

#include <memory>
#include <map>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

#include <boost/ptr_container/ptr_list.hpp>
#include <boost/ptr_container/ptr_map.hpp>

#include "config.h"

#include "gcc-plugin.h"

#include "tree.h"

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
