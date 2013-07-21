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
#include <memory>
#include <ostream>
#include <string>

#include <boost/ptr_container/ptr_list.hpp>

#include "ListAliases.h"

#include "Serialization.h"
#include "ConstantValue.h"
#include "NamedEntity.h"

#include "Attribute.h"


namespace CPPModel
{


	std::ostream&	Attribute::toXML( std::ostream&				outputStream,
						   	   	      int						indentLevel,
						   	   	      SerializationOptions		options ) const
	{
		const std::string&		currentIndent = XMLIndentTable::GetIndent( indentLevel );
		const std::string&		currentIndentPlusOne = XMLIndentTable::GetIndent( indentLevel + 1 );

		outputStream << currentIndent + "<attribute>\n";
		NamedEntity::toXML( outputStream, indentLevel + 1, options );

		if( !m_arguments->empty() )
		{
			outputStream << currentIndentPlusOne << "<arguments>\n";

			for( const ConstantValue& itrArgument : *m_arguments )
			{
				itrArgument.toXML( outputStream, indentLevel + 2, options );
			}

			outputStream << currentIndentPlusOne << "</arguments>\n";
		}

		outputStream << currentIndent + "</attribute>\n";

		return( outputStream );
	}



	std::ostream&	Attributes::toXML( std::ostream&			outputStream,
						   	   	       int						indentLevel,
						   	   	       SerializationOptions		options ) const
	{
		//	If we have been passed the NO_ATTRIBUTES option or there are now attributes, then return now

		if( MatchOptions( options, SerializationOptions::NO_ATTRIBUTES ) || m_attributes->empty() )
		{
			return( outputStream );
		}


		const std::string&		currentIndent = XMLIndentTable::GetIndent( indentLevel );

		outputStream << currentIndent << "<attributes>\n";

		for( const Attribute& itrAttribute : *m_attributes )
		{
			itrAttribute.toXML( outputStream, indentLevel + 1, options );
		}

		outputStream << currentIndent << "</attributes>\n";

		return( outputStream );
	}

}
