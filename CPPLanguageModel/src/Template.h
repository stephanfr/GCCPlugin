/*
 * Template.h
 *
 *  Created on: Aug 11, 2013
 *      Author: steve
 */

#ifndef TEMPLATE_H_
#define TEMPLATE_H_


#include "ListAliases.h"

#include "CompilerSpecific.h"
#include "NamedEntity.h"
#include "UID.h"
#include "SourceLocation.h"
#include "Namespace.h"



namespace CPPModel
{

	class TemplateParameter : public NamedEntity, public CompilerSpecific
	{
	public :

		TemplateParameter() = delete;
		TemplateParameter( TemplateParameter& ) = delete;
		TemplateParameter( const TemplateParameter& ) = delete;

		TemplateParameter( const std::string				name,
						   CompilerSpecific					compilerSpecific )
			: NamedEntity( name ),
			  CompilerSpecific( compilerSpecific )
		{}

		~TemplateParameter()
		{}


		std::ostream&	toXML( std::ostream&			outputStream,
							   int						indentLevel,
							   SerializationOptions		options ) const;
	};


	class TemplateParameters : public IXMLSerializable
	{
	public :

		TemplateParameters() = delete;
		TemplateParameters( TemplateParameters& ) = delete;
		TemplateParameters( const TemplateParameters& ) = delete;

		TemplateParameters( ConstListPtr<TemplateParameter>&		parameters )
			: m_parameters( std::move( parameters ))
		{}

		TemplateParameters( ListPtr<TemplateParameter>&				parameters )
			: m_parameters( std::move( parameters ))
		{}



		const ListRef<TemplateParameter>			parameters() const
		{
			return( *(m_parameters.get()) );
		}


		std::ostream&								toXML( std::ostream&			outputStream,
							   	   	   	 	 	 	 	   int						indentLevel,
							   	   	   	 	 	 	 	   SerializationOptions		options ) const;

	private :

		const ConstListPtr<TemplateParameter>		m_parameters;
	};


	class FunctionTemplateDefinition : public SourceElement, public NamespaceScoped, public IAttributes
	{
	public :

		FunctionTemplateDefinition() = delete;
		FunctionTemplateDefinition( FunctionTemplateDefinition& ) = delete;
		FunctionTemplateDefinition( const FunctionTemplateDefinition& ) = delete;

		FunctionTemplateDefinition( const std::string&					name,
						 			const UID&							uid,
						 			const Namespace&					namespaceScope,
						 			const SourceLocation&				sourceLocation,
						 			const bool							hiddenFriend,
						 			ConstListPtr<Attribute>				attributes,
						 			ConstListPtr<TemplateParameter>		parameters )
			: SourceElement( name, uid, sourceLocation ),
			  NamespaceScoped( namespaceScope ),
			  m_attributes( attributes ),
			  m_hiddenFriend( hiddenFriend ),
			  m_parameters( parameters )
		{}


		const bool				isHiddenFriend() const
		{
			return( m_hiddenFriend );
		}

		const Attributes&		attributes() const
		{
			return( m_attributes );
		}


		std::ostream&	toXML( std::ostream&			outputStream,
							   int						indentLevel,
							   SerializationOptions		options ) const;

	private :

		const Attributes						m_attributes;

		const bool								m_hiddenFriend;

		const TemplateParameters				m_parameters;
	};



} /* namespace CPPModel */

#endif /* TEMPLATE_H_ */
