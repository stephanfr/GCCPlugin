/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/
#ifndef STATIC_H_
#define STATIC_H_



#include "Serialization.h"



namespace CPPModel
{
	class Static : public IXMLSerializable
	{
	public :

		Static() = delete;
		Static( Static& ) = delete;
		Static( const Static& ) = delete;

		Static( bool		isStatic )
			: m_isStatic( isStatic )
		{}




		bool		isStatic() const
		{
			return( m_isStatic );
		}


		std::ostream&	toXML( std::ostream&			outputStream,
							   SerializationOptions		options ) const;


	private :

		const bool		m_isStatic;
	};
}



#endif /* STATIC_H_ */
