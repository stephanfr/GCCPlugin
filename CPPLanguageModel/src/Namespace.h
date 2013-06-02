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




namespace CPPModel
{

	class Namespace : public XMLSerializable
	{
	public :

		Namespace( Namespace& ) = delete;
		Namespace( const Namespace& ) = delete;

		Namespace( const char*				elementNamespace )
			: m_namespace( elementNamespace )
		{}

		Namespace( const std::string&		elementNamespace )
			: m_namespace( elementNamespace )
		{}

		Namespace() {};


		const std::string&			enclosingNamespace() const
		{
			return( m_namespace );
		}


		std::ostream&	toXML( std::ostream&			outputStream,
							   int						indentLevel,
							   SerializationOptions		options ) const
		{
			outputStream << XMLIndentTable::GetIndent( indentLevel ) << "<namespace>" << m_namespace << "</namespace>\n";

			return( outputStream );
		}


	private :

		const std::string			m_namespace;
	};

}

#endif /* NAMESPACE_H_ */
