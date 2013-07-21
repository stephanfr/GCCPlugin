/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/


#ifndef ATTRIBUTE_H_
#define ATTRIBUTE_H_




namespace CPPModel
{

	class Attribute : public NamedEntity
	{
	public :

		Attribute() = delete;
		Attribute( Attribute& ) = delete;

		Attribute( const char*					name,
				   ListPtr<ConstantValue>&		arguments )
			: NamedEntity( name ),
			  m_arguments( std::move( arguments ))
		{}

		~Attribute()
		{}


		const ListRef<ConstantValue>		arguments() const
		{
			return( *m_arguments );
		}

		std::ostream&	toXML( std::ostream&			outputStream,
							   int						indentLevel,
							   SerializationOptions		options ) const;

	protected :

		//	The following is needed for deep copying

		friend class Attributes;

		Attribute( const Attribute&				attrToCopy )
			: NamedEntity( attrToCopy.name() )
		{
			ListPtr<ConstantValue>		argumentsCopy( new boost::ptr_list<ConstantValue>() );

			ListRef<ConstantValue>		values( *((attrToCopy.m_arguments).get()) );

			for( const ConstantValue& itrValue : values )
			{
				argumentsCopy->push_back( ConstantValue::Copy( itrValue ) );
			}

			m_arguments = std::move( argumentsCopy );
		}


	private :

		//	This data member cannot be const as the copy constructor cannot initialize it up front

		ConstListPtr<ConstantValue>			m_arguments;
	};


	class Attributes : public IXMLSerializable
	{
	public :

		Attributes( Attributes& ) = delete;
		Attributes( const Attributes& ) = delete;

		Attributes( ConstListPtr<Attribute>&	attributes )
			: m_attributes( std::move( attributes ))
		{}

		Attributes()
			: m_attributes( ConstListPtr<Attribute>( new boost::ptr_list<Attribute>() ))
		{}


		const ListRef<Attribute>			attributes() const
		{
			return( *(m_attributes.get()) );
		}

		static ConstListPtr<Attribute>		deepCopy( const Attributes&			attributesToCopy )
		{
			return( deepCopy( attributesToCopy.attributes() ) );
		}

		static ConstListPtr<Attribute>		deepCopy( const ListRef<Attribute>	listToCopy )
		{
			boost::ptr_list<Attribute>*		copiedList( new boost::ptr_list<Attribute>() );

			for( const Attribute& itrAttribute : listToCopy )
			{
				copiedList->push_back( new Attribute( itrAttribute ) );
			}

			//	Return the list 'constified'

			return( ConstListPtr<Attribute>( copiedList ));
		}


		std::ostream&			toXML( std::ostream&			outputStream,
							   	   	   int						indentLevel,
							   	   	   SerializationOptions		options ) const;


		static ConstListPtr<Attribute>		emptyList()
		{
			return( ConstListPtr<Attribute>( new boost::ptr_list<Attribute>() ) );
		}


	private :

		const ConstListPtr<Attribute>		m_attributes;
	};



	class IAttributes
	{
	public :

		virtual ~IAttributes()
		{};

		virtual const Attributes&			attributes() const = 0;
	};
}



#endif /* ATTRIBUTE_H_ */
