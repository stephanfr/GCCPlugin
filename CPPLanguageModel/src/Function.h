/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/


#ifndef FUNCTION_H_
#define FUNCTION_H_



namespace CPPModel
{

	class FunctionParameter : public NamedEntity
	{
	public :

		FunctionParameter() = delete;
		FunctionParameter( FunctionParameter& ) = delete;
		FunctionParameter( const FunctionParameter& ) = delete;

		FunctionParameter( const std::string				name,
						   std::unique_ptr<const Type>		type )
			: NamedEntity( name ),
			  m_type( std::move( type ) )
		{}

		~FunctionParameter()
		{}


		const Type&			type() const
		{
			return( *m_type );
		}


		std::ostream&	toXML( std::ostream&			outputStream,
							   int						indentLevel,
							   SerializationOptions		options ) const;

	private :

		const std::unique_ptr<const Type>		m_type;
	};



	class FunctionParameters : public IXMLSerializable
	{
	public :

		FunctionParameters() = delete;
		FunctionParameters( FunctionParameters& ) = delete;
		FunctionParameters( const FunctionParameters& ) = delete;

		FunctionParameters( ConstListPtr<FunctionParameter>&		parameters )
			: m_parameters( std::move( parameters ))
		{}

		FunctionParameters( ListPtr<FunctionParameter>&		parameters )
			: m_parameters( std::move( parameters ))
		{}



		const ListRef<FunctionParameter>			parameters() const
		{
			return( *(m_parameters.get()) );
		}


		std::ostream&								toXML( std::ostream&			outputStream,
							   	   	   	 	 	 	 	   int						indentLevel,
							   	   	   	 	 	 	 	   SerializationOptions		options ) const;

	private :

		const ConstListPtr<FunctionParameter>		m_parameters;
	};



	class FunctionDefinition : public SourceElement, public NamespaceScoped, public IAttributes
	{
	public :

		FunctionDefinition() = delete;
		FunctionDefinition( FunctionDefinition& ) = delete;
		FunctionDefinition( const FunctionDefinition& ) = delete;

		FunctionDefinition( const std::string&					name,
						 	const UID&							uid,
						 	const Namespace&					namespaceScope,
						 	const SourceLocation&				sourceLocation,
						 	const bool							hiddenFriend,
							ConstListPtr<Attribute>				attributes,
							std::unique_ptr<const Type>			returnType,
						 	ConstListPtr<FunctionParameter>		parameters )
			: SourceElement( name, uid, sourceLocation ),
			  NamespaceScoped( namespaceScope ),
			  m_attributes( attributes ),
			  m_hiddenFriend( hiddenFriend ),
			  m_returnType( std::move( returnType ) ),
			  m_parameters( parameters )
		{}


		const bool		isHiddenFriend() const
		{
			return( m_hiddenFriend );
		}

		const Type&			returnType() const
		{
			return( *m_returnType );
		}

		const Attributes&				attributes() const
		{
			return( m_attributes );
		}


		std::ostream&	toXML( std::ostream&			outputStream,
							   int						indentLevel,
							   SerializationOptions		options ) const;

	private :

		const Attributes						m_attributes;

		const bool								m_hiddenFriend;

		const std::unique_ptr<const Type>		m_returnType;

		const FunctionParameters				m_parameters;
	};

}


#endif /* FUNCTION_H_ */
