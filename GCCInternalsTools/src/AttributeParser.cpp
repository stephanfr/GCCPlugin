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
#include "Result.h"
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

	std::unique_ptr<const CPPModel::Attribute>		GetAttribute( const IdentifierTree&			identifier,
	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  const PurposeValueList&		arguments )
	{
		//	Extract the arguments for the attribute
		//		Attribute arguments are stored in a TREE_LIST without PURPOSE fields, so we just have to
		//		worry about the VALUE fields

		CPPModel::ListPtr<CPPModel::ConstantValue>		argumentList( new boost::ptr_list<CPPModel::ConstantValue>() );

		for( PurposeValueList::iterator currentArgument = arguments.begin(); currentArgument != arguments.end(); ++currentArgument )
		{
			//	I don't fully understand this but sometimes the tree value is NULL which I suspect means a null list ???

			if( currentArgument->value() == NULL )
			{
				break;
			}

			//	Get the value and add it to our list of attributes

			if( CONSTANT_CLASS_P( currentArgument->value() ))
			{
				//	Generally, arguments are strings but I have tripped over a couple of integer constants

				argumentList->push_back( ConstantTree( currentArgument->value() ).value().release() );
			}
			else if( TREE_CODE( currentArgument->value() ) == IDENTIFIER_NODE )
			{
				//	It appears as if sometimes there are identifiers rather than constants

				argumentList->push_back( new CPPModel::StringConstant( IDENTIFIER_POINTER( currentArgument->value() ) ));
			}
			else
			{
				argumentList->push_back( new CPPModel::UnrecognizedConstant() );
			}
		}

		return( std::unique_ptr<const CPPModel::Attribute>( new CPPModel::Attribute( identifier, argumentList )));
	}


	CPPModel::ConstListPtr<CPPModel::Attribute>			GetAttributes( const PurposeValueList&		treeList )
	{
		//	I'd rather have a list of 'const Attribute' but the lib seems to have a bug in it that causes problems
		//		with const elements of in a ptr_list<>.

		CPPModel::ListPtr<CPPModel::Attribute>			attributeList( new boost::ptr_list<CPPModel::Attribute>() );

		for( PurposeValueList::iterator attribute = treeList.begin(); attribute != treeList.end(); ++attribute )
		{
			attributeList->push_back( (CPPModel::Attribute*)GetAttribute( IdentifierTree( attribute->purpose() ), PurposeValueList( attribute->value() )).release() );
		}

		//	Return a moved unique pointer to a const-ed version of the list

		return( CPPModel::ConstListPtr<CPPModel::Attribute>( std::move( attributeList )));
	}
}

