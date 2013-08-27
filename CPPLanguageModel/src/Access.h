/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/



#ifndef ACCESS_H_
#define ACCESS_H_


#include "Serialization.h"



namespace CPPModel
{

	enum class AccessSpecifier
	{
		PUBLIC,
		PROTECTED,
		PRIVATE
	};



	const char*		toString( AccessSpecifier	accessSpec );



	class Access : public IXMLSerializable
	{
	public :

		Access( AccessSpecifier		accessSpec )
			: m_accessSpec( accessSpec )
		{}


		AccessSpecifier		accessSpecifier() const
		{
			return( m_accessSpec );
		}


		std::ostream&		toXML( std::ostream&			outputStream,
							   	   int						indentLevel,
							   	   SerializationOptions		options ) const;


	private :

		const AccessSpecifier				m_accessSpec;
	};

}



#endif /* ACCESS_H_ */
