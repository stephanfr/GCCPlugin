/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/


#ifndef GCCINTERNALSTOOLS_H
#define GCCINTERNALSTOOLS_H


#include "ASTDictionary.h"

#include "config.h"
#include "gcc-plugin.h"
#include "tree.h"
#include "cp/cp-tree.h"
#include "diagnostic.h"
#include "real.h"
#include "toplev.h"



namespace GCCInternalsTools
{


//	Forward Declarations

	class NamespaceTree;



	class DictionaryTreeMixin
	{
	public :

		DictionaryTreeMixin( const tree&			treeNode )
				: m_mainTypeVariant( treeNode )
			{}


		const tree				getTree() const
		{
			return( m_mainTypeVariant );
		}

	private :

		const tree				m_mainTypeVariant;
	};





	class DictionaryClassEntryImpl : public CPPModel::DictionaryClassEntry, public DictionaryTreeMixin
	{
	public :

		DictionaryClassEntryImpl( const CPPModel::ASTDictionary&				dictionary,
			 	 	   	     	  const CPPModel::UID&							uid,
			 	 	   	     	  const std::string&							name,
			 	 	   	     	  const CPPModel::Namespace&					enclosingNamespace,
			 	 	   	     	  const CPPModel::SourceLocation&				sourceLocation,
			 	 	   	     	  const CPPModel::CompilerSpecific&				compilerSpecific,
			 	 	   	     	  CPPModel::ConstListPtr<CPPModel::Attribute>	attributeList,
			 	 	   	     	  CPPModel::TypeSpecifier						typeSpec,
			 	 	   	     	  const tree&									treeNode )
				: CPPModel::DictionaryClassEntry( (CPPModel::ASTDictionary&)dictionary, uid, name, enclosingNamespace, sourceLocation, compilerSpecific, attributeList, typeSpec ),
				  DictionaryTreeMixin( treeNode )
			{}


		bool							GetClassDefinition( const CPPModel::ParseOptions&							options,
															std::unique_ptr<const CPPModel::ClassDefinition>&		classDef ) const;
	};



	class DictionaryUnionEntryImpl : public CPPModel::DictionaryUnionEntry, public DictionaryTreeMixin
	{
	public :

		DictionaryUnionEntryImpl( const CPPModel::ASTDictionary&				dictionary,
			 	 	   	     	  const CPPModel::UID&							uid,
			 	 	   	     	  const std::string&							name,
			 	 	   	     	  const CPPModel::Namespace&					enclosingNamespace,
			 	 	   	     	  const CPPModel::SourceLocation&				sourceLocation,
			 	 	   	     	  const CPPModel::CompilerSpecific&				compilerSpecific,
			 	 	   	     	  CPPModel::ConstListPtr<CPPModel::Attribute>	attributes,
			 	 	   	     	  CPPModel::TypeSpecifier						typeSpec,
			 	 	   	     	  const tree&									treeNode )
				: CPPModel::DictionaryUnionEntry( (CPPModel::ASTDictionary&)dictionary, uid, name, enclosingNamespace, false, sourceLocation, compilerSpecific, attributes, typeSpec ),
				  DictionaryTreeMixin( treeNode )
			{}


		bool		GetUnionDefinition( const CPPModel::ParseOptions&							options,
										std::unique_ptr<const CPPModel::UnionDefinition>&		unionDef ) const;
	};




	class DictionaryFunctionEntryImpl : public CPPModel::DictionaryFunctionEntry, public DictionaryTreeMixin
	{
	public :

		DictionaryFunctionEntryImpl( const CPPModel::ASTDictionary&					dictionary,
			 	 	   	     	     const CPPModel::UID&							uid,
			 	 	   	     	     const std::string&								name,
			 	 	   	     	     const CPPModel::Namespace&						enclosingNamespace,
			 	 	   	     	     const CPPModel::SourceLocation&				sourceLocation,
				 	 	   	     	  const CPPModel::CompilerSpecific&				compilerSpecific,
			 	 	   	     	     CPPModel::ConstListPtr<CPPModel::Attribute>	attributes,
			 	 	   	     	     CPPModel::TypeSpecifier						returnTypeSpec,
			 	 	   	     	     const bool										hiddenFriend,
			 	 	   	     	     const tree&									treeNode )
				: CPPModel::DictionaryFunctionEntry( (CPPModel::ASTDictionary&)dictionary, uid, name, enclosingNamespace, false, sourceLocation, compilerSpecific, attributes, returnTypeSpec, hiddenFriend ),
				  DictionaryTreeMixin( treeNode )
			{}


		bool		GetFunctionDefinition( const CPPModel::ParseOptions&							options,
										   std::unique_ptr<const CPPModel::FunctionDefinition>&		functionDef ) const;

	};


	class DictionaryGlobalVarEntryImpl : public CPPModel::DictionaryGlobalVarEntry, public DictionaryTreeMixin
	{
	public :

		DictionaryGlobalVarEntryImpl( const CPPModel::ASTDictionary&				dictionary,
			 	 	   	     	      const CPPModel::UID&							uid,
			 	 	   	     	      const std::string&							name,
			 	 	   	     	      const CPPModel::Namespace&					enclosingNamespace,
			 	 	   	     	      bool											isStatic,
			 	 	   	     	      const CPPModel::SourceLocation&				sourceLocation,
				 	 	   	     	  const CPPModel::CompilerSpecific&				compilerSpecific,
			 	 	   	     	      CPPModel::ConstListPtr<CPPModel::Attribute>	attributes,
			 	 	   	     	      CPPModel::TypeSpecifier						typeSpec,
			 	 	   	     	      const tree&									treeNode )
				: CPPModel::DictionaryGlobalVarEntry( (CPPModel::ASTDictionary&)dictionary, uid, name, enclosingNamespace, isStatic, sourceLocation, compilerSpecific, attributes, typeSpec ),
				  DictionaryTreeMixin( treeNode )
			{}


		bool		GetGlobalVarEntry( const CPPModel::ParseOptions&							options,
									   std::unique_ptr<const CPPModel::GlobalVarEntry>&			globalVarEntry ) const;

	};


	class NestedNamespaceImpl : public CPPModel::NestedNamespace, public DictionaryTreeMixin
	{
	public :

		NestedNamespaceImpl( const CPPModel::UID&								uid,
				   	   	     const std::string&									name,
				   	   	     const CPPModel::Namespace&							enclosingNamespace,
				   	   	     const CPPModel::SourceLocation&					sourceLocation,
				   	   	     const CPPModel::CompilerSpecific&					compilerSpecificAttr,
				   	   	     CPPModel::ConstListPtr<CPPModel::Attribute>&		attributes,
				   	   	     const tree&										treeNode )
			: CPPModel::NestedNamespace( name, uid, sourceLocation, compilerSpecificAttr, attributes, enclosingNamespace ),
			  DictionaryTreeMixin( treeNode )
			{}
	};


	class GlobalNamespaceImpl : public CPPModel::GlobalNamespace, public DictionaryTreeMixin
	{
	public :

		GlobalNamespaceImpl()
			: DictionaryTreeMixin( global_namespace )
			{}
	};





	enum class DecodeNodeResultCodes { SUCCESS,
									   NULL_NODE,
									   UNRECOGNIZED_NODE_TYPE,
									   NOT_A_CLASS,
									   ERROR_DECODING_CLASS,
									   NOT_A_UNION,
									   ERROR_DECODING_UNION,
									   NOT_A_GLOBAL_VARIABLE,
									   GLOBAL_VARIABLE_IS_ARTIFICIAL,
									   ERROR_DECODING_GLOBAL_VARIABLE,
									   NOT_A_FUNCTION,
									   ARTIFICIAL_OR_BUILTIN_FUNCTION,
									   ERROR_DECODING_FUNCTION };

	typedef SEFUtility::ResultWithReturnPtr<DecodeNodeResultCodes, CPPModel::DictionaryEntry>		DecodeNodeResult;





	class ASTDictionaryImpl : public CPPModel::ASTDictionary
	{
	public :

		ASTDictionaryImpl();

		virtual ~ASTDictionaryImpl()
		{}


		//	Overrides of ASTDictionary


		void									CreateNamespace( const std::string&											namespaceToAdd );

		void									CreateGlobalVar( std::unique_ptr<const CPPModel::GlobalVarDeclaration>&		globalDeclToAdd );



		//	Public methods needed within the GCCInternalsTools compilation unit


		void									Build();

		DecodeNodeResult						DecodeASTNode( const tree&													ASTNode );


		void									DeclareGlobals();

		CPPModel::CreateNamespaceResult			CreateNamespaceInternal( const std::string&														namespaceToAdd );

		CPPModel::CreateGlobalVarResult			CreateGlobalFundamentalTypeVar( const CPPModel::FundamentalGlobalVarDeclarationBase&			globalDecl );

		CPPModel::CreateGlobalVarResult			CreateGlobalClassInstanceVar( const CPPModel::ClassGlobalVarDeclaration&						globalDecl );





		class ASTModificationEvent
		{
		public :

			enum class EventType { CREATE_NAMESPACE };

			virtual ~ASTModificationEvent()
			{}


			virtual EventType			Type() const= 0;


			virtual void				Execute( ASTDictionaryImpl&		astDictionary ) = 0;
		};


	private :


		void									collectFunction( const tree&						currentDecl );


		void									AddFQNamespace( const NamespaceTree&				fqNamespace );


		DecodeNodeResult						DecodeClass( const tree&							classTree );

		DecodeNodeResult						DecodeUnion( const tree&							unionNode );

		DecodeNodeResult						DecodeGlobalVar( const tree&						globalVarNode );

		DecodeNodeResult						DecodeFunction( const tree&							functionNode );



		boost::ptr_list<ASTModificationEvent>	m_namespaceCreationEventList;
		boost::ptr_list<ASTModificationEvent>	m_globalVarDeclarationEventList;

	};






	bool					ConvertAttribute( const tree&										identifier,
											  const tree&										arguments,
											  std::unique_ptr<const CPPModel::Attribute>&		attribute );

}


#endif /* GCCINTERNALSTOOLS_H */
