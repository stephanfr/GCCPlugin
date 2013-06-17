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


namespace CPPModel
{


	class SourceElement : public NamedEntity, public SourceLocation
	{
	public :

		SourceElement() = delete;
		SourceElement( SourceElement& ) = delete;

		SourceElement( const std::string&		name,
					   const UID&				uid,
					   const SourceLocation&	sourceLocation )
			: NamedEntity( name ),
			  SourceLocation( sourceLocation ),
			  m_uid( uid )
		{}

		SourceElement( const SourceElement&		elementToCopy )
			: NamedEntity( elementToCopy.name() ),
			  SourceLocation( elementToCopy.fileName().c_str(), elementToCopy.lineNumber(), elementToCopy.characterCount(), elementToCopy.location() ),
			  m_uid( elementToCopy.m_uid )
		{}


		virtual ~SourceElement() {};


		const UID&				uid() const
		{
			return( m_uid );
		}


		std::ostream&			toXML( std::ostream&			outputStream,
							   	   	   int						indentLevel,
							   	   	   SerializationOptions		options ) const;

	private :

		const UID				m_uid;
	};

}


#endif /* SOURCEELEMENT_H_ */
