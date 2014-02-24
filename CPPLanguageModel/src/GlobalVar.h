/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/



#ifndef GLOBALVAR_H_
#define GLOBALVAR_H_


#include "Static.h"
#include "SourceElement.h"
#include "Attribute.h"
#include "Namespace.h"




namespace CPPModel
{



	class GlobalVarEntry : public SourceElement, public NamespaceScoped, public Static, public IAttributes
	{
	public :

		GlobalVarEntry() = delete;
		GlobalVarEntry( GlobalVarEntry& ) = delete;
		GlobalVarEntry( const GlobalVarEntry& ) = delete;

		GlobalVarEntry( const std::string&				name,
						const UID&						uid,
						const Namespace&				namespaceScope,
						const SourceLocation&			sourceLocation,
						bool							isStatic,
						const Attributes&				attributes,
						std::unique_ptr<const Type>		varType )
			: SourceElement( name, uid, sourceLocation ),
			  NamespaceScoped( namespaceScope ),
			  Static( isStatic ),
			  m_attributes( std::move( attributes ) ),
			  m_varType( std::move( varType ) )
		{}

		virtual ~GlobalVarEntry() {};


		const Type&			type() const
		{
			return( *m_varType );
		}

		const Attributes&				attributes() const
		{
			return( m_attributes );
		}


		std::ostream&		toXML( std::ostream&			outputStream,
							   	   SerializationOptions		options ) const;

	private :

		const Attributes							m_attributes;

		const std::unique_ptr<const Type>			m_varType;
	};

}



#endif /* GLOBALVAR_H_ */
