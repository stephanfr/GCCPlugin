/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/


#ifndef NAMEDENTITY_H_
#define NAMEDENTITY_H_


#include "Serialization.h"


namespace CPPModel
{

	class NamedEntity : public IXMLSerializable
	{
	public :

		NamedEntity() = delete;
		NamedEntity( NamedEntity& ) = delete;
		NamedEntity( const NamedEntity& ) = delete;

		NamedEntity( const std::string&		name )
			: m_name( name )
		{}

		virtual ~NamedEntity()
		{}


		const std::string&			name() const
		{
			return( m_name );
		}


		std::ostream&	toXML( std::ostream&			outputStream,
							   SerializationOptions		options ) const;

	private :

		const std::string		m_name;
	};

}

#endif /* NAMEDENTITY_H_ */
