/*
 * ASTDictionary.cpp
 *
 *  Created on: Dec 31, 2013
 *      Author: steve
 */



#include "ASTDictionary.h"

#include <ostream>

#include "Utility/IndentingOutputStreambuf.h"





namespace CPPModel
{


	void			ASTDictionary::DumpASTXMLByNamespaces( std::ostream&					outputStream,
		  	  	  	  	  	  	  	  					   std::list<std::string>&			namespacesToDump,
		  					  	  	  	  	  	  	  	   XMLOutputOptions					outputOptions ) const
	{
		outputStream << "<ast>\n";

		if( (std::int64_t)outputOptions && (std::int64_t)XMLOutputOptions::LIST_NAMESPACES )
		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			outputStream << "<namespaces>\n";

			{
				SEFUtility::IndentingOutputStreambuf		indent( outputStream );

				for( CPPModel::ASTDictionary::NamespaceMapConstIterator itrNamespace = namespaces().begin(); itrNamespace != namespaces().end(); itrNamespace++ )
				{
					itrNamespace->second->toXML( outputStream, CPPModel::SerializationOptions::NONE );
				}
			}

			outputStream << "</namespaces>\n";
		}

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			outputStream << "<dictionary>\n";

			{
				SEFUtility::IndentingOutputStreambuf		indent( outputStream );

				for( std::string currentNamespace : namespacesToDump )
				{
					DumpDictionaryXMLByNamespace( outputStream, currentNamespace );
				}
			}
			outputStream << "</dictionary>\n";

			outputStream << "<elements>\n";

			{
				SEFUtility::IndentingOutputStreambuf		indent( outputStream );

				for( std::string currentNamespace : namespacesToDump )
				{
					DumpElementsXMLByNamespace( outputStream, currentNamespace );
				}
			}

			outputStream << "</elements>\n";
		}

		outputStream << "</ast>\n";
	}


	void			ASTDictionary::DumpDictionaryXMLByNamespace( std::ostream&				outputStream,
			  	  	  	  	  	  	  	  	  	  	  	  	     const std::string&			namespaceToDump ) const
	{
		//	Return immediately if the namespace does not exist

		if( !ContainsNamespace( namespaceToDump ) )
		{
			return;
		}

		outputStream << "<namespace name=\"" << namespaceToDump << "\">\n";

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			for( CPPModel::ASTDictionary::NamespaceIndexConstIterator namespaceIndex = NamespaceIdx().lower_bound( namespaceToDump ); namespaceIndex != NamespaceIdx().upper_bound( namespaceToDump ); namespaceIndex++ )
			{
				(*namespaceIndex)->toXML( outputStream, CPPModel::SerializationOptions::NONE );
			}
		}

		outputStream << "</namespace>\n";
	}



	void			ASTDictionary::DumpElementsXMLByNamespace( std::ostream&				outputStream,
			  	  	  	  	  	  	  	  	  	   	   	       const std::string&			namespaceToDump ) const
	{
		//	Return immediately if the namespace does not exist

		if( !ContainsNamespace( namespaceToDump ) )
		{
			return;
		}

		outputStream << "<namespace name=\"" << namespaceToDump << "\">\n";

		{
			SEFUtility::IndentingOutputStreambuf		indent( outputStream );

			CPPModel::ParseOptions		parseOptions;

			for( CPPModel::ASTDictionary::NamespaceIndexConstIterator namespaceIndex = NamespaceIdx().lower_bound( namespaceToDump ); namespaceIndex != NamespaceIdx().upper_bound( namespaceToDump ); namespaceIndex++ )
			{
				if( (*namespaceIndex)->entryKind() == CPPModel::DictionaryEntry::EntryKind::CLASS )
				{
					std::unique_ptr<const CPPModel::ClassDefinition>		classDef;

					((CPPModel::DictionaryClassEntry*)&(**namespaceIndex))->GetClassDefinition( parseOptions, classDef );

					classDef->toXML( outputStream, CPPModel::SerializationOptions::NONE );
				}
				else if( (*namespaceIndex)->entryKind() == CPPModel::DictionaryEntry::EntryKind::UNION )
				{
					std::unique_ptr<const CPPModel::UnionDefinition>		unionDef;

					((CPPModel::DictionaryUnionEntry*)&(**namespaceIndex))->GetUnionDefinition( parseOptions, unionDef );

					unionDef->toXML( outputStream, CPPModel::SerializationOptions::NONE );
				}
				else if( (*namespaceIndex)->entryKind() == CPPModel::DictionaryEntry::EntryKind::FUNCTION )
				{
					std::unique_ptr<const CPPModel::FunctionDefinition>		functionDef;

					if( ((CPPModel::DictionaryFunctionEntry*)&(**namespaceIndex))->GetFunctionDefinition( parseOptions, functionDef ))
					{
						functionDef->toXML( outputStream, CPPModel::SerializationOptions::NONE );
					}
				}
				else if( (*namespaceIndex)->entryKind() == CPPModel::DictionaryEntry::EntryKind::GLOBAL_VAR )
				{
					std::unique_ptr<const CPPModel::GlobalVarEntry>		globalVarDef;

					((CPPModel::DictionaryGlobalVarEntry*)&(**namespaceIndex))->GetGlobalVarEntry( parseOptions, globalVarDef );

					globalVarDef->toXML( outputStream, CPPModel::SerializationOptions::NONE );
				}
			}
		}

		outputStream << "</namespace>\n";
	}

}


