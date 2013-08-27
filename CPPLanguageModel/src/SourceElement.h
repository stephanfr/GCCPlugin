/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/


#ifndef SOURCEELEMENT_H_
#define SOURCEELEMENT_H_


#include "UID.h"
#include "SourceLocation.h"
#include "NamedEntity.h"


namespace CPPModel
{


	class SourceElement : public NamedEntity, public UID, public SourceLocation
	{
	public :

		SourceElement() = delete;
		SourceElement( SourceElement& ) = delete;

		SourceElement( const std::string&		name,
					   const UID&				uid,
					   const SourceLocation&	sourceLocation )
			: NamedEntity( name ),
			  UID( uid ),
			  SourceLocation( sourceLocation )
		{}

		SourceElement( const SourceElement&		elementToCopy )
			: NamedEntity( elementToCopy.name() ),
			  UID( elementToCopy.uid() ),
			  SourceLocation( elementToCopy.fileName().c_str(), elementToCopy.lineNumber(), elementToCopy.characterCount(), elementToCopy.location() )
		{}


		virtual ~SourceElement() {};


		const SourceElement&	sourceElement() const
		{
			return( *this );
		}


		std::ostream&			toXML( std::ostream&			outputStream,
							   	   	   int						indentLevel,
							   	   	   SerializationOptions		options ) const;
	};

}


#endif /* SOURCEELEMENT_H_ */
