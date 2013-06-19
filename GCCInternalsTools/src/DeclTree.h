/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/


#ifndef DECLTREE_H_
#define DECLTREE_H_



namespace GCCInternalsTools
{

	class DeclTree : public DeclOrTypeBaseTree
	{
	public :

		DeclTree( const tree&		declTree )
			: DeclOrTypeBaseTree( declTree )
		{
			assert( DECL_P( declTree ) );
		}


		const DeclTree&	operator= ( const tree&		declTree )
		{
			assert( DECL_P( declTree ) );

			(tree&)m_tree = declTree;

			return( *this );
		}

		bool		isArtificial() const
		{
			return( DECL_ARTIFICIAL( m_tree ) );
		}

		bool		isBuiltIn() const
		{
			return( DECL_IS_BUILTIN( m_tree ) );
		}

		bool		isAFunctionDeclaration() const
		{
			//	Function declarations are simple, the tree is a FUNCTION_DECL or not

			return( TREE_CODE( m_tree ) == FUNCTION_DECL );
		}

		int			treeCode() const
		{
			return( TREE_CODE( m_tree ) );
		}

		TypeTree	treeType() const
		{
			return( TypeTree( TREE_TYPE( m_tree ) ) );
		}

		const std::string								identifier() const
		{
			const tree&		declName = DECL_NAME( m_tree );

			if( declName == 0 )
			{
				return( "[No Identifier]" );
			}

			const char* identifier = IDENTIFIER_POINTER( declName );

			return( identifier ? std::string( identifier ) : std::string( "[No Identifier]" ));
		}

		const CPPModel::UID								uid() const
		{
			return( CPPModel::UID( DECL_UID( m_tree ), CPPModel::UID::UIDType::DECLARATION ) );
		}


		CPPModel::AccessSpecifier						accessSpecifier() const
		{
			//	We only have tests for PRIVATE or PROTECTED, so if it is not either of those access types then it is PUBLIC

			if( TREE_PRIVATE( m_tree ))
			{
				return( CPPModel::AccessSpecifier::PRIVATE );
			}
			else if( TREE_PROTECTED( m_tree ))
			{
				return( CPPModel::AccessSpecifier::PROTECTED );
			}

			return( CPPModel::AccessSpecifier::PUBLIC );
		}


		const CPPModel::SourceLocation					sourceLocation() const
		{
			return( CPPModel::SourceLocation( DECL_SOURCE_FILE ( m_tree ), DECL_SOURCE_LINE ( m_tree ),  1, DECL_SOURCE_LOCATION( m_tree ) ) );
		}


		const std::string								enclosingNamespace() const;


		CPPModel::TypeInfo::Specifier					typeSpecifier() const
		{
			return( TypeTree( TREE_TYPE( m_tree )).typeSpecifier() );
		}


		std::unique_ptr<const CPPModel::Type>			type( const CPPModel::ASTDictionary&		dictionary ) const
		{
			return( TypeTree( TREE_TYPE( m_tree )).type( dictionary ) );
		}

		CPPModel::ConstListPtr<CPPModel::Attribute>		attributes();
	};

}


#endif /* DECLTREE_H_ */
