/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/


#ifndef DECLORTYPEBASETREE_H_
#define DECLORTYPEBASETREE_H_



namespace GCCInternalsTools
{


	class DeclOrTypeBaseTree
	{
	public :

		static std::unique_ptr<DeclOrTypeBaseTree>		convert( const tree&		treeToConvert );


		virtual ~DeclOrTypeBaseTree()
		{}


		operator const tree&() const
		{
			return( m_tree );
		}

		bool		isNull() const
		{
			return( m_tree == NULL );
		}

		const std::string					identifier() const
		{
			const tree&		declName = DECL_NAME( m_tree );

			if( declName == 0 )
			{
				return( "[No Identifier]" );
			}

			const char* identifier = IDENTIFIER_POINTER( declName );

			return( identifier ? std::string( identifier ) : std::string( "[No Identifier]" ));
		}


		//
		//	Virtual Methods
		//

		virtual const CPPModel::UID							UID() const = 0;

		virtual const std::string							Namespace() const = 0;

		virtual CPPModel::TypeInfo::Specifier				typeSpecifier() const = 0;

		virtual std::unique_ptr<const CPPModel::Type>		type( const CPPModel::ASTDictionary&		dictionary ) const = 0;


	protected :

		DeclOrTypeBaseTree( const tree&		unclassifiedTree )
			: m_tree( unclassifiedTree )
		{}

		const tree&			m_tree;
	};
}


#endif /* DECLORTYPEBASETREE_H_ */


