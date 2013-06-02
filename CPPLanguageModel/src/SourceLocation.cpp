/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/


#include <algorithm>
#include <ostream>
#include <string>

#include "Serialization.h"

#include "SourceLocation.h"


#include <boost/lexical_cast.hpp>



namespace CPPModel
{

	std::ostream&	SourceLocation::toXML( std::ostream&			outputStream,
							  	  	  	   int						indentLevel,
							  	  	  	   SerializationOptions		options ) const

	{
		const std::string&		currentIndent = XMLIndentTable::GetIndent( indentLevel );
		const std::string&		currentIndentPlusOne = XMLIndentTable::GetIndent( indentLevel + 1 );

		outputStream << currentIndent << "<source-info>\n";
		outputStream << currentIndentPlusOne << "<file>" << m_fileName << "</file>\n";
		outputStream << currentIndentPlusOne << "<line>" << (boost::lexical_cast<std::string>(m_lineNumber)) << "</line>\n";
		outputStream << currentIndentPlusOne << "<char-count>" << (boost::lexical_cast<std::string>(m_characterCount)) << "</char-count>\n";
		outputStream << currentIndentPlusOne << "<location>" << (boost::lexical_cast<std::string>(m_location)) << "</location>\n";
		outputStream << currentIndent << "</source-info>\n";

		return( outputStream );
	}

}



