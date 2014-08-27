/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/


#ifndef __CLASS_H__
#define __CLASS_H__


#include "ListAliases.h"

#include "CompilerSpecific.h"
#include "Access.h"
#include "Static.h"
#include "NamedEntity.h"
#include "Attribute.h"
#include "SourceElement.h"
#include "Namespace.h"
#include "Types.h"

#include "Function.h"
#include "Template.h"



namespace CPPModel
{

	class BaseClassIdentifier : public NamedEntity, public Access
	{
	public :

		BaseClassIdentifier() = delete;
		BaseClassIdentifier( BaseClassIdentifier& ) = delete;
		BaseClassIdentifier( const BaseClassIdentifier& ) = delete;


		BaseClassIdentifier( const std::string&		name,
							 bool					isVirtual,
							 AccessSpecifier		accessSpecifier )
			: NamedEntity( name ),
			  Access( accessSpecifier ),
			  m_isVirtual( isVirtual )
		{}


		bool				isVirtual() const
		{
			return( m_isVirtual );
		}


		std::ostream&		toXML( std::ostream&			outputStream,
							   	   SerializationOptions		options ) const;

	private :

		const std::string			m_name;
		const bool					m_isVirtual;
	};



	class FriendIdentifier : public IXMLSerializable
	{
	public :

		enum class FriendKind { CLASS, CLASS_MEMBER, FUNCTION };

		FriendIdentifier( FriendIdentifier& ) = delete;
		FriendIdentifier( const FriendIdentifier& ) = delete;

		virtual ~FriendIdentifier()
		{}


		virtual const FriendKind 		friendKind() const = 0;


	protected :

		FriendIdentifier()
		{}

	};



	class FriendClassIdentifier : public FriendIdentifier
	{
	public :

		FriendClassIdentifier() = delete;
		FriendClassIdentifier( FriendIdentifier& ) = delete;
		FriendClassIdentifier( const FriendIdentifier& ) = delete;

		FriendClassIdentifier( std::unique_ptr<const Type>		type )
			: m_type( std::move( type ) )
		{}

		virtual ~FriendClassIdentifier() {};


		const FriendKind 		friendKind() const
		{
			return( FriendKind::CLASS );
		}


		const Type&				type() const
		{
			return( *m_type );
		}

		std::ostream&	toXML( std::ostream&			outputStream,
							   SerializationOptions		options ) const;

	private :

		const std::unique_ptr<const Type>			m_type;

	};



	class FriendClassMemberIdentifier : public FriendIdentifier
	{
	public :

		FriendClassMemberIdentifier() = delete;
		FriendClassMemberIdentifier( FriendClassMemberIdentifier& ) = delete;
		FriendClassMemberIdentifier( const FriendClassMemberIdentifier& ) = delete;

		FriendClassMemberIdentifier( std::unique_ptr<const Type>	type,
									 const std::string&				methodName )
			: m_type( std::move( type )),
			  m_methodName( methodName )
			{}

		virtual ~FriendClassMemberIdentifier() {};


		const FriendKind 		friendKind() const
		{
			return( FriendKind::CLASS_MEMBER );
		}

		const Type&				type() const
		{
			return( *m_type );
		}

		const std::string&		name() const
		{
			return( m_methodName );
		}


		std::ostream&	toXML( std::ostream&			outputStream,
							   SerializationOptions		options ) const;

	private :

		const std::unique_ptr<const Type>			m_type;
		const std::string							m_methodName;
	};



	class FriendFunctionIdentifier : public FriendIdentifier
	{
	public :

		FriendFunctionIdentifier() = delete;
		FriendFunctionIdentifier( FriendFunctionIdentifier& ) = delete;
		FriendFunctionIdentifier( const FriendFunctionIdentifier& ) = delete;

		FriendFunctionIdentifier( const std::string&		functionName )
			: m_functionName( functionName )
			{}

		virtual ~FriendFunctionIdentifier() {};


		const FriendKind 		friendKind() const
		{
			return( FriendKind::FUNCTION );
		}

		const std::string&		name() const
		{
			return( m_functionName );
		}


		std::ostream&	toXML( std::ostream&			outputStream,
							   SerializationOptions		options ) const;

	private :

		const std::string					m_functionName;
	};




	class FieldOffsetInfo : public IXMLSerializable
	{
	public :

		FieldOffsetInfo() = delete;
		FieldOffsetInfo( FieldOffsetInfo&			offsetInfo ) = delete;

		FieldOffsetInfo( long			size,
						 long			alignment,
						 long			offset,
						 long			bitOffsetAlignment,
						 long			bitOffset )
			: m_size( size ),
			  m_alignment( alignment ),
			  m_offset( offset ),
			  m_bitOffsetAlignment( bitOffsetAlignment ),
			  m_bitOffset( bitOffset )
		{}

		FieldOffsetInfo( const FieldOffsetInfo&			offsetInfo )
			: m_size( offsetInfo.m_size ),
			  m_alignment( offsetInfo.m_alignment ),
			  m_offset( offsetInfo.m_offset ),
			  m_bitOffsetAlignment( offsetInfo.m_bitOffsetAlignment ),
			  m_bitOffset( offsetInfo.m_bitOffset )
		{}



		const long						size() const
		{
			return( m_size );
		}

		const long						alignment() const
		{
			return( m_alignment );
		}

		const long						offset() const
		{
			return( m_offset );
		}

		const long						bitOffsetAlignment() const
		{
			return( m_bitOffsetAlignment );
		}

		const long						bitOffset() const
		{
			return( m_bitOffset );
		}


		const long						totalOffsetInBytes() const
		{
			//	Total offset of the data field in bytes is the offset value + the bit offset turned into bytes

			return( m_offset + ( m_bitOffset / 8 ) );
		}


		std::ostream&	toXML( std::ostream&			outputStream,
							   SerializationOptions		options ) const;

	private :

		const long				m_size;						//	bytes
		const long				m_alignment;				//	bytes
		const long				m_offset;					//	bytes
		const long				m_bitOffsetAlignment;		//	bits
		const long				m_bitOffset;				// 	bits
	};



	class FieldDeclaration : public SourceElement, public Static, public Access, public IAttributes
	{
	public :

		FieldDeclaration() = delete;
		FieldDeclaration( FieldDeclaration& ) = delete;
		FieldDeclaration( const FieldDeclaration& ) = delete;

		FieldDeclaration( const std::string&			name,
						  const UID&					uid,
						  const SourceLocation&			sourceLocation,
						  std::unique_ptr<const Type>	type,
						  bool							isStatic,
						  AccessSpecifier				accessSpec,
						  const FieldOffsetInfo&		offsetInfo,
						  ConstListPtr<Attribute>&		attributes )
					: SourceElement( name, uid, sourceLocation ),
					  Static( isStatic ),
					  Access( accessSpec ),
					  m_attributes( attributes ),
					  m_type( std::move( type )),
					  m_offsetInfo( offsetInfo )
		{}

		~FieldDeclaration()
		{}



		const Type&						type() const
		{
			return( *m_type );
		}

		const Attributes&				attributes() const
		{
			return( m_attributes );
		}

		const FieldOffsetInfo&			offsetInfo() const
		{
			return( m_offsetInfo );
		}



		std::ostream&	toXML( std::ostream&			outputStream,
							   SerializationOptions		options ) const;

	private :

		const Attributes							m_attributes;

		const std::unique_ptr<const Type>			m_type;

		const FieldOffsetInfo						m_offsetInfo;
	};




	class MethodDeclaration : public SourceElement, public Static, public Access, public IAttributes
	{
	public :

		MethodDeclaration() = delete;
		MethodDeclaration( MethodDeclaration& ) = delete;
		MethodDeclaration( const MethodDeclaration& ) = delete;

		MethodDeclaration( const std::string&				name,
						   const UID&						uid,
						   const SourceLocation&			sourceLocation,
						   bool								isStatic,
						   AccessSpecifier					accessSpec,
						   std::unique_ptr<const Type>		resultType,
						   ConstListPtr<Attribute>&			attributeList,
						   ListPtr<FunctionParameter>&		parameterList )
					: SourceElement( name, uid, sourceLocation ),
					  Static( isStatic ),
					  Access( accessSpec ),
					  m_attributes( attributeList ),
					  m_resultType( std::move( resultType )),
					  m_parameterList( std::move( parameterList ))
		{}



		const Type&									resultType() const
		{
			return( *m_resultType );
		}

		const ListRef<FunctionParameter>			parameterList() const
		{
			return( *m_parameterList );
		}

		const Attributes&							attributes() const
		{
			return( m_attributes );
		}

		std::ostream&	toXML( std::ostream&			outputStream,
							   SerializationOptions		options ) const;

	private :

		const Attributes							m_attributes;

		const std::unique_ptr<const Type>			m_resultType;
		const ConstListPtr<FunctionParameter>		m_parameterList;
	};


	class TemplateMethodDeclaration : public SourceElement, public Static, public Access, public IAttributes
	{
	public :

		TemplateMethodDeclaration() = delete;
		TemplateMethodDeclaration( TemplateMethodDeclaration& ) = delete;
		TemplateMethodDeclaration( const TemplateMethodDeclaration& ) = delete;

		TemplateMethodDeclaration( const std::string&				name,
						   	   	   const UID&						uid,
						   	   	   const SourceLocation&			sourceLocation,
						   	   	   bool								isStatic,
						   	   	   AccessSpecifier					accessSpec,
						   	   	   std::unique_ptr<const Type>		resultType,
						   	   	   ConstListPtr<Attribute>&			attributeList,
						   	   	   ListPtr<TemplateParameter>&		parameterList )
					: SourceElement( name, uid, sourceLocation ),
					  Static( isStatic ),
					  Access( accessSpec ),
					  m_attributes( attributeList ),
					  m_resultType( std::move( resultType )),
					  m_parameterList( std::move( parameterList ))
		{}



		const Type&									resultType() const
		{
			return( *m_resultType );
		}

		const ListRef<TemplateParameter>			parameterList() const
		{
			return( *m_parameterList );
		}

		const Attributes&							attributes() const
		{
			return( m_attributes );
		}

		std::ostream&	toXML( std::ostream&			outputStream,
							   SerializationOptions		options ) const;

	private :

		const Attributes							m_attributes;

		const std::unique_ptr<const Type>			m_resultType;
		const ConstListPtr<TemplateParameter>		m_parameterList;
	};




	class ClassDefinition : public SourceElement, public CompilerSpecific, public NamespaceScoped, public IAttributes
	{
	public :

		ClassDefinition() = delete;
		ClassDefinition( ClassDefinition& ) = delete;
		ClassDefinition( const ClassDefinition& ) = delete;

		ClassDefinition( const std::string&								name,
						 const UID&										uid,
						 const Namespace&								namespaceScope,
						 const CompilerSpecific&						compilerSpecificAttr,
						 bool											isStruct,
						 ConstListPtr<Attribute>&						attributes,
						 ConstListPtr<BaseClassIdentifier>&				baseClassList,
						 ConstListPtr<FriendIdentifier>&				friendClassList,
						 ConstListPtr<FieldDeclaration>&				fieldList,
						 ConstListPtr<MethodDeclaration>&				methodList,
						 ConstListPtr<TemplateMethodDeclaration>&		templateMethodList,
						 const SourceLocation&							sourceLocation )
			: SourceElement( name, uid, sourceLocation ),
			  CompilerSpecific( compilerSpecificAttr ),
			  NamespaceScoped( namespaceScope ),
			  m_attributes( attributes ),
			  m_isStruct( isStruct ),
			  m_baseClassList( std::move( baseClassList )),
			  m_friendClassList( std::move( friendClassList )),
			  m_fieldList( std::move( fieldList )),
			  m_methodList( std::move( methodList )),
		  	  m_templateMethodList( std::move( templateMethodList ))
		{}



		bool										isStruct() const
		{
			return( m_isStruct );
		}

		const ListRef<BaseClassIdentifier>			baseClasses() const
		{
			return( *m_baseClassList );
		}

		const ListRef<FriendIdentifier>				friends() const
		{
			return( *m_friendClassList );
		}

		const ListRef<FieldDeclaration>				fields() const
		{
			return(	*m_fieldList );
		}

		const ListRef<MethodDeclaration>			methods() const
		{
			return( *m_methodList );
		}

		const ListRef<TemplateMethodDeclaration>	templateMethods() const
		{
			return( *m_templateMethodList );
		}

		const Attributes&							attributes() const
		{
			return( m_attributes );
		}

		std::ostream&	toXML( std::ostream&			outputStream,
							   SerializationOptions		options ) const;

	private :

		const Attributes										m_attributes;

		const bool												m_isStruct;

		const ConstListPtr<BaseClassIdentifier>					m_baseClassList;
		const ConstListPtr<FriendIdentifier>					m_friendClassList;

		const ConstListPtr<FieldDeclaration>					m_fieldList;
		const ConstListPtr<MethodDeclaration>					m_methodList;
		const ConstListPtr<TemplateMethodDeclaration>			m_templateMethodList;
	};

}	//	CPPModel



#endif /* __CLASS_H__ */
