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

#include "Utility/IndentingOutputStreambuf.h"




namespace CPPModel
{


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
									   SerializationOptions		options ) const
	{
		outputStream << "<access>" << toString( m_accessSpec ) << "</access>\n";

		return( outputStream );
	}


	//
	//	ASTDictionary
	//


	std::ostream&	DictionaryEntry::toXML( std::ostream&			outputStream,
											SerializationOptions	options ) const
	{
		outputStream << "<dictionary_entry>\n";

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			enclosingNamespace().toXML( outputStream, options );

			outputStream << "<name>" << name() << "</name>\n";
			outputStream << "<uid>" << (boost::lexical_cast<std::string>(uid().uidValue())) << "</uid>\n";

			sourceLocation().toXML( outputStream, options );

			attributes().toXML( outputStream, options );

			//	Be careful below, classes cannot be static and are fixed false in the constructor for their
			//		dictionary entry.  If you punch out the 'static' flag all the time then it will
			//		always appear as false for classes (beyond just not making sense for classes anyway).

			if( isStatic() )
			{
				outputStream << "<static>true</static>\n";
			}
		}

		outputStream << "</dictionary_entry>\n";

		return( outputStream );
	}



	//
	//	Attributes
	//


	std::ostream&	Attribute::toXML( std::ostream&				outputStream,
						   	   	      SerializationOptions		options ) const
	{
		outputStream << "<attribute>\n";

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			NamedEntity::toXML( outputStream, options );

			if( !m_arguments->empty() )
			{
				outputStream << "<arguments>\n";

				{
					SEFUtility::IndentingOutputStreambuf		indent( outputStream );

					for( const ConstantValue& itrArgument : *m_arguments )
					{
						itrArgument.toXML( outputStream, options );
					}
				}

				outputStream << "</arguments>\n";
			}
		}

		outputStream << "</attribute>\n";

		return( outputStream );
	}



	std::ostream&	Attributes::toXML( std::ostream&			outputStream,
						   	   	       SerializationOptions		options ) const
	{
		//	If we have been passed the NO_ATTRIBUTES option or there are now attributes, then return now

		if( MatchOptions( options, SerializationOptions::NO_ATTRIBUTES ) || m_attributes->empty() )
		{
			return( outputStream );
		}


		outputStream << "<attributes>\n";

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			for( const Attribute& itrAttribute : *m_attributes )
			{
				itrAttribute.toXML( outputStream, options );
			}
		}

		outputStream << "</attributes>\n";

		return( outputStream );
	}


	//
	//	Class
	//


	std::ostream&	BaseClassIdentifier::toXML( std::ostream&			outputStream,
					   	   	   	   	   	   	    SerializationOptions	options ) const
	{
		outputStream << "<base-class>\n";

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			NamedEntity::toXML( outputStream, options );

			outputStream << "<virtual>" << (boost::lexical_cast<std::string>( isVirtual() )) << "</virtual>\n";
			outputStream << "<access>" << std::string( toString( accessSpecifier() )) << "</access>\n";
		}

		outputStream << "</base-class>\n";

		return( outputStream );
	}


	std::ostream&	FriendClassIdentifier::toXML( std::ostream&				outputStream,
					   	   	   	   	   	   	 	  SerializationOptions		options ) const
	{
		outputStream << "<friend>\n";

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			outputStream << "<type>CLASS</type>\n";
			m_type->toXML( outputStream, AddOption( options, SerializationOptions::NO_ATTRIBUTES ) );
		}

		outputStream << "</friend>\n";

		return( outputStream );
	}


	std::ostream&	FriendClassMemberIdentifier::toXML( std::ostream&			outputStream,
					   	   	   	   	   	   	 	  	    SerializationOptions	options ) const
	{
		outputStream << "<friend>\n";

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			outputStream << "<type>CLASS_MEMBER</type>\n";
			m_type->toXML( outputStream, AddOption( options, SerializationOptions::NO_ATTRIBUTES ) );
		}

		outputStream << "</friend>\n";

		return( outputStream );
	}


	std::ostream&	FriendFunctionIdentifier::toXML( std::ostream&				outputStream,
					   	   	   	   	   	   	 	     SerializationOptions		options ) const
	{
		outputStream << "<friend>\n";

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			outputStream << "<type>FUNCTION</type>\n";
		}

		outputStream << "</friend>\n";

		return( outputStream );
	}



	std::ostream&	FieldDeclaration::toXML( std::ostream&				outputStream,
					   	   	   	   	   	   	 SerializationOptions		options ) const
	{
		outputStream << "<field>\n";

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			NamedEntity::toXML( outputStream, options );
			SourceLocation::toXML( outputStream, options );
			type().toXML( outputStream, AddOption( options, SerializationOptions::NO_ATTRIBUTES ) );
			Access::toXML( outputStream, options );
			Static::toXML( outputStream, options );
			attributes().toXML( outputStream, options );
		}

		outputStream << "</field>\n";

		return( outputStream );
	}


	std::ostream&	MethodDeclaration::toXML( std::ostream&				outputStream,
					   	   	   	   	   	   	  SerializationOptions		options ) const
	{
		outputStream << "<method>\n";

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			SourceElement::toXML( outputStream, options );
			Access::toXML( outputStream, options );
			Static::toXML( outputStream, options );

			outputStream << "<result>\n";
			{
				SEFUtility::IndentingOutputStreambuf		indent( outputStream );

				resultType().toXML( outputStream, AddOption( options, SerializationOptions::NO_ATTRIBUTES ) );
			}
			outputStream << "</result>\n";

			outputStream << "<parameters>\n";
			{
				SEFUtility::IndentingOutputStreambuf		indent( outputStream );

				for( boost::ptr_list<FunctionParameter>::const_iterator itrParam = m_parameterList->begin(); itrParam != m_parameterList->end(); itrParam++ )
				{
					itrParam->toXML( outputStream, options );
				}
			}
			outputStream << "</parameters>\n";

			attributes().toXML( outputStream, options );
		}

		outputStream << "</method>\n";

		return( outputStream );
	}


	std::ostream&	TemplateMethodDeclaration::toXML( std::ostream&				outputStream,
					   	   	   	   	   	   	  	  	  SerializationOptions		options ) const
	{
		outputStream << "<template_method>\n";

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			SourceElement::toXML( outputStream, options );
			Access::toXML( outputStream, options );
			Static::toXML( outputStream, options );

			outputStream << "<result>\n";
			{
				SEFUtility::IndentingOutputStreambuf		indent( outputStream );

				resultType().toXML( outputStream, AddOption( options, SerializationOptions::NO_ATTRIBUTES ) );
			}
			outputStream << "</result>\n";

			outputStream << "<template_parameters>\n";
			{
				SEFUtility::IndentingOutputStreambuf		indent( outputStream );

				for( boost::ptr_list<TemplateParameter>::const_iterator itrParam = m_parameterList->begin(); itrParam != m_parameterList->end(); itrParam++ )
				{
					itrParam->toXML( outputStream, options );
				}
			}
			outputStream << "</template_parameters>\n";

			attributes().toXML( outputStream, options );
		}

		outputStream << "</template_method>\n";

		return( outputStream );
	}


	std::ostream&	ClassDefinition::toXML( std::ostream&			outputStream,
					   	   	   	   	   	   	SerializationOptions	options ) const
	{
		if( MatchOptions( SerializationMask(), options ) )
		{
			return( outputStream );
		}

		if( !isStruct() )
		{
			outputStream << "<class type=\"class\">\n";
		}
		else
		{
			outputStream << "<class type=\"struct\">\n";
		}

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			SourceElement::toXML( outputStream, options );
			NamespaceScoped::toXML( outputStream, options );
			CompilerSpecific::toXML( outputStream, options );

			attributes().toXML( outputStream, options );

			outputStream << "<base-classes>\n";
			if( !baseClasses().empty() )
			{
				SEFUtility::IndentingOutputStreambuf		indent( outputStream );

				for( boost::ptr_list<BaseClassIdentifier>::const_iterator itr = baseClasses().begin(); itr != baseClasses().end(); itr++ )
				{
					itr->toXML( outputStream, options );
				}
			}
			outputStream << "</base-classes>\n";

			outputStream << "<friends>\n";
			if( !friends().empty() )
			{
				SEFUtility::IndentingOutputStreambuf		indent( outputStream );

				for( boost::ptr_list<const FriendIdentifier>::const_iterator itr = friends().begin(); itr != friends().end(); itr++ )
				{
					itr->toXML( outputStream, options );
				}
			}
			outputStream << "</friends>\n";

			outputStream << "<fields>\n";
			if( !fields().empty() )
			{
				SEFUtility::IndentingOutputStreambuf		indent( outputStream );

				for( boost::ptr_list<const FieldDeclaration>::const_iterator itr = fields().begin(); itr != fields().end(); itr++ )
				{
					itr->toXML( outputStream, options );
				}
			}
			outputStream << "</fields>\n";

			outputStream << "<methods>\n";
			if( !methods().empty() )
			{
				SEFUtility::IndentingOutputStreambuf		indent( outputStream );

				for( boost::ptr_list<const MethodDeclaration>::const_iterator itr = methods().begin(); itr != methods().end(); itr++ )
				{
					itr->toXML( outputStream, options );
				}
			}
			outputStream << "</methods>\n";

			outputStream << "<template_methods>\n";
			if( !templateMethods().empty() )
			{
				SEFUtility::IndentingOutputStreambuf		indent( outputStream );

				for( boost::ptr_list<const TemplateMethodDeclaration>::const_iterator itr = templateMethods().begin(); itr != templateMethods().end(); itr++ )
				{
					itr->toXML( outputStream, options );
				}
			}
			outputStream << "</template_methods>\n";
		}

		outputStream << "</class>\n";

		return( outputStream );
	}

	//
	//	Compiler Specific
	//


	std::ostream&	CompilerSpecific::toXML( std::ostream&				outputStream,
											 SerializationOptions		options ) const
	{
		if( !m_builtIn && !m_artificial )
		{
			return( outputStream );
		}

		outputStream << "<compiler_specific>\n";

		if( m_builtIn )
		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );
			outputStream << "</built_in>\n";
		}

		if( m_artificial )
		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );
			outputStream << "</artificial>\n";
		}

		outputStream << "</compiler_specific>\n";

		return( outputStream );
	}

	//
	//	Constant Values
	//


	std::ostream&	UnrecognizedConstant::toXML( std::ostream&			outputStream,
					   	   	   	   	   	   	   	 SerializationOptions	options ) const
	{
		outputStream << "</constant type=\"UNRECOGNIZED\">\n";

		return( outputStream );
	}


	std::ostream&	StringConstant::toXML( std::ostream&			outputStream,
					   	   	   	   	   	   SerializationOptions		options ) const
	{
		outputStream << "<constant type=\"STRING\">" << m_value << "</constant>\n";

		return( outputStream );
	}


	std::ostream&	IntegerConstant::toXML( std::ostream&			outputStream,
					   	   	   	   	   	    SerializationOptions	options ) const
	{
		outputStream << "<constant type=\"INTEGER\">" << (boost::lexical_cast<std::string>( m_value )) << "</constant>\n";

		return( outputStream );
	}

	std::ostream&	RealConstant::toXML( std::ostream&				outputStream,
					   	   	   	   	   	 SerializationOptions		options ) const
	{
		outputStream << "<constant type=\"REAL\">" << (boost::lexical_cast<std::string>( m_value )) << "</constant>\n";

		return( outputStream );
	}

	//
	//	DeclarationType
	//


	std::ostream&	DeclarationType::toXML( std::ostream&			outputStream,
									 	    SerializationOptions	options ) const
	{
		outputStream << "<declaration-type>" << "</declaration-type>\n";

		return( outputStream );
	}

	//
	//	Function
	//

	std::ostream&	FunctionParameter::toXML( std::ostream&				outputStream,
					   	   	   	   	   	   	  SerializationOptions		options ) const
	{
		outputStream << "<parameter>\n";

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );
			NamedEntity::toXML( outputStream, options );
			type().toXML( outputStream, AddOption( options, SerializationOptions::NO_ATTRIBUTES ));
			CompilerSpecific::toXML( outputStream, options );
		}

		outputStream << "</parameter>\n";

		return( outputStream );
	}


	std::ostream&	FunctionParameters::toXML( std::ostream&			outputStream,
						   	   	       	   	   SerializationOptions		options ) const
	{
		if( !m_parameters->empty() )
		{
			outputStream << "<parameters>\n";

			{
				SEFUtility::IndentingOutputStreambuf		indent( outputStream );

				for( const FunctionParameter& itrParameter : *m_parameters )
				{
					itrParameter.toXML( outputStream, options );
				}
			}

			outputStream << "</parameters>\n";
		}

		return( outputStream );
	}


	std::ostream&	FunctionDefinition::toXML( std::ostream&			outputStream,
					   	   	   	   	   	   	   SerializationOptions		options ) const
	{
		if( !isHiddenFriend() )
		{
			outputStream << "<function>\n";
		}
		else
		{
			outputStream << "<function type=\"hiddenFriend\">\n";
		}

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			NamespaceScoped::toXML( outputStream, options );
			SourceElement::toXML( outputStream, options );
			attributes().toXML( outputStream, options );
			outputStream << "<returnType>\n";

			{
				SEFUtility::IndentingOutputStreambuf		indent( outputStream );

				returnType().toXML( outputStream, AddOption( options, SerializationOptions::NO_ATTRIBUTES ));
			}

			outputStream << "</returnType>\n";

			m_parameters.toXML( outputStream, options );
		}

		outputStream << "</function>\n";

		return( outputStream );
	}

	//
	//	Global Variables
	//


	std::ostream&	GlobalVarDeclaration::toXML( std::ostream&				outputStream,
												 SerializationOptions		options ) const
	{
		outputStream << "<global_var_decl>\n";

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			NamedEntity::toXML( outputStream, options );
			NamespaceScoped::toXML( outputStream, options );
		//		type().toXML( outputStream, AddOption( options, SerializationOptions::NO_ATTRIBUTES ));
			attributes().toXML( outputStream, options );
			Static::toXML( outputStream, options );
		}

		outputStream << "</global_var_decl>\n";

		return( outputStream );
	}


	std::ostream&	GlobalVarEntry::toXML( std::ostream&			outputStream,
										   SerializationOptions		options ) const
	{
//		std::string		currentIndent = XMLIndentTable::GetIndent( indentLevel );
//		std::string		currentIndentPlusOne = XMLIndentTable::GetIndent( indentLevel + 1 );


		outputStream << "<global_var_entry>\n";

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			NamespaceScoped::toXML( outputStream, options );
			SourceElement::toXML( outputStream, options );
			attributes().toXML( outputStream, options );
			Static::toXML( outputStream, options );
			type().toXML( outputStream, AddOption( options, SerializationOptions::NO_ATTRIBUTES ));
		}

		outputStream << "</global_var_entry>\n";

		return( outputStream );
	}



	//
	//	Named Entity
	//

	std::ostream&	NamedEntity::toXML( std::ostream&			outputStream,
						   	   	   	    SerializationOptions	options ) const
	{
		outputStream << "<name>" << m_name << "</name>\n";

		return( outputStream );
	}


	//
	//	Namespace
	//

	std::ostream&	Namespace::toXML( std::ostream&				outputStream,
									  SerializationOptions		options ) const
	{
		if( MatchOptions( SerializationMask(), options ) )
		{
			return( outputStream );
		}

		outputStream << "<namespace>\n";

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			outputStream << "<name>" << fqName() << "</name>\n";
			CompilerSpecific::toXML( outputStream, options );
		}

		outputStream << "</namespace>\n";

		return( outputStream );
	}


	//
	//	SourceElement
	//


	std::ostream&	SourceElement::toXML( std::ostream&				outputStream,
										  SerializationOptions		options ) const
	{
		NamedEntity::toXML( outputStream, options );
		UID::toXML( outputStream, options );

		SourceLocation::toXML( outputStream, options );

		return( outputStream );
	}


	//
	//	SourceLocation
	//


	std::ostream&	SourceLocation::toXML( std::ostream&			outputStream,
							  	  	  	   SerializationOptions		options ) const

	{
		outputStream << "<source-info>\n";

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			if( !m_fileName.empty() )
			{
				outputStream << "<file>" << m_fileName << "</file>\n";
				outputStream << "<line>" << (boost::lexical_cast<std::string>(m_lineNumber)) << "</line>\n";
				outputStream << "<char-count>" << (boost::lexical_cast<std::string>(m_characterCount)) << "</char-count>\n";
				outputStream << "<location>" << (boost::lexical_cast<std::string>(m_location)) << "</location>\n";
			}
			else
			{
				outputStream << "<unknown>true</unknown>\n";
			}
		}

		outputStream << "</source-info>\n";

		return( outputStream );
	}


	//
	//	Static
	//


	std::ostream&	Static::toXML( std::ostream&			outputStream,
								   SerializationOptions		options ) const
	{
		outputStream << "<static>" << ( m_isStatic ? "true" : "false" ) << "</static>\n";

		return( outputStream );
	}



	//
	//	Template
	//

	std::ostream&	TemplateParameter::toXML( std::ostream&				outputStream,
											  SerializationOptions		options ) const
	{
		outputStream << "<parameter>\n";

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			NamedEntity::toXML( outputStream, options );
			CompilerSpecific::toXML( outputStream, options );
		}

		outputStream << "</parameter>\n";

		return( outputStream );
	}


	std::ostream&	TemplateParameters::toXML( std::ostream&			outputStream,
											   SerializationOptions		options ) const
	{
		if( !m_parameters->empty() )
		{
			outputStream << "<parameters>\n";

			{
				SEFUtility::IndentingOutputStreambuf		indent( outputStream );

				for( const TemplateParameter& itrParameter : *m_parameters )
				{
					itrParameter.toXML( outputStream, options );
				}
			}

			outputStream << "</parameters>\n";
		}

		return( outputStream );
	}


	std::ostream&	FunctionTemplateDefinition::toXML( std::ostream&			outputStream,
					   	   	   	   	   	   	   	   	   SerializationOptions		options ) const
	{
		if( !isHiddenFriend() )
		{
			outputStream << "<function>\n";
		}
		else
		{
			outputStream << "<function type=\"hiddenFriend\">\n";
		}

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			SourceElement::toXML( outputStream, options );
			NamespaceScoped::toXML( outputStream, options );
			outputStream << "<returnType>\n";
			outputStream << "</returnType>\n";
			attributes().toXML( outputStream, options );

			m_parameters.toXML( outputStream, options );
		}

		outputStream << "</function>\n";

		return( outputStream );
	}


	//
	//	Types
	//


	std::ostream&	UnrecognizedType::toXML( std::ostream&			outputStream,
											 SerializationOptions	options ) const
	{
		outputStream << "<type>\n";

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			outputStream << "<kind>unrecognized</kind>\n";
		}

		outputStream << "</type>\n";

		attributes().toXML( outputStream, options );

		return( outputStream );
	}


	std::ostream&	FundamentalType::toXML( std::ostream&			outputStream,
											SerializationOptions	options ) const
	{
		outputStream << "<type>\n";

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			outputStream << "<kind>fundamental</kind>\n";
			outputStream << "<declaration>" << CPPTypes[(int)typeSpec()].label.c_str() << "</declaration>\n";
		}

		outputStream << "</type>\n";

		attributes().toXML( outputStream, options );

		return( outputStream );
	}


	std::ostream&	ClassOrStructType::toXML( std::ostream&				outputStream,
											  SerializationOptions	 	options ) const
	{
		outputStream << "<type>\n";

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			outputStream << "<kind>class-or-struct</kind>\n";
			outputStream << "<declaration>" << fqName().c_str() << "</declaration>\n";
			NamespaceScoped::toXML( outputStream, options );
		}

		outputStream << "</type>\n";

		attributes().toXML( outputStream, options );

		return( outputStream );
	}


	std::ostream&	DerivedType::toXML( std::ostream&			outputStream,
										SerializationOptions	options ) const
	{
		outputStream << "<type>\n";

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			outputStream << "<kind>derived</kind>\n";
			outputStream << "<declaration>\n";

			{
				SEFUtility::IndentingOutputStreambuf		indent( outputStream );

				Type*		currentBaseType = (Type*)this;
				while( currentBaseType->kind() == Type::Kind::DERIVED )
				{
					outputStream << "<operator>" << CPPTypes[(int)currentBaseType->typeSpec()].label << "</operator>\n";
					currentBaseType = (Type*)(&(((DerivedType*)currentBaseType)->baseType()));
				}

				((Type*)currentBaseType)->toXML( outputStream, options );
			}

			outputStream << "</declaration>\n";
		}

		outputStream << "</type>\n";

		attributes().toXML( outputStream, options );


		return( outputStream );
	}


	std::ostream&	UnionType::toXML( std::ostream&				outputStream,
									  SerializationOptions		options ) const
	{
		outputStream << "<type>\n";
		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			outputStream << "<kind>union</kind>\n";
			outputStream << "<declaration>" << name().c_str() << "</declaration>\n";
		}

		NamespaceScoped::toXML( outputStream, options );
		outputStream << "</type>\n";

		return( outputStream );
	}


	//
	//	UID
	//


	std::ostream&		UID::toXML( std::ostream&			outputStream,
						   	   	    SerializationOptions	options ) const
	{
		outputStream << "<uid>" << m_uid << "</uid>\n";

		return( outputStream );
	}



	//
	//	Union
	//


	std::ostream&	UnionDefinition::toXML( std::ostream&			outputStream,
					   	   	   	   	  	    SerializationOptions	options ) const
	{
		outputStream << "<union>\n";

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			SourceElement::toXML( outputStream, options );
			NamespaceScoped::toXML( outputStream, options );
			attributes().toXML( outputStream, options );

			outputStream << "<members>\n";
			for( const UnionMember& itrMember : members() )
			{
				SEFUtility::IndentingOutputStreambuf		indent( outputStream );
				itrMember.toXML( outputStream, options );
			}
			outputStream << "</members>\n";
		}

		outputStream << "</union>\n";;

		return( outputStream );
	}


	std::ostream&	UnionMember::toXML( std::ostream&			outputStream,
					   	   	   	   	  	SerializationOptions	options ) const
	{
		outputStream << "<member>\n";

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			NamedEntity::toXML( outputStream, options );
			SourceLocation::toXML( outputStream, options );
			type().toXML( outputStream, options );
		}

		outputStream << "</member>\n";

		return( outputStream );
	}

}
