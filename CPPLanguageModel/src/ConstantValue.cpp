/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/
/*
 * ConstantValue.cpp
 *
 *  Created on: May 12, 2013
 *      Author: steve
 */


#include <algorithm>
#include <ostream>
#include <string>

#include "Serialization.h"
#include "ConstantValue.h"

#include <boost/lexical_cast.hpp>



namespace CPPModel
{
	ConstantValue*		ConstantValue::Copy( const ConstantValue&		valueToCopy )
	{
		switch( valueToCopy.kind() )
		{
			case Kind::STRING :
				return( new StringConstant( ((const StringConstant&)valueToCopy).value() ) );
				break;

			case Kind::INTEGER :
				return( new IntegerConstant( ((const IntegerConstant&)valueToCopy).value() ) );
				break;

			case Kind::REAL :
				return( new RealConstant( ((const RealConstant&)valueToCopy).value() ) );
				break;

			case Kind::UNRECOGNIZED :
				return( new UnrecognizedConstant() );
				break;
		}

		return( new UnrecognizedConstant() );

	}


	std::ostream&	UnrecognizedConstant::toXML( std::ostream&			outputStream,
					   	   	   	   	   	   	   	 int					indentLevel,
					   	   	   	   	   	   	   	 SerializationOptions	options ) const
	{
		outputStream << XMLIndentTable::GetIndent( indentLevel ) << "</constant type=\"UNRECOGNIZED\">\n";

		return( outputStream );
	}


	std::ostream&	StringConstant::toXML( std::ostream&			outputStream,
					   	   	   	   	   	   int						indentLevel,
					   	   	   	   	   	   SerializationOptions		options ) const
	{
		outputStream << XMLIndentTable::GetIndent( indentLevel ) << "<constant type=\"STRING\">" << m_value << "</constant>\n";

		return( outputStream );
	}


	std::ostream&	IntegerConstant::toXML( std::ostream&			outputStream,
					   	   	   	   	   	    int						indentLevel,
					   	   	   	   	   	    SerializationOptions	options ) const
	{
		outputStream << XMLIndentTable::GetIndent( indentLevel ) << "<constant type=\"INTEGER\">" << (boost::lexical_cast<std::string>( m_value )) << "</constant>\n";

		return( outputStream );
	}

	std::ostream&	RealConstant::toXML( std::ostream&				outputStream,
					   	   	   	   	   	 int						indentLevel,
					   	   	   	   	   	 SerializationOptions		options ) const
	{
		outputStream << XMLIndentTable::GetIndent( indentLevel ) << "<constant type=\"REAL\">" << (boost::lexical_cast<std::string>( m_value )) << "</constant>\n";

		return( outputStream );
	}

}



