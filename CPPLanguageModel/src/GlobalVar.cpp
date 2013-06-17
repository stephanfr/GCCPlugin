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

#include "GlobalVar.h"



namespace CPPModel
{

	std::ostream&	GlobalVarDefinition::toXML( std::ostream&			outputStream,
					   	   	   	   	   	   	    int						indentLevel,
					   	   	   	   	   	   	    SerializationOptions	options ) const
	{
		std::string		currentIndent = XMLIndentTable::GetIndent( indentLevel );
		std::string		currentIndentPlusOne = XMLIndentTable::GetIndent( indentLevel + 1 );


		outputStream << currentIndent << "<global_var>\n";

		SourceElement::toXML( outputStream, indentLevel + 1, options );
		Namespace::toXML( outputStream, indentLevel + 1, options );
		Static::toXML( outputStream, indentLevel + 1, options );
		type().toXML( outputStream, indentLevel + 1, addOption( options, SerializationOptions::NO_ATTRIBUTES ));
		Attributes::toXML( outputStream, indentLevel + 1, options );

		outputStream << currentIndent << "</global_var>\n";

		return( outputStream );
	}

}












