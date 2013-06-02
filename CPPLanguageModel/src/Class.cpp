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

#include "Function.h"

#include "Class.h"



namespace CPPModel
{

	std::ostream&	BaseClassIdentifier::toXML( std::ostream&			outputStream,
					   	   	   	   	   	   	    int						indentLevel,
					   	   	   	   	   	   	    SerializationOptions	options ) const
	{
		std::string		currentIndent = XMLIndentTable::GetIndent( indentLevel );

		outputStream << currentIndent << "<base-class>\n";
		NamedEntity::toXML( outputStream, indentLevel + 1, options );
		outputStream << currentIndent << XMLIndentTable::GetIndent( 1 ) << "<virtual>" << (boost::lexical_cast<std::string>( isVirtual() )) << "</virtual>\n";
		outputStream << currentIndent << XMLIndentTable::GetIndent( 1 ) << "<access>" << std::string( toString( accessSpecifier() )) << "</access>\n";
		outputStream << currentIndent << "</base-class>\n";

		return( outputStream );
	}


	std::ostream&	FriendClassIdentifier::toXML( std::ostream&				outputStream,
					   	   	   	   	   	   	 	  int						indentLevel,
					   	   	   	   	   	   	 	  SerializationOptions		options ) const
	{
		std::string		currentIndent = XMLIndentTable::GetIndent( indentLevel );

		outputStream << currentIndent << "<friend>\n";
		outputStream << currentIndent << XMLIndentTable::GetIndent( 1 ) << "<type>CLASS</type>\n";
		m_type->toXML( outputStream, indentLevel + 1, addOption( options, SerializationOptions::NO_ATTRIBUTES ) );
		outputStream << currentIndent << "</friend>\n";

		return( outputStream );
	}


	std::ostream&	FriendClassMemberIdentifier::toXML( std::ostream&			outputStream,
					   	   	   	   	   	   	 	  	    int						indentLevel,
					   	   	   	   	   	   	 	  	    SerializationOptions	options ) const
	{
		std::string		currentIndent = XMLIndentTable::GetIndent( indentLevel );

		outputStream << currentIndent << "<friend>\n";
		outputStream << currentIndent << XMLIndentTable::GetIndent( 1 ) << "<type>CLASS_MEMBER</type>\n";
		m_type->toXML( outputStream, indentLevel + 1, addOption( options, SerializationOptions::NO_ATTRIBUTES ) );
		outputStream << currentIndent << "</friend>\n";

		return( outputStream );
	}


	std::ostream&	FriendFunctionIdentifier::toXML( std::ostream&				outputStream,
					   	   	   	   	   	   	 	     int						indentLevel,
					   	   	   	   	   	   	 	     SerializationOptions		options ) const
	{
		std::string		currentIndent = XMLIndentTable::GetIndent( indentLevel );

		outputStream << currentIndent << "<friend>\n";
		outputStream << currentIndent << XMLIndentTable::GetIndent( 1 ) << "<type>FUNCTION</type>\n";
		outputStream << currentIndent << "</friend>\n";

		return( outputStream );
	}



	std::ostream&	FieldDeclaration::toXML( std::ostream&				outputStream,
					   	   	   	   	   	   	 int						indentLevel,
					   	   	   	   	   	   	 SerializationOptions		options ) const
	{
		std::string		currentIndent = XMLIndentTable::GetIndent( indentLevel );
		std::string		currentIndentPlusOne = XMLIndentTable::GetIndent( indentLevel + 1 );

		outputStream << currentIndent << "<field>\n";
		NamedEntity::toXML( outputStream, indentLevel + 1, options );
		SourceLocation::toXML( outputStream, indentLevel + 1, options );
		type().toXML( outputStream, indentLevel + 1, addOption( options, SerializationOptions::NO_ATTRIBUTES ) );
		Access::toXML( outputStream, indentLevel + 1, options );
		Static::toXML( outputStream, indentLevel + 1, options );
		Attributes::toXML( outputStream, indentLevel + 1, options );
		outputStream << currentIndent << "</field>\n";

		return( outputStream );
	}


	std::ostream&	MethodDeclaration::toXML( std::ostream&				outputStream,
					   	   	   	   	   	   	  int						indentLevel,
					   	   	   	   	   	   	  SerializationOptions		options ) const
	{
		std::string		currentIndent = XMLIndentTable::GetIndent( indentLevel );
		std::string		currentIndentPlusOne = XMLIndentTable::GetIndent( indentLevel + 1 );

		outputStream << currentIndent << "<method>\n";
		SourceElement::toXML( outputStream, indentLevel + 1, options );
		Access::toXML( outputStream, indentLevel + 1, options );
		Static::toXML( outputStream, indentLevel + 1, options );
		outputStream << currentIndentPlusOne << "<result>\n";
		resultType().toXML( outputStream, indentLevel + 2, addOption( options, SerializationOptions::NO_ATTRIBUTES ) );
		outputStream << currentIndentPlusOne << "</result>\n";

		outputStream << currentIndentPlusOne << "<parameters>\n";
		for( boost::ptr_list<FunctionParameter>::const_iterator itrParam = m_parameterList->begin(); itrParam != m_parameterList->end(); itrParam++ )
		{
			itrParam->toXML( outputStream, indentLevel + 2, options );
		}
		outputStream << currentIndentPlusOne << "</parameters>\n";

		Attributes::toXML( outputStream, indentLevel + 1, options );

		outputStream << currentIndent << "</method>\n";

		return( outputStream );
	}


	std::ostream&	ClassDefinition::toXML( std::ostream&			outputStream,
					   	   	   	   	   	   	int						indentLevel,
					   	   	   	   	   	   	SerializationOptions	options ) const
	{
		std::string		currentIndent = XMLIndentTable::GetIndent( indentLevel );
		std::string		currentIndentPlusOne = XMLIndentTable::GetIndent( indentLevel + 1 );


		if( !isStruct() )
		{
			outputStream << currentIndent << "<class type=\"class\">\n";
		}
		else
		{
			outputStream << currentIndent << "<class type=\"struct\">\n";
		}

		SourceElement::toXML( outputStream, indentLevel + 1, options );
		Namespace::toXML( outputStream, indentLevel + 1, options );

		Attributes::toXML( outputStream, indentLevel + 1, options );

		outputStream << currentIndentPlusOne << "<base-classes>\n";

		for( boost::ptr_list<BaseClassIdentifier>::const_iterator itr = baseClasses().begin(); itr != baseClasses().end(); itr++ )
		{
			itr->toXML( outputStream, indentLevel + 2, options );
		}
		outputStream << currentIndentPlusOne << "</base-classes>\n";

		outputStream << currentIndentPlusOne << "<friends>\n";
		for( boost::ptr_list<const FriendIdentifier>::const_iterator itr = friends().begin(); itr != friends().end(); itr++ )
		{
			itr->toXML( outputStream, indentLevel + 2, options );
		}
		outputStream << currentIndentPlusOne << "</friends>\n";

		outputStream << currentIndentPlusOne << "<fields>\n";
		for( boost::ptr_list<const FieldDeclaration>::const_iterator itr = fields().begin(); itr != fields().end(); itr++ )
		{
			itr->toXML( outputStream, indentLevel + 2, options );
		}
		outputStream << currentIndentPlusOne << "</fields>\n";

		outputStream << currentIndentPlusOne << "<methods>\n";
		for( boost::ptr_list<const MethodDeclaration>::const_iterator itr = methods().begin(); itr != methods().end(); itr++ )
		{
			itr->toXML( outputStream, indentLevel + 2, options );
		}
		outputStream << currentIndentPlusOne << "</methods>\n";

		outputStream << currentIndent << "</class>\n";

		return( outputStream );
	}

}





