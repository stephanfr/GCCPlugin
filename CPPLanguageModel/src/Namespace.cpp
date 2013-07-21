/*
 * Namespace.cpp
 *
 *  Created on: Jul 18, 2013
 *      Author: steve
 */


#include <string>
#include <iostream>
#include <memory>

#include <boost/ptr_container/ptr_list.hpp>

#include "ListAliases.h"

#include "Serialization.h"
#include "ConstantValue.h"
#include "SourceLocation.h"
#include "CompilerSpecific.h"
#include "UID.h"
#include "NamedEntity.h"
#include "Attribute.h"
#include "SourceElement.h"
#include "Namespace.h"


namespace CPPModel
{

	std::ostream&	Namespace::toXML( std::ostream&				outputStream,
									  int						indentLevel,
									  SerializationOptions		options ) const
	{
		if( MatchOptions( SerializationMask(), options ) )
		{
			return( outputStream );
		}

		std::string		currentIndent = XMLIndentTable::GetIndent( indentLevel );
		std::string		currentIndentPlusOne = XMLIndentTable::GetIndent( indentLevel + 1 );

		outputStream << currentIndent << "<namespace>\n";
		outputStream << currentIndentPlusOne << "<name>" << fqName() << "</name>\n";
		CompilerSpecific::toXML( outputStream, indentLevel + 1, options );
		outputStream << currentIndent << "</namespace>\n";

		return( outputStream );
	}
}
