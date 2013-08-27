/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/


#ifndef DECLARATIONTYPE_H_
#define DECLARATIONTYPE_H_



#include <memory>
#include <boost/variant.hpp>

#include "Serialization.h"
#include "Types.h"




namespace CPPModel
{

	class DictionaryClassEntry;
	class DictionaryUnionEntry;


	typedef boost::variant< const Type&, const DictionaryClassEntry&, const DictionaryUnionEntry& >	DeclarationTypeVariant;


	class DeclarationType : public IXMLSerializable
	{
	public :

		DeclarationType() = delete;
		DeclarationType( DeclarationType& ) = delete;
		DeclarationType( const DeclarationType& ) = delete;

		DeclarationType( std::unique_ptr<const Type>&	varType )
			: m_type( std::move( varType )),
			  m_varType( *m_type )
		{}

		DeclarationType( const DictionaryClassEntry&	classType )
			: m_varType( classType )
		{}

		DeclarationType( const DictionaryUnionEntry&	unionType )
			: m_varType( unionType )
		{}

		virtual ~DeclarationType() {};


		const DeclarationTypeVariant&		typeVariant() const
		{
			return( m_varType );
		}


		std::ostream&	toXML( std::ostream&			outputStream,
							   int						indentLevel,
							   SerializationOptions		options ) const;

	private :

		std::unique_ptr<const Type>			m_type;

		const DeclarationTypeVariant		m_varType;
	};

}


#endif /* DECLARATIONTYPE_H_ */


