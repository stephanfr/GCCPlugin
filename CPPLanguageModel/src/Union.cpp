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
#include <memory>
#include <ostream>
#include <string>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/global_fun.hpp>

#include <boost/ptr_container/ptr_list.hpp>

#include <boost/lexical_cast.hpp>

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



namespace CPPModel
{

	std::ostream&	UnionDefinition::toXML( std::ostream&			outputStream,
					   	   	   	   	  	    int						indentLevel,
					   	   	   	   	  	    SerializationOptions	options ) const
	{
		const std::string&		currentIndent = XMLIndentTable::GetIndent( indentLevel );
		const std::string&		currentIndentPlusOne = XMLIndentTable::GetIndent( indentLevel + 1 );

		outputStream << currentIndent << "<union>\n";

		SourceElement::toXML( outputStream, indentLevel + 1, options );
		Namespace::toXML( outputStream, indentLevel + 1, options );
		Attributes::toXML( outputStream, indentLevel + 1, options );

		outputStream << currentIndentPlusOne << "<members>\n";
		for( const UnionMember& itrMember : members() )
		{
			itrMember.toXML( outputStream, indentLevel + 2, options );
		}
		outputStream << currentIndentPlusOne << "</members>\n";

		outputStream << currentIndent << "</union>\n";;

		return( outputStream );
	}


	std::ostream&	UnionMember::toXML( std::ostream&			outputStream,
					   	   	   	   	  	int						indentLevel,
					   	   	   	   	  	SerializationOptions	options ) const
	{
		const std::string&		currentIndent = XMLIndentTable::GetIndent( indentLevel );
		const std::string&		currentIndentPlusOne = XMLIndentTable::GetIndent( indentLevel + 1 );

		outputStream << currentIndent << "<member>\n";
		NamedEntity::toXML( outputStream, indentLevel + 1, options );
		SourceLocation::toXML( outputStream, indentLevel + 1, options );
		type().toXML( outputStream, indentLevel + 1, options );
		outputStream << currentIndent << "</member>\n";

		return( outputStream );
	}

}









