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


namespace GCCInternalsTools
{
	const std::string		NAMESPACE_SEPARATOR = "::";


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


		const CPPModel::UID									uid() const
		{
			return( CPPModel::UID( TYPE_UID( TYPE_MAIN_VARIANT( m_tree ) ), CPPModel::UID::UIDType::TYPE ) );
		}


		const std::string									enclosingNamespace() const;

		std::unique_ptr<const CPPModel::Type>				type( const CPPModel::ASTDictionary&		dictionary ) const;

		CPPModel::TypeInfo::Specifier						typeSpecifier() const;

		CPPModel::ConstListPtr<CPPModel::Attribute>			attributes();
	};
}


#endif /* TYPETREE_H_ */
