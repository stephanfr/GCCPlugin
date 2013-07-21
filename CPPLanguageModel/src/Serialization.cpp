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
 * Serialization.cpp
 *
 *  Created on: Dec 27, 2012
 *      Author: steve
 */


#include <algorithm>
#include <ostream>
#include <string>


#include "Serialization.h"
#include "CompilerSpecific.h"


namespace CPPModel
{

	const std::string XMLIndentTable::IndentTable[] = {
		{ std::string() },
		{ std::string( 1 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 2 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 3 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 4 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 5 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 6 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 7 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 8 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 9 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 10 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 11 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 12 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 13 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 14 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 15 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 16 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 17 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 18 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 19 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 20 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 21 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 22 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 23 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 24 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 25 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 26 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 27 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 28 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 29 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 30 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 31 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 32 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 33 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 34 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 35 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 36 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 37 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 38 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 39 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 40 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 41 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 42 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 43 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 44 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 45 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 46 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 47 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 48 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 49 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 50 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 51 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 52 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 53 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 54 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 55 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 56 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 57 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 58 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 59 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 60 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 61 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 62 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 63 * XML_INDENT_SIZE, ' ' ) },
		{ std::string( 64 * XML_INDENT_SIZE, ' ' ) }
	};


	bool						MatchOptions( SerializationOptions			firstOption,
							  	  	  	  	  SerializationOptions			secondOption )
	{
		return( (std::int64_t)firstOption && (std::int64_t)secondOption );
	}

	SerializationOptions		AddOption( SerializationOptions				option,
						   	   	   	   	   SerializationOptions				optionToAdd )
	{
		return( (SerializationOptions)((std::int64_t)option | (std::int64_t)optionToAdd) );
	}

}
