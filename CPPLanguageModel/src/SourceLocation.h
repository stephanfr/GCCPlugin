/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/
#ifndef SOURCELOCATION_H_
#define SOURCELOCATION_H_




namespace CPPModel
{
	class SourceLocation : public IXMLSerializable
	{
	public :

		SourceLocation() = delete;
		SourceLocation( SourceLocation& ) = delete;

		SourceLocation( const char*		fileName,
					    long			lineNumber,
					    int				characterCount,
					    long			location )
			: m_fileName( fileName != NULL ? fileName : "" ),
			  m_lineNumber( lineNumber ),
			  m_characterCount( characterCount ),
			  m_location( location )
		{}

		SourceLocation( const SourceLocation&		locationToCopy )
			: m_fileName( locationToCopy.m_fileName ),
			  m_lineNumber( locationToCopy.m_lineNumber ),
			  m_characterCount( locationToCopy.m_characterCount ),
			  m_location( locationToCopy.m_location )
		{}

		virtual ~SourceLocation()
		{}



		virtual const bool			isUnknown() const
		{
			return( false );
		}

		const std::string&			fileName() const
		{
			return( m_fileName );
		}

		long						lineNumber() const
		{
			return( m_lineNumber );
		}

		int							characterCount() const
		{
			return( m_characterCount );
		}

		long						location() const
		{
			return( m_location );
		}


		//	Comparison operator for ordered lists

		bool operator<( const SourceLocation&	locationToCompare ) const
		{
			return( m_location < locationToCompare.m_location );
		}


		//	Implementation of XMLSerializable virtual method

		virtual std::ostream&	toXML( std::ostream&			outputStream,
							   	   	   int						indentLevel,
							   	   	   SerializationOptions		options ) const;

	private :

		const std::string		m_fileName;
		const long				m_lineNumber;
		const int				m_characterCount;
		const long				m_location;
	};

}	//	namespace CPPModel


#endif /* SOURCELOCATION_H_ */
