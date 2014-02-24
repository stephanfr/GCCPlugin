/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/


#ifndef TREELIST_H_
#define TREELIST_H_


#include "DeclTree.h"



namespace GCCInternalsTools
{

	template <class T, int treeCode>
	class TreeList
	{
	public :

		class iterator
		{
		public :

			iterator( const iterator&		itrToCopy )
				: m_tree( itrToCopy.m_tree )
			{
				if( m_tree != NULL_TREE )
				{
					m_castTree.reset( new T( m_tree ) );
				}
				else
				{
					m_castTree.reset();
				}
			}


			iterator&	operator=( const iterator&	itrToCopy )
			{
				m_tree = itrToCopy.m_tree;

				if( m_tree != NULL_TREE )
				{
					m_castTree.reset( new T( m_tree ) );
				}
				else
				{
					m_castTree.reset();
				}

				return( *this );
			}

			const iterator& operator ++()
			{
				m_tree = TREE_CHAIN( m_tree );

				if( m_tree != NULL_TREE )
				{
					m_castTree.reset( new T( m_tree ) );
				}
				else
				{
					m_castTree.reset();
				}

				return( *this );
			}

			bool operator ==( const iterator&	itrToCompare ) const
			{
				return( m_tree == itrToCompare.m_tree );
			}

			bool operator !=( const iterator&	itrToCompare ) const
			{
				return( m_tree != itrToCompare.m_tree );
			}

			const T& operator*() const
			{
				assert( m_tree != NULL_TREE );

				return( *m_castTree );
			}

			const T* const operator->() const
			{
				assert( m_tree != NULL_TREE );

				return( m_castTree.get() );
			}

		private :

			tree					m_tree;

			std::unique_ptr<T>		m_castTree;


			iterator( tree		treeList )
				: m_tree( treeList )
			{
				if( m_tree != NULL_TREE )
				{
					m_castTree.reset( new T( m_tree ) );
				}
				else
				{
					m_castTree.reset();
				}
			}

			friend TreeList<T, treeCode>;
		};


		TreeList( const tree		treeList )
			: m_begin( treeList ),
			  m_end( NULL_TREE )
		{
			if(( treeList != NULL ) && ( treeCode != 0 ))
			{
				assert( TREE_CODE( treeList ) == treeCode );
			}
		}



		TreeList&	operator= ( const TreeList&		treeList )
		{
			m_begin = treeList.m_begin;

			return( *this );
		}


		bool		empty() const
		{
			return( m_begin == NULL_TREE );
		}

		iterator 	begin() const
		{
			return(  m_begin );
		}

		iterator 	end() const
		{
			return( m_end );
		}


	private :


		const iterator		m_begin;
		const iterator		m_end;
	};




	class PurposeValueElement
	{
	public :

		PurposeValueElement( const tree&				listElement )
			: m_listElement( listElement )
		{}


		const PurposeValueElement&	operator=( const tree& 	listElement )
		{
			(tree&)m_listElement = listElement;

			return( *this );
		}

		const tree&			purpose() const
		{
			return( TREE_PURPOSE( m_listElement ) );
		}

		const tree&			value() const
		{
			return( TREE_VALUE( m_listElement ) );
		}

	private :

		const tree&			m_listElement;


		operator const tree&() const
		{
			return( m_listElement );
		}


		friend TreeList<PurposeValueElement, TREE_LIST>;
	};



	typedef TreeList<PurposeValueElement, TREE_LIST>		PurposeValueList;

	//	Field lists contain a variety elements, thus passing zero to disable the type checking

	typedef TreeList<DeclTree, 0>							DeclList;
	typedef TreeList<DeclTree, NAMESPACE_DECL>				NamespaceList;

	typedef TreeList<DeclTree, 0>							FieldList;

	typedef TreeList<DeclTree, FUNCTION_DECL>				FunctionList;
	typedef TreeList<DeclTree, 0>							MethodList;
	typedef TreeList<DeclTree, PARM_DECL>					ParameterList;

	typedef TreeList<DeclTree, 0>							UnionMemberList;

}



#endif /* TREELIST_H_ */
