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
#include <string>
#include <cassert>

#include <boost/ptr_container/ptr_list.hpp>

#include "ListAliases.h"

#include "Constants.h"
#include "Serialization.h"
#include "Namespace.h"
#include "ConstantValue.h"
#include "NamedEntity.h"
#include "Attribute.h"
#include "UID.h"
#include "SourceLocation.h"
#include "SourceElement.h"
#include "Types.h"




namespace CPPModel
{

	std::ostream&	UnrecognizedType::toXML( std::ostream&			outputStream,
											 int					indentLevel,
											 SerializationOptions	options ) const
	{
		const std::string&		currentIndent = XMLIndentTable::GetIndent( indentLevel );
		const std::string&		currentIndentPlusOne = XMLIndentTable::GetIndent( indentLevel + 1 );

		outputStream << currentIndent << "<type>\n";
		outputStream << currentIndentPlusOne << "<kind>unrecognized</kind>\n";
		outputStream << currentIndent << "</type>\n";

		Attributes::toXML( outputStream, indentLevel, options );

		return( outputStream );
	}


	std::ostream&	FundamentalType::toXML( std::ostream&			outputStream,
											int						indentLevel,
											SerializationOptions	options ) const
	{
		const std::string&		currentIndent = XMLIndentTable::GetIndent( indentLevel );
		const std::string&		currentIndentPlusOne = XMLIndentTable::GetIndent( indentLevel + 1 );

		outputStream << currentIndent << "<type>\n";
		outputStream << currentIndentPlusOne << "<kind>fundamental</kind>\n";
		outputStream << currentIndentPlusOne << "<declaration>" << CPPTypes[(int)type()].label.c_str() << "</declaration>\n";
		outputStream << currentIndent << "</type>\n";

		Attributes::toXML( outputStream, indentLevel, options );

		return( outputStream );
	}


	std::ostream&	UserDefinedType::toXML( std::ostream&			outputStream,
											int						indentLevel,
											SerializationOptions	options ) const
	{
		const std::string&		currentIndent = XMLIndentTable::GetIndent( indentLevel );
		const std::string&		currentIndentPlusOne = XMLIndentTable::GetIndent( indentLevel + 1 );

		outputStream << currentIndent << "<type>\n";
		outputStream << currentIndentPlusOne << "<kind>userDefined</kind>\n";
		outputStream << currentIndentPlusOne << "<declaration>" << name().c_str() << "</declaration>\n";
		Namespace::toXML( outputStream, indentLevel + 1, options );
		outputStream << currentIndent << "</type>\n";

		Attributes::toXML( outputStream, indentLevel, options );

		return( outputStream );
	}


	std::ostream&	DerivedType::toXML( std::ostream&			outputStream,
										int						indentLevel,
										SerializationOptions	options ) const
	{
		const std::string&		currentIndent = XMLIndentTable::GetIndent( indentLevel );
		const std::string&		currentIndentPlusOne = XMLIndentTable::GetIndent( indentLevel + 1 );
		const std::string&		currentIndentPlusTwo = XMLIndentTable::GetIndent( indentLevel + 2 );


		outputStream << currentIndent << "<type>\n";
		outputStream << currentIndentPlusOne << "<kind>derived</kind>\n";
		outputStream << currentIndentPlusOne << "<declaration>\n";

		Type*		currentBaseType = (Type*)this;
		while( currentBaseType->kind() == Type::Kind::DERIVED )
		{
			outputStream << currentIndentPlusTwo << "<operator>" << CPPTypes[(int)currentBaseType->type()].label << "</operator>\n";
			currentBaseType = (Type*)(&(((DerivedType*)currentBaseType)->baseType()));
		}

		((Type*)currentBaseType)->toXML( outputStream, indentLevel + 2, options );
		outputStream << currentIndentPlusOne << "</declaration>\n";

		outputStream << currentIndent << "</type>\n";

		Attributes::toXML( outputStream, indentLevel, options );


		return( outputStream );
	}


	std::ostream&	UnionType::toXML( std::ostream&				outputStream,
									  int						indentLevel,
									  SerializationOptions		options ) const
	{
		const std::string&		currentIndent = XMLIndentTable::GetIndent( indentLevel );
		const std::string&		currentIndentPlusOne = XMLIndentTable::GetIndent( indentLevel + 1 );


		outputStream << currentIndent << "<type>\n";
		outputStream << currentIndentPlusOne << "<kind>union</kind>\n";
		outputStream << currentIndentPlusOne << "<declaration>" << name().c_str() << "</declaration>\n";
		Namespace::toXML( outputStream, indentLevel, options );
		outputStream << currentIndent << "</type>\n";

		return( outputStream );
	}

}




