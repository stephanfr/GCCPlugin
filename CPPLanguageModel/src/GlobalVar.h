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

namespace CPPModel
{


	class GlobalVarDeclaration : public NamedEntity, public Namespace, public Static, public Attributes
	{
	public :

		GlobalVarDeclaration() = delete;
		GlobalVarDeclaration( GlobalVarDeclaration& ) = delete;
		GlobalVarDeclaration( const GlobalVarDeclaration& ) = delete;

		GlobalVarDeclaration( const std::string&				name,
							  const std::string&				enclosingNamespace,
							  bool								isStatic,
							  ConstListPtr<Attribute>			attributes,
							  std::unique_ptr<const Type>		varType )
			: NamedEntity( name ),
			  Namespace( enclosingNamespace ),
			  Static( isStatic ),
			  Attributes( attributes ),
			  m_varType( std::move( varType ) )
		{}


		const Type&			type() const
		{
			return( *m_varType );
		}


		std::ostream&	toXML( std::ostream&			outputStream,
							   int						indentLevel,
							   SerializationOptions		options ) const;

	private :

		const std::unique_ptr<const Type>				m_varType;

	};



	class GlobalVarEntry : public GlobalVarDeclaration, public ASTEntry
	{
	public :

		GlobalVarEntry() = delete;
		GlobalVarEntry( GlobalVarEntry& ) = delete;
		GlobalVarEntry( const GlobalVarEntry& ) = delete;

		GlobalVarEntry( const std::string&				name,
						const UID&						uid,
						const std::string&				enclosingNamespace,
						const SourceLocation&			sourceLocation,
						bool							isStatic,
						ConstListPtr<Attribute>			attributes,
						std::unique_ptr<const Type>		varType )
			: GlobalVarDeclaration( name, enclosingNamespace, isStatic, std::move( attributes ), std::move( varType ) ),
			  ASTEntry( uid, sourceLocation )
		{}


		std::ostream&	toXML( std::ostream&			outputStream,
							   int						indentLevel,
							   SerializationOptions		options ) const;
	};

}



#endif /* GLOBALVAR_H_ */
