/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/


#include <memory>
#include <string>
#include <cassert>

#include <boost/ptr_container/ptr_list.hpp>

#include <boost/lexical_cast.hpp>

#include "ListAliases.h"

#include "Constants.h"
#include "Serialization.h"
#include "Namespace.h"
#include "NamedEntity.h"
#include "ConstantValue.h"
#include "Attribute.h"
#include "UID.h"
#include "SourceLocation.h"

#include "SourceElement.h"


namespace CPPModel
{

	std::ostream&	SourceElement::toXML( std::ostream&				outputStream,
										  int						indentLevel,
										  SerializationOptions		options ) const
	{
		const std::string&		currentIndent = XMLIndentTable::GetIndent( indentLevel );


		NamedEntity::toXML( outputStream, indentLevel, options );
		outputStream << currentIndent << "<uid>" << (boost::lexical_cast<std::string>(uid().value())) << "</uid>\n";

		SourceLocation::toXML( outputStream, indentLevel, options );

		return( outputStream );
	}

}
