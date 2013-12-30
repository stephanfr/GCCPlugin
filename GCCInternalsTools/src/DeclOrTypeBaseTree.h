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


#include <memory>



typedef union tree_node *tree;


namespace CPPModel
{
	enum class TypeSpecifier;
	class CompilerSpecific;
	class UID;
	class Type;
	class ASTDictionary;
}


namespace GCCInternalsTools
{

	class NamespaceTree;


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

		//
		//	Virtual Methods
		//

		virtual const std::string							identifier() const = 0;

		virtual const CPPModel::UID							uid() const = 0;

		virtual const CPPModel::CompilerSpecific			compilerSpecificFlags() const = 0;

		virtual const NamespaceTree							fullyQualifiedNamespace() const = 0;

		virtual CPPModel::TypeSpecifier						typeSpecifier() const = 0;

		virtual std::unique_ptr<const CPPModel::Type>		type( const CPPModel::ASTDictionary&		dictionary ) const = 0;


	protected :

		DeclOrTypeBaseTree( const tree&		unclassifiedTree )
			: m_tree( unclassifiedTree )
		{}

		const tree&			m_tree;
	};
}


#endif /* DECLORTYPEBASETREE_H_ */


