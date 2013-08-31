/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/


#ifndef NAMESPACE_H_
#define NAMESPACE_H_


#include "ListAliases.h"

#include "CompilerSpecific.h"
#include "UID.h"
#include "SourceElement.h"
#include "Attribute.h"



namespace CPPModel
{
	const std::string		SCOPE_RESOLUTION_OPERATOR = "::";
	const std::string		STD_NAMESPACE_LABEL = "std::";



	class Namespace : public SourceElement, public CompilerSpecific, public IAttributes
	{
	public :

//		Namespace() = delete;

		virtual ~Namespace()
		{};

		virtual bool					isGlobal() const = 0;

		virtual const Namespace&		parentNamespace() const = 0;

		virtual const std::string&		fqName() const = 0;


		const Attributes&				attributes() const
		{
			return( m_attributes );
		}


		std::ostream&	toXML( std::ostream&			outputStream,
							   int						indentLevel,
							   SerializationOptions		options ) const;

	protected :

		Namespace( const std::string&			name,
				   const UID&					uid,
				   const SourceLocation&		sourceLocation,
				   const CompilerSpecific&		compilerSpecificAttr,
				   ConstListPtr<Attribute>&		attributes )
		: SourceElement( name, uid, sourceLocation ),
		  CompilerSpecific( compilerSpecificAttr ),
		  m_attributes( attributes )
		{}

		Namespace( const std::string&			name,
				   const UID&					uid,
				   const SourceLocation&		sourceLocation,
		   	   	   const CompilerSpecific&		compilerSpecificAttr )
		: SourceElement( name, uid, sourceLocation ),
		  CompilerSpecific( compilerSpecificAttr ),
		  m_attributes()
		{}

	private :

		const Attributes				m_attributes;

	};



	class GlobalNamespace : public Namespace
	{
	public :

		GlobalNamespace( GlobalNamespace& ) = delete;
		GlobalNamespace( const GlobalNamespace& ) = delete;

		GlobalNamespace()
			: Namespace( SCOPE_RESOLUTION_OPERATOR, UID( (long)0, UID::UIDType::DECLARATION ), SourceLocation( NULL, 0, 0, 0 ), CompilerSpecific( false, false ) )
		{}

		~GlobalNamespace()
		{}


		bool						isGlobal() const
		{
			return( true );
		}

		const Namespace&			parentNamespace() const
		{
			return( *this );
		}

		const std::string&			fqName() const
		{
			return( name() );
		}
	};



	class NestedNamespace : public Namespace
	{
	public :

		NestedNamespace( NestedNamespace& ) = delete;
		NestedNamespace( const NestedNamespace& ) = delete;

		NestedNamespace( const std::string&			name,
						 const UID&					uid,
						 const SourceLocation&		sourceLocation,
						 const CompilerSpecific&	compilerSpecificAttr,
						 ConstListPtr<Attribute>&	attributes,
						 const Namespace&			parentNamespace )
			: Namespace( name, uid, sourceLocation, compilerSpecificAttr, attributes ),
			  m_parentNamespace( parentNamespace ),
			  m_fqName( parentNamespace.isGlobal() ? name + SCOPE_RESOLUTION_OPERATOR : parentNamespace.fqName() + name + SCOPE_RESOLUTION_OPERATOR )
		{}

		NestedNamespace( const char*				name,
						 const UID&					uid,
						 const SourceLocation&		sourceLocation,
						 const CompilerSpecific&	compilerSpecificAttr,
						 ConstListPtr<Attribute>&	attributes,
						 const Namespace&			parentNamespace )
			: Namespace( name, uid, sourceLocation, compilerSpecificAttr, attributes ),
			  m_parentNamespace( parentNamespace ),
			  m_fqName( parentNamespace.isGlobal() ? name + SCOPE_RESOLUTION_OPERATOR : parentNamespace.fqName() + name + SCOPE_RESOLUTION_OPERATOR )
		{}

		~NestedNamespace()
		{}



		bool						isGlobal() const
		{
			return( false );
		}

		const Namespace&			parentNamespace() const
		{
			return( m_parentNamespace );
		}

		const std::string&			fqName() const
		{
			return( m_fqName );
		}


	private :

		const Namespace&			m_parentNamespace;

		const std::string			m_fqName;
	};


	class NamespaceScoped : public virtual IXMLSerializable
	{
	public :

		NamespaceScoped( const Namespace&		namespaceScope )
			: m_namespaceScope( namespaceScope )
		{}


		const Namespace&		namespaceScope() const
		{
			return( m_namespaceScope );
		}

		std::ostream&	toXML( std::ostream&			outputStream,
							   int						indentLevel,
							   SerializationOptions		options ) const
		{
			m_namespaceScope.toXML( outputStream, indentLevel, options );

			return( outputStream );
		}


	private :

		const Namespace&		m_namespaceScope;
	};


}

#endif /* NAMESPACE_H_ */
