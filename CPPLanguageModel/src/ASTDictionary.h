/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/


#ifndef ASTDICTIONARY_H_
#define ASTDICTIONARY_H_



namespace CPPModel
{

	class ParseOptions
	{
	public :

		bool	includeCompilerGeneratedFunctionArguments = false;
		bool	includeClonedFunctions = false;
	};


	class ASTDictionary;

	class DictionaryEntry : public virtual XMLSerializable, public Attributes
	{
	public :

		enum class EntryKind { CLASS, UNION, FUNCTION, GLOBAL_VAR, TEMPLATE, UNRECOGNIZED };


		DictionaryEntry( const ASTDictionary&			dictionary,
				   	     const CPPModel::UID&			uid,
				   	     const std::string&				name,
				   	     const std::string&				enclosingNamespace,
				   	     const SourceLocation&			sourceLocation,
				   	     ConstListPtr<Attribute>&		attributes )
			: Attributes( attributes ),
			  m_dictionary( dictionary ),
			  m_uid( uid ),
			  m_name( name ),
			  m_enclosingNamespace( enclosingNamespace ),
			  m_sourceLocation( sourceLocation ),
			  m_fqName( std::string( enclosingNamespace ) + std::string( name ) )
			{}


		virtual ~DictionaryEntry()
		{}


		virtual const EntryKind			entryKind() const = 0;

		const ASTDictionary&			Dictionary() const
		{
			return( m_dictionary );
		}

		const CPPModel::UID&			uid() const
		{
			return( m_uid );
		}

		const std::string&				name() const
		{
			return( m_name );
		}

		const std::string&				enclosingNamespace() const
		{
			return( m_enclosingNamespace );
		}

		const std::string&				fullyQualifiedName() const
		{
			return( m_fqName );
		}

		const SourceLocation&			sourceLocation() const
		{
			return( m_sourceLocation );
		}

		bool operator<( const DictionaryEntry&	entryToCompare ) const
		{
			return( m_uid < entryToCompare.m_uid );
		}

		virtual std::ostream&	toXML( std::ostream&			outputStream,
									   int						indentLevel,
									   SerializationOptions		options ) const;

	private :

		friend class ASTDictionary;

		const ASTDictionary&			m_dictionary;

		const CPPModel::UID				m_uid;

		const std::string				m_name;
		const std::string				m_enclosingNamespace;

		const SourceLocation			m_sourceLocation;

		const std::string				m_fqName;
	};



	class DictionaryClassEntry : public DictionaryEntry
	{
	public :

		DictionaryClassEntry( const ASTDictionary&				dictionary,
				   	    	  const CPPModel::UID&				uid,
				   	    	  const std::string&				name,
				   	    	  const std::string&				enclosingNamespace,
				   	    	  const SourceLocation&				sourceLocation,
				   	    	  ConstListPtr<Attribute>&			attributeList,
				   	    	  TypeInfo::Specifier				typeSpec )
			: DictionaryEntry( dictionary, uid, name, enclosingNamespace, sourceLocation, attributeList ),
			  m_typeSpec( typeSpec )
			{}


		virtual ~DictionaryClassEntry()
		{}


		const DictionaryEntry::EntryKind		entryKind() const
		{
			return( DictionaryEntry::EntryKind::CLASS );
		}


		const TypeInfo::Specifier				typeSpec() const
		{
			return( m_typeSpec );
		}


		virtual bool		GetClassDefinition( const CPPModel::ParseOptions&							options,
												std::unique_ptr<const CPPModel::ClassDefinition>&		classDef ) const = 0;



	private :

		const TypeInfo::Specifier			m_typeSpec;
	};




	class DictionaryUnionEntry : public DictionaryEntry
	{
	public :

		DictionaryUnionEntry( const ASTDictionary&			dictionary,
				   	    	  const CPPModel::UID&			uid,
				   	    	  const std::string&			name,
				   	    	  const std::string&			enclosingNamespace,
				   	    	  const SourceLocation&			sourceLocation,
				   	    	  ConstListPtr<Attribute>&		attributeList,
				   	    	  TypeInfo::Specifier			typeSpec )
			: DictionaryEntry( dictionary, uid, name, enclosingNamespace, sourceLocation, attributeList ),
			  m_typeSpec( typeSpec )
			{}


		virtual ~DictionaryUnionEntry()
		{}


		const DictionaryEntry::EntryKind		entryKind() const
		{
			return( DictionaryEntry::EntryKind::UNION );
		}


		const CPPModel::TypeInfo::Specifier		typeSpec() const
		{
			return( m_typeSpec );
		}


		virtual bool		GetUnionDefinition( const CPPModel::ParseOptions&							options,
												std::unique_ptr<const CPPModel::UnionDefinition>&		unionDef ) const = 0;


	private :

		const TypeInfo::Specifier			m_typeSpec;
	};




	class DictionaryFunctionEntry : public DictionaryEntry
	{
	public :

		DictionaryFunctionEntry( const ASTDictionary&			dictionary,
				   	    		 const CPPModel::UID&			uid,
				   	    		 const std::string&				name,
				   	    		 const std::string&				enclosingNamespace,
				   	    		 const SourceLocation&			sourceLocation,
				   	    		 ConstListPtr<Attribute>&		attributeList,
				   	    		 TypeInfo::Specifier			returnTypeSpec,
				   	    		 const bool						hiddenFriend )
			: DictionaryEntry( dictionary, uid, name, enclosingNamespace, sourceLocation, attributeList ),
			  m_returnTypeSpec( returnTypeSpec ),
			  m_hiddenFriend( hiddenFriend )
			{}


		virtual ~DictionaryFunctionEntry()
		{}


		const bool								isHiddenFriend() const
		{
			return( m_hiddenFriend );
		}

		const DictionaryEntry::EntryKind		entryKind() const
		{
			return( DictionaryEntry::EntryKind::FUNCTION );
		}


		const CPPModel::TypeInfo::Specifier		returnTypeSpec() const
		{
			return( m_returnTypeSpec );
		}


		virtual bool		GetFunctionDefinition( const CPPModel::ParseOptions&							options,
												   std::unique_ptr<const CPPModel::FunctionDefinition>&		functionDef ) const = 0;

	private :

		const TypeInfo::Specifier			m_returnTypeSpec;

		const bool							m_hiddenFriend;
	};



	class DictionaryGlobalVarEntry : public DictionaryEntry
	{
	public :

		DictionaryGlobalVarEntry( const ASTDictionary&			dictionary,
				   	    		  const CPPModel::UID&			uid,
				   	    		  const std::string&			name,
				   	    		  const std::string&			enclosingNamespace,
				   	    		  const SourceLocation&			sourceLocation,
				   	    		  ConstListPtr<Attribute>&		attributes,
				   	    		  TypeInfo::Specifier			typeSpec )
			: DictionaryEntry( dictionary, uid, name, enclosingNamespace, sourceLocation, attributes ),
			  m_typeSpec( typeSpec )
			{}


		virtual ~DictionaryGlobalVarEntry()
		{}


		const DictionaryEntry::EntryKind		entryKind() const
		{
			return( DictionaryEntry::EntryKind::GLOBAL_VAR );
		}


		const CPPModel::TypeInfo::Specifier		typeSpec() const
		{
			return( m_typeSpec );
		}


		virtual bool		GetGlobalVarDefinition( const CPPModel::ParseOptions&								options,
												    std::unique_ptr<const CPPModel::GlobalVarDefinition>&		globalVarDef ) const = 0;

	private :

		const TypeInfo::Specifier			m_typeSpec;
	};




	class ASTDictionary
	{
	public :

		struct Indices
		{
			struct Identity {};
			struct UID {};
			struct Namespace {};
			struct FQName {};
			struct Location {};
		};

		//	The following typedef is for use in the code to keep the Eclipse IDE from complaining when iterators are de-referenced

		typedef std::shared_ptr<DictionaryEntry>	DictionaryEntryPtr;

	private :

	//	This may look a bit odd, but without the #define for BMI and the typedefs for the indices, the declaration for DictionaryType would be long and
	//		indecipherable.  Also, the syntax checker in Eclipse has trouble digesting the whole thing when expressed as a single typedef.

	#define BMI boost::multi_index

		typedef BMI::ordered_unique< BMI::tag<Indices::Identity>, BMI::identity<DictionaryEntry> >																	__Identity__;
		typedef BMI::ordered_unique< BMI::tag<Indices::UID>, BMI::member<DictionaryEntry, const CPPModel::UID, &DictionaryEntry::m_uid> > 							__UID__;
		typedef BMI::ordered_non_unique< BMI::tag<Indices::Namespace>, BMI::member<DictionaryEntry, const std::string, &DictionaryEntry::m_enclosingNamespace> >	__Namespace__;
		typedef BMI::ordered_unique< BMI::tag<Indices::FQName>, BMI::member<DictionaryEntry, const std::string, &DictionaryEntry::m_fqName> >						__FQName__;
		typedef BMI::ordered_non_unique< BMI::tag<Indices::Location>, BMI::member<DictionaryEntry, const SourceLocation, &DictionaryEntry::m_sourceLocation> >		__SourceLocation__;

		typedef	BMI::multi_index_container< DictionaryEntryPtr, BMI::indexed_by< __Identity__, __UID__, __Namespace__, __FQName__, __SourceLocation__ > > DictionaryType;

	#undef BMI

	public :

		typedef DictionaryType::const_iterator										constIterator;

		typedef DictionaryType::index<Indices::Identity>::type						IdentityIndex;
		typedef DictionaryType::index<Indices::Identity>::type::const_iterator		IdentityIndexConstIterator;

		typedef DictionaryType::index<Indices::UID>::type							UIDIndex;
		typedef DictionaryType::index<Indices::UID>::type::const_iterator			UIDIndexConstIterator;

		typedef DictionaryType::index<Indices::Namespace>::type						NamespaceIndex;
		typedef DictionaryType::index<Indices::Namespace>::type::const_iterator		NamespaceIndexConstIterator;

		typedef DictionaryType::index<Indices::FQName>::type						FQNameIndex;
		typedef DictionaryType::index<Indices::FQName>::type::const_iterator		FQNameIndexConstIterator;

		typedef DictionaryType::index<Indices::Location>::type						LocationIndex;
		typedef DictionaryType::index<Indices::Location>::type::const_iterator		LocationIndexConstIterator;




		ASTDictionary()
			: m_dictionary( new DictionaryType() )
		{}

		virtual ~ASTDictionary()
		{}


		virtual void		Build() = 0;


		const ASTDictionary::IdentityIndex&			IdentityIdx() const
		{
			return( m_dictionary->get<Indices::Identity>() );
		}

		const ASTDictionary::UIDIndex&				UIDIdx() const
		{
			return( m_dictionary->get<Indices::UID>() );
		}

		const ASTDictionary::NamespaceIndex&		NamespaceIdx() const
		{
			return( m_dictionary->get<Indices::Namespace>() );
		}

		const ASTDictionary::FQNameIndex&			FQNameIdx() const
		{
			return( m_dictionary->get<Indices::FQName>() );
		}

		const ASTDictionary::LocationIndex&			LocationIdx() const
		{
			return( m_dictionary->get<Indices::Location>() );
		}


		bool		insert( DictionaryEntry*		entryToAdd )
		{
			std::pair<CPPModel::ASTDictionary::constIterator,bool> insertResult = m_dictionary->insert( std::shared_ptr<CPPModel::DictionaryEntry>( entryToAdd ) );

			return( insertResult.second );
		}




	protected :

		std::shared_ptr<DictionaryType>		m_dictionary;
	};

}	//	namespace CPPModel


#endif /* ASTDICTIONARY_H_ */
