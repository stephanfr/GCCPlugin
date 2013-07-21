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


namespace CPPModel
{

	std::ostream&	ASTEntry::toXML( std::ostream&				outputStream,
									 int						indentLevel,
									 SerializationOptions		options ) const
	{
		const std::string&		currentIndent = XMLIndentTable::GetIndent( indentLevel );


		outputStream << currentIndent << "<uid>" << (boost::lexical_cast<std::string>(uid().uidValue())) << "</uid>\n";

		SourceLocation::toXML( outputStream, indentLevel, options );

		return( outputStream );
	}

}
