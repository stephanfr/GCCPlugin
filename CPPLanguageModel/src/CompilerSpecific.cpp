/*
 * CompilerSpecific.cpp
 *
 *  Created on: Jul 20, 2013
 *      Author: steve
 */


#include <ostream>
#include <string>

#include "Serialization.h"

#include "CompilerSpecific.h"



namespace CPPModel
{

	std::ostream&	CompilerSpecific::toXML( std::ostream&				outputStream,
											 int						indentLevel,
											 SerializationOptions		options ) const
	{
		if( !m_builtIn && !m_artificial && !m_compilerGeneratedFunctionParam )
		{
			return( outputStream );
		}

		std::string		currentIndent = XMLIndentTable::GetIndent( indentLevel );
		std::string		currentIndentPlusOne = XMLIndentTable::GetIndent( indentLevel + 1 );


		outputStream << currentIndent << "<compiler_specific>\n";

		if( m_builtIn )
		{
			outputStream << currentIndentPlusOne << "</built_in>\n";
		}

		if( m_artificial )
		{
			outputStream << currentIndentPlusOne << "</artificial>\n";
		}

		if( m_compilerGeneratedFunctionParam )
		{
			outputStream << currentIndentPlusOne << "</compilerGeneratedFunctionParam>\n";
		}

		outputStream << currentIndent << "</compiler_specific>\n";

		return( outputStream );
	}

}
