/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/


#ifndef UNION_H_
#define UNION_H_


namespace CPPModel
{

	class UnionMember : public SourceElement, public Access
	{
	public :

		UnionMember() = delete;
		UnionMember( UnionMember& ) = delete;
		UnionMember( const UnionMember& ) = delete;

		UnionMember( const std::string&				name,
					 const UID&						uid,
					 std::unique_ptr<const Type>	type,
					 AccessSpecifier				accessSpec,
					 const SourceLocation&			sourceLocation)
					: SourceElement( name, uid, sourceLocation ),
					  Access( accessSpec ),
					  m_type( std::move( type ))
		{}

		virtual ~UnionMember()
		{}


		const Type&			type() const
		{
			return( *m_type );
		}

		std::ostream&		toXML( std::ostream&			outputStream,
							   	   int						indentLevel,
							   	   SerializationOptions		options ) const;

	private :

		const std::unique_ptr<const Type>			m_type;
	};



	class UnionDefinition : public SourceElement, public Namespace, public Attributes
	{
	public :

		UnionDefinition() = delete;
		UnionDefinition( UnionDefinition& ) = delete;
		UnionDefinition( const UnionDefinition& ) = delete;

		UnionDefinition( const std::string&				name,
					     const UID&						uid,
					     const std::string&				enclosingNamespace,
					     const SourceLocation&			sourceLocation,
						 ConstListPtr<Attribute>		attributes,
					     ConstListPtr<UnionMember>		memberList )
			: SourceElement( name, uid, sourceLocation ),
			  Namespace( enclosingNamespace ),
			  Attributes( attributes ),
			  m_memberList( std::move( memberList ))
		{
			assert( uid.type() == UID::UIDType::DECLARATION );
		}

		virtual ~UnionDefinition()
		{}


		const ListRef<UnionMember>		members() const
		{
			return( *m_memberList );
		}


		std::ostream&	toXML( std::ostream&			outputStream,
							   int						indentLevel,
							   SerializationOptions		options ) const;

	private :

		const ConstListPtr<UnionMember>		m_memberList;
	};

}



#endif /* UNION_H_ */
