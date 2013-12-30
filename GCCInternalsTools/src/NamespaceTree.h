/*
 * FullyQualifiedNamespace.h
 *
 *  Created on: Dec 14, 2013
 *      Author: steve
 */

#ifndef NAMESPACETREE_H_
#define NAMESPACETREE_H_


#include <list>
#include <string>

#include "Constants.h"

//#include "config.h"
//#include "gcc-plugin.h"
//#include "tree.h"
//#include "cp/cp-tree.h"

#include "DeclTree.h"


namespace GCCInternalsTools
{

	class NamespaceTree : public DeclTree
	{
	public:

		NamespaceTree( const tree&				namespaceNode )
		: DeclTree( namespaceNode )
		{
			for( tree currentContext = (tree&)namespaceNode; ( currentContext != NULL ) && ( TREE_CODE( currentContext ) == NAMESPACE_DECL ) && ( currentContext != global_namespace ); currentContext = DECL_CONTEXT( currentContext ) )
			{
				m_namespaceNodes.push_front( currentContext );
			}

			m_namespaceNodes.push_front( global_namespace );
		}


		virtual ~NamespaceTree() {};


		const std::list<tree>		nodes() const
		{
			return( m_namespaceNodes );
		}

		const std::string			asString() const
		{
			std::string			fqNamespace;

			if(( m_namespaceNodes.size() == 1 ) && ( m_namespaceNodes.front() == global_namespace ))
			{
				fqNamespace = CPPModel::SCOPE_RESOLUTION_OPERATOR;
			}
			else
			{
				for( tree currentNode : m_namespaceNodes )
				{
					//	If this is the global namespace, then just tack it on and continue

					if( currentNode == global_namespace )
					{
						continue;
					}

					//	If this is the standard library namespace, then tack it on and continue

					if( DECL_NAMESPACE_STD_P( currentNode ))
					{
						fqNamespace += CPPModel::STD_NAMESPACE_LABEL;
						continue;
					}

					//	If we are inside of a class, then add that to the namespace

					if( TREE_CODE( currentNode ) == RECORD_TYPE )
					{
						currentNode = TYPE_NAME( currentNode );
					}

					fqNamespace += IDENTIFIER_POINTER( DECL_NAME( currentNode ) ) + CPPModel::SCOPE_RESOLUTION_OPERATOR;
				}
			}

			return( fqNamespace );
		}


	private :

		std::list<tree>				m_namespaceNodes;
	};

} /* namespace GCCInternalsTools */

#endif /* NAMESPACETREE_H_ */
