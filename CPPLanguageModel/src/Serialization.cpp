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


//	ASTDictionary includes pretty much everything we need in terms of includes

#include "ASTDictionary.h"


#include <boost/lexical_cast.hpp>



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



	//
	//	Serializers for various classes follow.  This cleans up the project such that we don't have
	//		a bunch of .cpp files with only a serializer in them.
	//


	//
	//	Access
	//


	std::ostream&		Access::toXML( std::ostream&			outputStream,
									   int						indentLevel,
									   SerializationOptions		options ) const
	{
		outputStream << XMLIndentTable::GetIndent( indentLevel ) << "<access>" << toString( m_accessSpec ) << "</access>\n";

		return( outputStream );
	}


	//
	//	ASTEntry
	//

	std::ostream&	ASTEntry::toXML( std::ostream&				outputStream,
									 int						indentLevel,
									 SerializationOptions		options ) const
	{
		const std::string&		currentIndent = XMLIndentTable::GetIndent( indentLevel );


		outputStream << currentIndent << "<uid>" << (boost::lexical_cast<std::string>(uid().uidValue())) << "</uid>\n";

		SourceLocation::toXML( outputStream, indentLevel, options );

		return( outputStream );
	}

	//
	//	ASTDictionary
	//


	std::ostream&	DictionaryEntry::toXML( std::ostream&			outputStream,
											int						indentLevel,
											SerializationOptions	options ) const
	{
		const std::string&		currentIndent = XMLIndentTable::GetIndent( indentLevel );
		const std::string&		currentIndentPlusOne = XMLIndentTable::GetIndent( indentLevel + 1 );


		outputStream << currentIndent << "<dictionary_entry>\n";

		outputStream << currentIndentPlusOne << "<name>" << name() << "</name>\n";
		outputStream << currentIndentPlusOne << "<uid>" << (boost::lexical_cast<std::string>(uid().uidValue())) << "</uid>\n";

		enclosingNamespace().toXML( outputStream, indentLevel +1, options );

		//	Be careful below, classes cannot be static and are fixed false in the constructor for their
		//		dictionary entry.  If you punch out the 'static' flag all the time then it will
		//		always appear as false for classes (beyond just not making sense for classes anyway).

		if( isStatic() )
		{
			outputStream << currentIndentPlusOne << "<static>true</static>\n";
		}

		sourceLocation().toXML( outputStream, indentLevel + 1, options );

		attributes().toXML( outputStream, indentLevel + 1, options );

		outputStream << currentIndent << "</dictionary_entry>\n";

		return( outputStream );
	}



	//
	//	Attributes
	//


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


	//
	//	Class
	//


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
		m_type->toXML( outputStream, indentLevel + 1, AddOption( options, SerializationOptions::NO_ATTRIBUTES ) );
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
		m_type->toXML( outputStream, indentLevel + 1, AddOption( options, SerializationOptions::NO_ATTRIBUTES ) );
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
		type().toXML( outputStream, indentLevel + 1, AddOption( options, SerializationOptions::NO_ATTRIBUTES ) );
		Access::toXML( outputStream, indentLevel + 1, options );
		Static::toXML( outputStream, indentLevel + 1, options );
		attributes().toXML( outputStream, indentLevel + 1, options );
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
		resultType().toXML( outputStream, indentLevel + 2, AddOption( options, SerializationOptions::NO_ATTRIBUTES ) );
		outputStream << currentIndentPlusOne << "</result>\n";

		outputStream << currentIndentPlusOne << "<parameters>\n";
		for( boost::ptr_list<FunctionParameter>::const_iterator itrParam = m_parameterList->begin(); itrParam != m_parameterList->end(); itrParam++ )
		{
			itrParam->toXML( outputStream, indentLevel + 2, options );
		}
		outputStream << currentIndentPlusOne << "</parameters>\n";

		attributes().toXML( outputStream, indentLevel + 1, options );

		outputStream << currentIndent << "</method>\n";

		return( outputStream );
	}


	std::ostream&	TemplateMethodDeclaration::toXML( std::ostream&				outputStream,
					   	   	   	   	   	   	  	  	  int						indentLevel,
					   	   	   	   	   	   	  	  	  SerializationOptions		options ) const
	{
		std::string		currentIndent = XMLIndentTable::GetIndent( indentLevel );
		std::string		currentIndentPlusOne = XMLIndentTable::GetIndent( indentLevel + 1 );

		outputStream << currentIndent << "<template_method>\n";
		SourceElement::toXML( outputStream, indentLevel + 1, options );
		Access::toXML( outputStream, indentLevel + 1, options );
		Static::toXML( outputStream, indentLevel + 1, options );
		outputStream << currentIndentPlusOne << "<result>\n";
		resultType().toXML( outputStream, indentLevel + 2, AddOption( options, SerializationOptions::NO_ATTRIBUTES ) );
		outputStream << currentIndentPlusOne << "</result>\n";

		outputStream << currentIndentPlusOne << "<template_parameters>\n";
		for( boost::ptr_list<TemplateParameter>::const_iterator itrParam = m_parameterList->begin(); itrParam != m_parameterList->end(); itrParam++ )
		{
			itrParam->toXML( outputStream, indentLevel + 2, options );
		}
		outputStream << currentIndentPlusOne << "</template_parameters>\n";

		attributes().toXML( outputStream, indentLevel + 1, options );

		outputStream << currentIndent << "</template_method>\n";

		return( outputStream );
	}


	std::ostream&	ClassDefinition::toXML( std::ostream&			outputStream,
					   	   	   	   	   	   	int						indentLevel,
					   	   	   	   	   	   	SerializationOptions	options ) const
	{
		if( MatchOptions( SerializationMask(), options ) )
		{
			return( outputStream );
		}

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
		NamespaceScoped::toXML( outputStream, indentLevel + 1, options );
		CompilerSpecific::toXML( outputStream, indentLevel + 1, options );

		attributes().toXML( outputStream, indentLevel + 1, options );

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

		outputStream << currentIndentPlusOne << "<template_methods>\n";
		for( boost::ptr_list<const TemplateMethodDeclaration>::const_iterator itr = methods().begin(); itr != methods().end(); itr++ )
		{
			itr->toXML( outputStream, indentLevel + 2, options );
		}
		outputStream << currentIndentPlusOne << "</template_methods>\n";

		outputStream << currentIndent << "</class>\n";

		return( outputStream );
	}

	//
	//	Compiler Specific
	//


	std::ostream&	CompilerSpecific::toXML( std::ostream&				outputStream,
											 int						indentLevel,
											 SerializationOptions		options ) const
	{
		if( !m_builtIn && !m_artificial )
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

		outputStream << currentIndent << "</compiler_specific>\n";

		return( outputStream );
	}

	//
	//	Constant Values
	//


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

	//
	//	DeclarationType
	//


	std::ostream&	DeclarationType::toXML( std::ostream&			outputStream,
									 	    int						indentLevel,
									 	    SerializationOptions	options ) const
	{
		const std::string&		currentIndent = XMLIndentTable::GetIndent( indentLevel );


		outputStream << currentIndent << "<declaration-type>" << "</declaration-type>\n";

		return( outputStream );
	}

	//
	//	Function
	//

	std::ostream&	FunctionParameter::toXML( std::ostream&				outputStream,
					   	   	   	   	   	   	  int						indentLevel,
					   	   	   	   	   	   	  SerializationOptions		options ) const
	{
		std::string		currentIndent = XMLIndentTable::GetIndent( indentLevel );

		outputStream << currentIndent + "<parameter>\n";
		NamedEntity::toXML( outputStream, indentLevel + 1, options );
		type().toXML( outputStream, indentLevel + 1, AddOption( options, SerializationOptions::NO_ATTRIBUTES ));
		CompilerSpecific::toXML( outputStream, indentLevel + 1, options );
		outputStream << currentIndent << "</parameter>\n";

		return( outputStream );
	}


	std::ostream&	FunctionParameters::toXML( std::ostream&			outputStream,
						   	   	       	   	   int						indentLevel,
						   	   	       	   	   SerializationOptions		options ) const
	{
		const std::string&		currentIndent = XMLIndentTable::GetIndent( indentLevel );


		if( !m_parameters->empty() )
		{
			outputStream << currentIndent << "<parameters>\n";

			for( const FunctionParameter& itrParameter : *m_parameters )
			{
				itrParameter.toXML( outputStream, indentLevel + 1, options );
			}

			outputStream << currentIndent << "</parameters>\n";
		}

		return( outputStream );
	}


	std::ostream&	FunctionDefinition::toXML( std::ostream&			outputStream,
					   	   	   	   	   	   	   int						indentLevel,
					   	   	   	   	   	   	   SerializationOptions		options ) const
	{
		std::string		currentIndent = XMLIndentTable::GetIndent( indentLevel );
		std::string		currentIndentPlusOne = XMLIndentTable::GetIndent( indentLevel + 1 );


		if( !isHiddenFriend() )
		{
			outputStream << currentIndent << "<function>\n";
		}
		else
		{
			outputStream << currentIndent << "<function type=\"hiddenFriend\">\n";
		}

		SourceElement::toXML( outputStream, indentLevel + 1, options );
		NamespaceScoped::toXML( outputStream, indentLevel + 1, options );
		outputStream << currentIndentPlusOne << "<returnType>\n";
		returnType().toXML( outputStream, indentLevel + 2, AddOption( options, SerializationOptions::NO_ATTRIBUTES ));
		outputStream << currentIndentPlusOne << "</returnType>\n";
		attributes().toXML( outputStream, indentLevel + 1, options );

		m_parameters.toXML( outputStream, indentLevel + 1, options );

		outputStream << currentIndent << "</function>\n";

		return( outputStream );
	}

	//
	//	Global Variables
	//


	std::ostream&	GlobalVarDeclaration::toXML( std::ostream&				outputStream,
												 int						indentLevel,
												 SerializationOptions		options ) const
	{
		std::string		currentIndent = XMLIndentTable::GetIndent( indentLevel );
		std::string		currentIndentPlusOne = XMLIndentTable::GetIndent( indentLevel + 1 );


		outputStream << currentIndent << "<global_var_decl>\n";

		NamedEntity::toXML( outputStream, indentLevel + 1, options );
		NamespaceScoped::toXML( outputStream, indentLevel + 1, options );
		Static::toXML( outputStream, indentLevel + 1, options );
	//		type().toXML( outputStream, indentLevel + 1, AddOption( options, SerializationOptions::NO_ATTRIBUTES ));
		attributes().toXML( outputStream, indentLevel + 1, options );

		outputStream << currentIndent << "</global_var_decl>\n";

		return( outputStream );
	}


	std::ostream&	GlobalVarEntry::toXML( std::ostream&			outputStream,
					   	   	   	   	   	   int						indentLevel,
					   	   	   	   	   	   SerializationOptions		options ) const
	{
		std::string		currentIndent = XMLIndentTable::GetIndent( indentLevel );
		std::string		currentIndentPlusOne = XMLIndentTable::GetIndent( indentLevel + 1 );


		outputStream << currentIndent << "<global_var>\n";

		NamedEntity::toXML( outputStream, indentLevel + 1, options );
		NamespaceScoped::toXML( outputStream, indentLevel + 1, options );
		ASTEntry::toXML( outputStream, indentLevel + 1, options );
		Static::toXML( outputStream, indentLevel + 1, options );
		type().toXML( outputStream, indentLevel + 1, AddOption( options, SerializationOptions::NO_ATTRIBUTES ));
		attributes().toXML( outputStream, indentLevel + 1, options );

		outputStream << currentIndent << "</global_var>\n";

		return( outputStream );
	}


	//
	//	Named Entity
	//

	std::ostream&	NamedEntity::toXML( std::ostream&			outputStream,
						   	   	   	    int						indentLevel,
						   	   	   	    SerializationOptions	options ) const
	{
		outputStream << XMLIndentTable::GetIndent( indentLevel ) << "<name>" << m_name << "</name>\n";

		return( outputStream );
	}


	//
	//	Namespace
	//

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


	//
	//	SourceElement
	//


	std::ostream&	SourceElement::toXML( std::ostream&				outputStream,
										  int						indentLevel,
										  SerializationOptions		options ) const
	{
		NamedEntity::toXML( outputStream, indentLevel, options );
		UID::toXML( outputStream, indentLevel, options );

		SourceLocation::toXML( outputStream, indentLevel, options );

		return( outputStream );
	}


	//
	//	SourceLocation
	//


	std::ostream&	SourceLocation::toXML( std::ostream&			outputStream,
							  	  	  	   int						indentLevel,
							  	  	  	   SerializationOptions		options ) const

	{
		const std::string&		currentIndent = XMLIndentTable::GetIndent( indentLevel );
		const std::string&		currentIndentPlusOne = XMLIndentTable::GetIndent( indentLevel + 1 );

		outputStream << currentIndent << "<source-info>\n";

		if( !m_fileName.empty() )
		{
			outputStream << currentIndentPlusOne << "<file>" << m_fileName << "</file>\n";
			outputStream << currentIndentPlusOne << "<line>" << (boost::lexical_cast<std::string>(m_lineNumber)) << "</line>\n";
			outputStream << currentIndentPlusOne << "<char-count>" << (boost::lexical_cast<std::string>(m_characterCount)) << "</char-count>\n";
			outputStream << currentIndentPlusOne << "<location>" << (boost::lexical_cast<std::string>(m_location)) << "</location>\n";
		}
		else
		{
			outputStream << currentIndentPlusOne << "<unknown>true</unknown>\n";
		}

		outputStream << currentIndent << "</source-info>\n";

		return( outputStream );
	}


	//
	//	Static
	//


	std::ostream&	Static::toXML( std::ostream&			outputStream,
								   int						indentLevel,
								   SerializationOptions		options ) const
	{
		outputStream << XMLIndentTable::GetIndent( indentLevel ) << "<static>" << ( m_isStatic ? "true" : "false" ) << "</static>\n";

		return( outputStream );
	}



	//
	//	Template
	//

	std::ostream&	TemplateParameter::toXML( std::ostream&				outputStream,
											  int						indentLevel,
											  SerializationOptions		options ) const
	{
		std::string		currentIndent = XMLIndentTable::GetIndent( indentLevel );

		outputStream << currentIndent + "<parameter>\n";
		NamedEntity::toXML( outputStream, indentLevel + 1, options );
		CompilerSpecific::toXML( outputStream, indentLevel + 1, options );
		outputStream << currentIndent << "</parameter>\n";

		return( outputStream );
	}


	std::ostream&	TemplateParameters::toXML( std::ostream&			outputStream,
											   int						indentLevel,
											   SerializationOptions		options ) const
	{
		const std::string&		currentIndent = XMLIndentTable::GetIndent( indentLevel );


		if( !m_parameters->empty() )
		{
			outputStream << currentIndent << "<parameters>\n";

			for( const TemplateParameter& itrParameter : *m_parameters )
			{
				itrParameter.toXML( outputStream, indentLevel + 1, options );
			}

			outputStream << currentIndent << "</parameters>\n";
		}

		return( outputStream );
	}


	std::ostream&	FunctionTemplateDefinition::toXML( std::ostream&			outputStream,
					   	   	   	   	   	   	   	   	   int						indentLevel,
					   	   	   	   	   	   	   	   	   SerializationOptions		options ) const
	{
		std::string		currentIndent = XMLIndentTable::GetIndent( indentLevel );
		std::string		currentIndentPlusOne = XMLIndentTable::GetIndent( indentLevel + 1 );


		if( !isHiddenFriend() )
		{
			outputStream << currentIndent << "<function>\n";
		}
		else
		{
			outputStream << currentIndent << "<function type=\"hiddenFriend\">\n";
		}

		SourceElement::toXML( outputStream, indentLevel + 1, options );
		NamespaceScoped::toXML( outputStream, indentLevel + 1, options );
		outputStream << currentIndentPlusOne << "<returnType>\n";
		outputStream << currentIndentPlusOne << "</returnType>\n";
		attributes().toXML( outputStream, indentLevel + 1, options );

		m_parameters.toXML( outputStream, indentLevel + 1, options );

		outputStream << currentIndent << "</function>\n";

		return( outputStream );
	}


	//
	//	Types
	//


	std::ostream&	UnrecognizedType::toXML( std::ostream&			outputStream,
											 int					indentLevel,
											 SerializationOptions	options ) const
	{
		const std::string&		currentIndent = XMLIndentTable::GetIndent( indentLevel );
		const std::string&		currentIndentPlusOne = XMLIndentTable::GetIndent( indentLevel + 1 );

		outputStream << currentIndent << "<type>\n";
		outputStream << currentIndentPlusOne << "<kind>unrecognized</kind>\n";
		outputStream << currentIndent << "</type>\n";

		attributes().toXML( outputStream, indentLevel, options );

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
		outputStream << currentIndentPlusOne << "<declaration>" << CPPTypes[(int)typeSpec()].label.c_str() << "</declaration>\n";
		outputStream << currentIndent << "</type>\n";

		attributes().toXML( outputStream, indentLevel, options );

		return( outputStream );
	}


	std::ostream&	ClassOrStructType::toXML( std::ostream&				outputStream,
											  int						indentLevel,
											  SerializationOptions	 	options ) const
	{
		const std::string&		currentIndent = XMLIndentTable::GetIndent( indentLevel );
		const std::string&		currentIndentPlusOne = XMLIndentTable::GetIndent( indentLevel + 1 );

		outputStream << currentIndent << "<type>\n";
		outputStream << currentIndentPlusOne << "<kind>class-or-struct</kind>\n";
		outputStream << currentIndentPlusOne << "<declaration>" << fqName().c_str() << "</declaration>\n";
		NamespaceScoped::toXML( outputStream, indentLevel + 1, options );
		outputStream << currentIndent << "</type>\n";

		attributes().toXML( outputStream, indentLevel, options );

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
			outputStream << currentIndentPlusTwo << "<operator>" << CPPTypes[(int)currentBaseType->typeSpec()].label << "</operator>\n";
			currentBaseType = (Type*)(&(((DerivedType*)currentBaseType)->baseType()));
		}

		((Type*)currentBaseType)->toXML( outputStream, indentLevel + 2, options );
		outputStream << currentIndentPlusOne << "</declaration>\n";

		outputStream << currentIndent << "</type>\n";

		attributes().toXML( outputStream, indentLevel, options );


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
		NamespaceScoped::toXML( outputStream, indentLevel, options );
		outputStream << currentIndent << "</type>\n";

		return( outputStream );
	}


	//
	//	UID
	//


	std::ostream&		UID::toXML( std::ostream&			outputStream,
						   	   	    int						indentLevel,
						   	   	    SerializationOptions	options ) const
	{
		outputStream << XMLIndentTable::GetIndent( indentLevel ) << "<uid>" << m_uid << "</uid>\n";

		return( outputStream );
	}



	//
	//	Union
	//


	std::ostream&	UnionDefinition::toXML( std::ostream&			outputStream,
					   	   	   	   	  	    int						indentLevel,
					   	   	   	   	  	    SerializationOptions	options ) const
	{
		const std::string&		currentIndent = XMLIndentTable::GetIndent( indentLevel );
		const std::string&		currentIndentPlusOne = XMLIndentTable::GetIndent( indentLevel + 1 );

		outputStream << currentIndent << "<union>\n";

		SourceElement::toXML( outputStream, indentLevel + 1, options );
		NamespaceScoped::toXML( outputStream, indentLevel + 1, options );
		attributes().toXML( outputStream, indentLevel + 1, options );

		outputStream << currentIndentPlusOne << "<members>\n";
		for( const UnionMember& itrMember : members() )
		{
			itrMember.toXML( outputStream, indentLevel + 2, options );
		}
		outputStream << currentIndentPlusOne << "</members>\n";

		outputStream << currentIndent << "</union>\n";;

		return( outputStream );
	}


	std::ostream&	UnionMember::toXML( std::ostream&			outputStream,
					   	   	   	   	  	int						indentLevel,
					   	   	   	   	  	SerializationOptions	options ) const
	{
		const std::string&		currentIndent = XMLIndentTable::GetIndent( indentLevel );

		outputStream << currentIndent << "<member>\n";
		NamedEntity::toXML( outputStream, indentLevel + 1, options );
		SourceLocation::toXML( outputStream, indentLevel + 1, options );
		type().toXML( outputStream, indentLevel + 1, options );
		outputStream << currentIndent << "</member>\n";

		return( outputStream );
	}

}
