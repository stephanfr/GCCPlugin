/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/


#ifndef ASTENTRY_H_
#define ASTENTRY_H_


namespace CPPModel
{


	class ASTEntry : public SourceLocation
	{
	public :

		ASTEntry() = delete;
		ASTEntry( ASTEntry& ) = delete;

		ASTEntry( const UID&				uid,
				  const SourceLocation&		sourceLocation )
			: SourceLocation( sourceLocation ),
			  m_uid( uid )
		{}

		ASTEntry( const ASTEntry&		elementToCopy )
			: SourceLocation( elementToCopy.fileName().c_str(), elementToCopy.lineNumber(), elementToCopy.characterCount(), elementToCopy.location() ),
			  m_uid( elementToCopy.m_uid )
		{}


		virtual ~ASTEntry() {};


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


#endif /* ASTENTRY_H_ */
