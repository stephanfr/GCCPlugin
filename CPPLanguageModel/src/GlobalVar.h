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


#include "DeclarationType.h"
#include "Static.h"
#include "ASTEntry.h"
#include "NamedEntity.h"
#include "Attribute.h"
#include "Namespace.h"



namespace CPPModel
{

	class GlobalVarBase : public NamedEntity, public NamespaceScoped, public Static, public IAttributes
	{
	public :

		GlobalVarBase() = delete;
		GlobalVarBase( GlobalVarBase& ) = delete;
		GlobalVarBase( const GlobalVarBase& ) = delete;

		GlobalVarBase( const std::string&			name,
					   const Namespace&				namespaceScope,
					   bool							isStatic,
					   const Attributes&			attributes )
			: NamedEntity( name ),
			  NamespaceScoped( namespaceScope ),
			  Static( isStatic ),
			  m_attributes( attributes )
		{}

		virtual ~GlobalVarBase() {};


		const Attributes&				attributes() const
		{
			return( m_attributes );
		}

	private :

		const Attributes							m_attributes;
	};




		class GlobalVarDeclaration : public GlobalVarBase, public DeclarationType
		{
		public :

			GlobalVarDeclaration() = delete;
			GlobalVarDeclaration( GlobalVarDeclaration& ) = delete;
			GlobalVarDeclaration( const GlobalVarDeclaration& ) = delete;

			GlobalVarDeclaration( const std::string&			name,
								  const Namespace&				namespaceScope,
								  bool							isStatic,
								  const Attributes&				attributes,
								  std::unique_ptr<const Type>	varType )
				: GlobalVarBase( name, namespaceScope, isStatic, attributes ),
				  DeclarationType( varType )
			{}

			GlobalVarDeclaration( const std::string&			name,
								  const Namespace&				namespaceScope,
								  bool							isStatic,
								  const Attributes&				attributes,
								  const DictionaryClassEntry&	classType )
				: GlobalVarBase( name, namespaceScope, isStatic, attributes ),
				  DeclarationType( classType )
			{}

			GlobalVarDeclaration( const std::string&			name,
								  const Namespace&				namespaceScope,
								  bool							isStatic,
								  const Attributes&				attributes,
								  const DictionaryUnionEntry&	unionType )
				: GlobalVarBase( name, namespaceScope, isStatic, attributes ),
				  DeclarationType( unionType )
			{}

			virtual ~GlobalVarDeclaration() {};



			std::ostream&	toXML( std::ostream&			outputStream,
								   int						indentLevel,
								   SerializationOptions		options ) const;

		private :

			const Attributes					m_attributes;
		};




	class GlobalVarEntry : public GlobalVarBase, public ASTEntry
	{
	public :

		GlobalVarEntry() = delete;
		GlobalVarEntry( GlobalVarEntry& ) = delete;
		GlobalVarEntry( const GlobalVarEntry& ) = delete;

		GlobalVarEntry( const std::string&				name,
						const UID&						uid,
						const Namespace&				namespaceScope,
						const SourceLocation&			sourceLocation,
						bool							isStatic,
						const Attributes&				attributes,
						std::unique_ptr<const Type>		varType )
			: GlobalVarBase( name, namespaceScope, isStatic, std::move( attributes ) ),
			  ASTEntry( uid, sourceLocation ),
			  m_varType( std::move( varType ) )
		{}

		virtual ~GlobalVarEntry() {};


		const Type&			type() const
		{
			return( *m_varType );
		}

		std::ostream&		toXML( std::ostream&			outputStream,
							   	   int						indentLevel,
							   	   SerializationOptions		options ) const;

	private :

		const std::unique_ptr<const Type>			m_varType;
	};

}



#endif /* GLOBALVAR_H_ */
