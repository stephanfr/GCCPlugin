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





namespace GCCInternalsTools
{



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
			 	 	   	     	  const std::string&							enclosingNamespace,
			 	 	   	     	  const CPPModel::SourceLocation&				sourceLocation,
			 	 	   	     	  CPPModel::ConstListPtr<CPPModel::Attribute>	attributeList,
			 	 	   	     	  CPPModel::TypeInfo::Specifier					typeSpec,
			 	 	   	     	  const tree&									treeNode )
				: CPPModel::DictionaryClassEntry( (CPPModel::ASTDictionary&)dictionary, uid, name, enclosingNamespace, sourceLocation, attributeList, typeSpec ),
				  DictionaryTreeMixin( treeNode )
			{}


		bool		GetClassDefinition( const CPPModel::ParseOptions&							options,
										std::unique_ptr<const CPPModel::ClassDefinition>&		classDef ) const;

	};



	class DictionaryUnionEntryImpl : public CPPModel::DictionaryUnionEntry, public DictionaryTreeMixin
	{
	public :

		DictionaryUnionEntryImpl( const CPPModel::ASTDictionary&				dictionary,
			 	 	   	     	  const CPPModel::UID&							uid,
			 	 	   	     	  const std::string&							name,
			 	 	   	     	  const std::string&							enclosingNamespace,
			 	 	   	     	  const CPPModel::SourceLocation&				sourceLocation,
			 	 	   	     	  CPPModel::ConstListPtr<CPPModel::Attribute>	attributes,
			 	 	   	     	  CPPModel::TypeInfo::Specifier					typeSpec,
			 	 	   	     	  const tree&									treeNode )
				: CPPModel::DictionaryUnionEntry( (CPPModel::ASTDictionary&)dictionary, uid, name, enclosingNamespace, false, sourceLocation, attributes, typeSpec ),
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
			 	 	   	     	     const std::string&								enclosingNamespace,
			 	 	   	     	     const CPPModel::SourceLocation&				sourceLocation,
			 	 	   	     	     CPPModel::ConstListPtr<CPPModel::Attribute>	attributes,
			 	 	   	     	     CPPModel::TypeInfo::Specifier					returnTypeSpec,
			 	 	   	     	     const bool										hiddenFriend,
			 	 	   	     	     const tree&									treeNode )
				: CPPModel::DictionaryFunctionEntry( (CPPModel::ASTDictionary&)dictionary, uid, name, enclosingNamespace, false, sourceLocation, attributes, returnTypeSpec, hiddenFriend ),
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
			 	 	   	     	      const std::string&							enclosingNamespace,
			 	 	   	     	      bool											isStatic,
			 	 	   	     	      const CPPModel::SourceLocation&				sourceLocation,
			 	 	   	     	      CPPModel::ConstListPtr<CPPModel::Attribute>	attributes,
			 	 	   	     	      CPPModel::TypeInfo::Specifier					typeSpec,
			 	 	   	     	      const tree&									treeNode )
				: CPPModel::DictionaryGlobalVarEntry( (CPPModel::ASTDictionary&)dictionary, uid, name, enclosingNamespace, isStatic, sourceLocation, attributes, typeSpec ),
				  DictionaryTreeMixin( treeNode )
			{}


		bool		GetGlobalVarEntry( const CPPModel::ParseOptions&							options,
									   std::unique_ptr<const CPPModel::GlobalVarEntry>&			globalVarEntry ) const;

	};


	class NamespaceEntryImpl : public CPPModel::NamespaceEntry, public DictionaryTreeMixin
	{
	public :

		NamespaceEntryImpl( const CPPModel::UID&							uid,
				   	    	const std::string&								name,
				   	    	const std::string&								enclosingNamespace,
				   	    	CPPModel::ConstListPtr<CPPModel::Attribute>&	attributes,
	 	 	   	     	    const tree&										treeNode )
			: CPPModel::NamespaceEntry( uid, name, enclosingNamespace, attributes ),
			  DictionaryTreeMixin( treeNode )
			{}
	};



	class ITreeDecoder
	{
	public :

		virtual ~ITreeDecoder()
		{};

		virtual bool		Decode( const tree&						treeNode,
				  	  	  	  	    CPPModel::ASTDictionary&		dictionary ) const = 0;
	};



	class ClassDecoder : public ITreeDecoder
	{
	public :

		bool		Decode( const tree&						treeNode,
	  	  	  	    		CPPModel::ASTDictionary&		dictionary ) const;
	};



	class UnionDecoder : public ITreeDecoder
	{
	public :

		bool		Decode( const tree&						treeNode,
	  	  	  	    		CPPModel::ASTDictionary&		dictionary ) const;
	};


	class FunctionDecoder : public ITreeDecoder
	{
	public :

		bool		Decode( const tree&						treeNode,
	  	  	  	    		CPPModel::ASTDictionary&		dictionary ) const;
	};


	class GlobalVarDecoder : public ITreeDecoder
	{
	public :

		bool		Decode( const tree&						treeNode,
	  	  	  	    		CPPModel::ASTDictionary&		dictionary ) const;
	};



	class ASTDictionaryImpl : public CPPModel::ASTDictionary
	{
	public :

		ASTDictionaryImpl()
		{}

		virtual ~ASTDictionaryImpl()
		{}


		void		Build();


	private :


		void 		DecodingPass( const tree&			namespaceNode,
								  ITreeDecoder&			decoder );

		void		collectFunction( const tree&		currentDecl );
	};





	bool					ConvertAttribute( const tree&										identifier,
											  const tree&										arguments,
											  std::unique_ptr<const CPPModel::Attribute>&		attribute );

}


#endif /* GCCINTERNALSTOOLS_H */
