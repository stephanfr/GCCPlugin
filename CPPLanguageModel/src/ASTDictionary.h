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


#include <boost/ptr_container/ptr_map.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/composite_key.hpp>

#include "ListAliases.h"

#include "Serialization.h"
#include "CompilerSpecific.h"
#include "Attribute.h"
#include "UID.h"
#include "SourceLocation.h"
#include "Namespace.h"
#include "Types.h"
#include "Declarations.h"

#include "GlobalVar.h"
#include "Function.h"
#include "Template.h"
#include "Union.h"
#include "Class.h"

#include "Utility/Result.h"



namespace CPPModel
{

	class ParseOptions
	{
	public :

		bool	includeCompilerGeneratedFunctionArguments = false;
		bool	includeClonedFunctions = false;
	};


	class ASTDictionary;


	class DictionaryEntry : public IXMLSerializable, public IAttributes, public ICompilerSpecific
	{
	public :

		enum class EntryKind { CLASS, UNION, FUNCTION, GLOBAL_VAR, TEMPLATE, UNRECOGNIZED };


		DictionaryEntry( const ASTDictionary&			dictionary,
				   	     const UID&						uid,
				   	     const std::string&				name,
				   	     const Namespace&				enclosingNamespace,
				   	     bool							isStatic,
				   	     bool							isExtern,
				   	     const SourceLocation&			sourceLocation,
				   	     const CompilerSpecific&		compilerSpecific,
				   	     ConstListPtr<Attribute>&		attributes );


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

		const Namespace&				enclosingNamespace() const
		{
			return( m_enclosingNamespace );
		}

		const std::string&				fullyQualifiedName() const
		{
			return( m_fqName );
		}

		const bool						isStatic() const
		{
			return( m_static );
		}

		const bool						isExtern() const
		{
			return( m_extern );
		}

		const SourceLocation&			sourceLocation() const
		{
			return( m_sourceLocation );
		}

		const CompilerSpecific&			compilerSpecific() const
		{
			return( m_compilerSpecific );
		}

		const Attributes&				attributes() const
		{
			return( m_attributes );
		}

		bool operator<( const DictionaryEntry&	entryToCompare ) const
		{
			return( m_uid < entryToCompare.m_uid );
		}

		virtual std::ostream&	toXML( std::ostream&			outputStream,
									   SerializationOptions		options ) const;

	private :

		friend class ASTDictionary;

		const ASTDictionary&			m_dictionary;

		const CPPModel::UID				m_uid;

		const std::string				m_name;

		const SourceLocation			m_sourceLocation;

		const CompilerSpecific			m_compilerSpecific;

		const Attributes				m_attributes;

		const bool						m_static;

		const bool						m_extern;

		const Namespace&				m_enclosingNamespace;
		const std::string				m_enclosingNamespaceFQName;

		const std::string				m_fqName;
	};




	enum class GetClassDefinitionResultCodes { SUCCESS,
											   INTERNAL_ERROR };

	typedef SEFUtility::ResultWithUniqueReturnPtr<GetClassDefinitionResultCodes, ClassDefinition>				GetClassDefinitionResult;



	class DictionaryClassEntry : public DictionaryEntry
	{
	public :

		DictionaryClassEntry( const ASTDictionary&				dictionary,
				   	    	  const UID&						uid,
				   	    	  const std::string&				name,
				   	    	  const Namespace&					enclosingNamespace,
						   	  bool								isExtern,
				   	    	  const SourceLocation&				sourceLocation,
						   	  const CompilerSpecific&			compilerSpecific,
				   	    	  ConstListPtr<Attribute>&			attributeList,
				   	    	  TypeSpecifier						typeSpec )
			: DictionaryEntry( dictionary, uid, name, enclosingNamespace, false, isExtern, sourceLocation, compilerSpecific, attributeList ),
			  m_typeSpec( typeSpec )
			{}


		virtual ~DictionaryClassEntry()
		{}


		const DictionaryEntry::EntryKind		entryKind() const
		{
			return( DictionaryEntry::EntryKind::CLASS );
		}


		const TypeSpecifier						typeSpec() const
		{
			return( m_typeSpec );
		}


		const ClassOrStructType					type() const
		{
			ConstListPtr<Attribute>		attributesCopy( Attributes::deepCopy( attributes().attributes() ) );

			return( ClassOrStructType( TypeSpecifier::CLASS,
									   name(),
									   uid(),
									   enclosingNamespace(),
									   sourceLocation(),
									   attributesCopy ) );
		}


		const bool				isStatic() const = delete;


		virtual GetClassDefinitionResult			GetClassDefinition( const CPPModel::ParseOptions&							options ) const = 0;



	private :

		const TypeSpecifier			m_typeSpec;
	};




	class DictionaryUnionEntry : public DictionaryEntry
	{
	public :

		DictionaryUnionEntry( const ASTDictionary&			dictionary,
				   	    	  const UID&					uid,
				   	    	  const std::string&			name,
				   	    	  const Namespace&				enclosingNamespace,
						   	  bool							isStatic,
						   	  bool							isExtern,
				   	    	  const SourceLocation&			sourceLocation,
						   	  const CompilerSpecific&		compilerSpecific,
				   	    	  ConstListPtr<Attribute>&		attributeList,
				   	    	  TypeSpecifier					typeSpec )
			: DictionaryEntry( dictionary, uid, name, enclosingNamespace, isStatic, isExtern, sourceLocation, compilerSpecific, attributeList ),
			  m_typeSpec( typeSpec )
			{}


		virtual ~DictionaryUnionEntry()
		{}


		const DictionaryEntry::EntryKind		entryKind() const
		{
			return( DictionaryEntry::EntryKind::UNION );
		}


		const CPPModel::TypeSpecifier			typeSpec() const
		{
			return( m_typeSpec );
		}


		const UnionType		type() const
		{
			ConstListPtr<Attribute>		attributesCopy( Attributes::deepCopy( attributes().attributes() ) );

			return( UnionType( TypeSpecifier::CLASS,
							   name(),
							   uid(),
							   enclosingNamespace(),
							   sourceLocation(),
							   attributesCopy ) );
		}


		virtual bool			GetUnionDefinition( const CPPModel::ParseOptions&							options,
													std::unique_ptr<const CPPModel::UnionDefinition>&		unionDef ) const = 0;


	private :

		const TypeSpecifier			m_typeSpec;
	};




	class DictionaryFunctionEntry : public DictionaryEntry
	{
	public :

		DictionaryFunctionEntry( const ASTDictionary&			dictionary,
				   	    		 const UID&						uid,
				   	    		 const std::string&				name,
				   	    		 const Namespace&				enclosingNamespace,
						   	     bool							isStatic,
						   	     bool							isExtern,
				   	    		 const SourceLocation&			sourceLocation,
						   	     const CompilerSpecific&		compilerSpecific,
				   	    		 ConstListPtr<Attribute>&		attributeList,
				   	    		 TypeSpecifier					returnTypeSpec,
				   	    		 const bool						hiddenFriend )
			: DictionaryEntry( dictionary, uid, name, enclosingNamespace, isStatic, isExtern, sourceLocation, compilerSpecific, attributeList ),
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


		const CPPModel::TypeSpecifier			returnTypeSpec() const
		{
			return( m_returnTypeSpec );
		}


		virtual bool		GetFunctionDefinition( const CPPModel::ParseOptions&							options,
												   std::unique_ptr<const CPPModel::FunctionDefinition>&		functionDef ) const = 0;

	private :

		const TypeSpecifier			m_returnTypeSpec;

		const bool					m_hiddenFriend;
	};



	class DictionaryGlobalVarEntry : public DictionaryEntry
	{
	public :

		DictionaryGlobalVarEntry( const ASTDictionary&			dictionary,
				   	    		  const UID&					uid,
				   	    		  const std::string&			name,
				   	    		  const Namespace&				enclosingNamespace,
				   	    		  bool							isStatic,
							   	  bool							isExtern,
				   	    		  const SourceLocation&			sourceLocation,
							   	  const CompilerSpecific&		compilerSpecific,
				   	    		  ConstListPtr<Attribute>&		attributes,
				   	    		  TypeSpecifier					typeSpec )
			: DictionaryEntry( dictionary, uid, name, enclosingNamespace, isStatic, isExtern, sourceLocation, compilerSpecific, attributes ),
			  m_typeSpec( typeSpec )
			{}


		virtual ~DictionaryGlobalVarEntry()
		{}


		const DictionaryEntry::EntryKind	entryKind() const
		{
			return( DictionaryEntry::EntryKind::GLOBAL_VAR );
		}


		const CPPModel::TypeSpecifier		typeSpec() const
		{
			return( m_typeSpec );
		}


		virtual bool		GetGlobalVarEntry( const CPPModel::ParseOptions&						options,
											   std::unique_ptr<const CPPModel::GlobalVarEntry>&		globalVarEntry ) const = 0;

	private :

		const TypeSpecifier			m_typeSpec;
	};




	enum class CreateNamespaceResultCodes { SUCCESS,
											NAMESPACE_ALREADY_EXISTS };

	typedef SEFUtility::Result<CreateNamespaceResultCodes>													CreateNamespaceResult;



	enum class CreateGlobalVarResultCodes { SUCCESS,
											UNRECOGNIZED_TYPE_TO_CREATE,
											MISMATCH_OF_NHUMBER_OF_INITIAL_VALUES_AND_GLOBAL_VAR,
											MISMATCH_OF_VAR_TYPE_AND_INITIAL_VALUE,
											ERROR_ADDING_GLOBAL_TO_DICTIONARY,
											NAME_ALREADY_EXISTS,
											WRONG_FUNCTION_FOR_GLOBAL_CREATION,
											UNABLE_TO_FIND_INITIALIZATION_INSERTION_POINT,
											UNABLE_TO_FIND_CORRECT_CONSTRUCTOR,
											INTERNAL_ERROR };

	typedef SEFUtility::ResultWithReturnValue<CreateGlobalVarResultCodes,CPPModel::UID>						CreateGlobalVarResult;



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


		typedef boost::ptr_map<const std::string, Namespace >						NamespaceMap;
		typedef boost::ptr_map<const std::string, Namespace >::const_iterator		NamespaceMapConstIterator;


		//	The following typedef is for use in the code to keep the Eclipse IDE from complaining when iterators are de-referenced

		typedef std::shared_ptr<DictionaryEntry>	DictionaryEntryPtr;

	protected :

	//	This may look a bit odd, but without the #define for BMI and the typedefs for the indices, the declaration for DictionaryType would be long and
	//		indecipherable.  Also, the syntax checker in Eclipse has trouble digesting the whole thing when expressed as a single typedef.
	//
	//	The namespace index is non-unique so to insure ordering is consistent between runs, we will use a composite key of namespace and entry name.
	//		Source Location is also non-unique but I don't think ordering there is as critical.


	#define BMI boost::multi_index

		typedef BMI::ordered_unique< BMI::tag<Indices::Identity>, BMI::identity<DictionaryEntry> >																		__Identity__;
		typedef BMI::ordered_unique< BMI::tag<Indices::UID>, BMI::member<DictionaryEntry, const CPPModel::UID, &DictionaryEntry::m_uid> > 								__UID__;
		typedef BMI::ordered_non_unique< BMI::tag<Indices::Namespace>, BMI::composite_key< DictionaryEntry,
																		BMI::member<DictionaryEntry, const std::string, &DictionaryEntry::m_enclosingNamespaceFQName>,
																		BMI::member<DictionaryEntry, const std::string, &DictionaryEntry::m_name> > >					__Namespace__;
		typedef BMI::ordered_unique< BMI::tag<Indices::FQName>, BMI::member<DictionaryEntry, const std::string, &DictionaryEntry::m_fqName> >							__FQName__;
		typedef BMI::ordered_non_unique< BMI::tag<Indices::Location>, BMI::member<DictionaryEntry, const SourceLocation, &DictionaryEntry::m_sourceLocation> >			__SourceLocation__;

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
			: m_dictionary( new DictionaryType() ),
			  m_namespaces( new boost::ptr_map<const std::string, Namespace>() )
		{}

		virtual ~ASTDictionary()
		{}



		virtual CPPModel::CreateNamespaceResult			CreateNamespace( const std::string&								namespaceToAdd ) = 0;

		virtual CPPModel::CreateGlobalVarResult			CreateGlobalVar( const CPPModel::GlobalVarDeclaration&			globalDeclToAdd ) = 0;



		const ASTDictionary::IdentityIndex&				IdentityIdx() const
		{
			return( m_dictionary->get<Indices::Identity>() );
		}

		const ASTDictionary::UIDIndex&					UIDIdx() const
		{
			return( m_dictionary->get<Indices::UID>() );
		}

		const ASTDictionary::NamespaceIndex&			NamespaceIdx() const
		{
			return( m_dictionary->get<Indices::Namespace>() );
		}

		const ASTDictionary::FQNameIndex&				FQNameIdx() const
		{
			return( m_dictionary->get<Indices::FQName>() );
		}

		const ASTDictionary::LocationIndex&				LocationIdx() const
		{
			return( m_dictionary->get<Indices::Location>() );
		}


		bool		Insert( std::shared_ptr<DictionaryEntry>&		entryToAdd );


		bool		AddNamespace( Namespace*	namespaceToAdd )
		{
			std::pair<boost::ptr_map<const std::string, Namespace>::const_iterator, bool>		insertResult = m_namespaces->insert( namespaceToAdd->fqName(), namespaceToAdd );

			return( insertResult.second );
		}

		bool		ContainsNamespace( const std::string&		fullyQualifiedName ) const
		{
			return( m_namespaces->find( fullyQualifiedName ) != m_namespaces->end() );
		}

		bool		GetNamespace( const std::string&		fullyQualifiedName,
								  const Namespace*&			namespaceEntry ) const
		{
			boost::ptr_map<const std::string, Namespace>::const_iterator		itrNamespace = m_namespaces->find( fullyQualifiedName );

			if( itrNamespace != m_namespaces->end() )
			{
				namespaceEntry = (*itrNamespace).second;

				return( true );
			}

			namespaceEntry = NULL;

			return( false );
		}

		const NamespaceMap&			namespaces() const
		{
			return( *m_namespaces );
		}





		enum class XMLOutputOptions : std::int64_t { NONE = 0, LIST_NAMESPACES = 1 };



		void			DumpASTXMLByNamespaces( std::ostream&					outputStream,
				  	  	  	  	  	  	  	  	std::list<std::string>&			namespacesToDump,
				  	  	  	  	  	  	  	  	XMLOutputOptions				outputOptions = XMLOutputOptions::NONE ) const;

		void			DumpDictionaryXMLByNamespace( std::ostream&				outputStream,
													  const std::string&		namespaceToDump ) const;

		void			DumpElementsXMLByNamespace( std::ostream&				outputStream,
				  	  	  	  	  	  	  	  	   const std::string&			namespaceToDump ) const;

		void			DumpNamespaces( std::ostream&							outputStream ) const;


	protected :

		std::shared_ptr<DictionaryType>										m_dictionary;

		std::unique_ptr<boost::ptr_map<const std::string, Namespace>>		m_namespaces;
	};

}	//	namespace CPPModel


#endif /* ASTDICTIONARY_H_ */
