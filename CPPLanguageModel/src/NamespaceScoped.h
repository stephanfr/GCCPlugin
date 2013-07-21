/*
 * NamespaceScoped.h
 *
 *  Created on: Jul 6, 2013
 *      Author: steve
 */

#ifndef NAMESPACESCOPED_H_
#define NAMESPACESCOPED_H_

namespace CPPModel
{

	class NamespaceScoped : public virtual IXMLSerializable
	{
	public :

		NamespaceScoped( const Namespace&		namespaceScope )
			: m_namespaceScope( namespaceScope )
		{}


		const Namespace&		namespaceScope() const
		{
			return( m_namespaceScope );
		}

		std::ostream&	toXML( std::ostream&			outputStream,
							   int						indentLevel,
							   SerializationOptions		options ) const
		{
			m_namespaceScope.toXML( outputStream, indentLevel, options );

			return( outputStream );
		}


	private :

		const Namespace&		m_namespaceScope;
	};

}

#endif /* NAMESPACESCOPED_H_ */
