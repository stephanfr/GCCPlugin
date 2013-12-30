/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/


#ifndef TYPETREE_H_
#define TYPETREE_H_


#include "DeclOrTypeBaseTree.h"



namespace GCCInternalsTools
{

	class TypeTree : public DeclOrTypeBaseTree
	{
	public :

		TypeTree( const tree&		typeTree )
			: DeclOrTypeBaseTree( typeTree )
		{
			assert( TYPE_P( typeTree ) );
		}

		TypeTree&	operator= ( const tree&		typeTree )
		{
			assert( TYPE_P( typeTree ) );

			(tree&)m_tree = typeTree;

			return( *this );
		}


		const std::string									identifier() const
		{
			const tree&		typeName = TYPE_IDENTIFIER( m_tree );

			if( typeName == 0 )
			{
				return( "[No Identifier]" );
			}

			const char* identifier = IDENTIFIER_POINTER( typeName );

			return( identifier ? std::string( identifier ) : std::string( "[No Identifier]" ));
		}

		const CPPModel::UID									uid() const
		{
			return( CPPModel::UID( TYPE_UID( m_tree ), CPPModel::UID::UIDType::TYPE ) );
		}

		const CPPModel::CompilerSpecific					compilerSpecificFlags() const
		{
			return( CPPModel::CompilerSpecific( false, TYPE_ARTIFICIAL( m_tree ) ) );
		}


		virtual const NamespaceTree							fullyQualifiedNamespace() const;

		std::unique_ptr<const CPPModel::Type>				type( const CPPModel::ASTDictionary&		dictionary ) const;

		CPPModel::TypeSpecifier								typeSpecifier() const;

		CPPModel::ConstListPtr<CPPModel::Attribute>			attributes();
	};
}


#endif /* TYPETREE_H_ */
