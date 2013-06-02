/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/


#include <cassert>

#include <algorithm>
#include <cstdlib>
#include <gmp.h>
#include <map>
#include <memory>
#include <ostream>
#include <stdlib.h>
#include <string>
#include <utility>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/ptr_container/ptr_list.hpp>

#include "config.h"

#include "gcc-plugin.h"

#include "tree.h"
#include "cp/cp-tree.h"
#include "diagnostic.h"
#include "real.h"

#include "ListAliases.h"

#include "Constants.h"
#include "Serialization.h"
#include "ConstantValue.h"
#include "NamedEntity.h"
#include "Attribute.h"
#include "UID.h"
#include "SourceLocation.h"
#include "Namespace.h"
#include "Static.h"
#include "Access.h"
#include "SourceElement.h"
#include "Types.h"
#include "Union.h"
#include "Function.h"
#include "GlobalVar.h"
#include "Class.h"
#include "ASTDictionary.h"

#include "ConstantTree.h"
#include "IdentifierTree.h"
#include "DeclOrTypeBaseTree.h"

#include "TypeTree.h"
#include "DeclTree.h"

#include "TreeList.h"
#include "AttributeParser.h"




namespace GCCInternalsTools
{

	CPPModel::ConstListPtr<CPPModel::Attribute>			TypeTree::attributes()
	{
		return( GetAttributes( PurposeValueList( TYPE_ATTRIBUTES( m_tree )) ));
	}


	const std::string			TypeTree::Namespace() const
	{
		std::string			fullNamespace = "";


		if( DECL_NAMESPACE_STD_P( m_tree ))
		{
			fullNamespace = "std::";
		}
		else
		{
			for( tree& currentContext = CP_TYPE_CONTEXT( m_tree ); currentContext != global_namespace; currentContext = CP_TYPE_CONTEXT( currentContext ) )
			{
				fullNamespace += std::string( DeclTree( currentContext ).identifier() + NAMESPACE_SEPARATOR );
			}
		}

		return( fullNamespace );
	}


	CPPModel::TypeInfo::Specifier			TypeTree::typeSpecifier() const
	{
		CPPModel::TypeInfo::Specifier		returnValue = CPPModel::TypeInfo::Specifier::UNRECOGNIZED;

		//	If this is a TYPE, then use the m_tree value as-is.  If it is a DECL, then we need the TREE_TYPE.

		switch( TREE_CODE( m_tree ))
		{
			case VOID_TYPE :
				returnValue = CPPModel::TypeInfo::Specifier::VOID;
				break;

			case ENUMERAL_TYPE :
				returnValue = CPPModel::TypeInfo::Specifier::ENUM;
				break;

			case BOOLEAN_TYPE :
				returnValue = CPPModel::TypeInfo::Specifier::BOOLEAN;
				break;

			case INTEGER_TYPE :
				{
					bool			isUnsigned = TYPE_UNSIGNED( m_tree );

					const std::unique_ptr<const CPPModel::ConstantValue> typeLengthInBits( ConstantTree( TYPE_SIZE( m_tree ) ).value() );

					assert( typeLengthInBits->kind() == CPPModel::ConstantValue::Kind::INTEGER );


					switch( dynamic_cast<const CPPModel::IntegerConstant*>( typeLengthInBits.get() )->value() )
					{
						case 8 :
							returnValue = isUnsigned ? CPPModel::TypeInfo::Specifier::UNSIGNED_CHAR : CPPModel::TypeInfo::Specifier::CHAR;
							break;

						case 16 :
							returnValue = isUnsigned ? CPPModel::TypeInfo::Specifier::UNSIGNED_SHORT_INT : CPPModel::TypeInfo::Specifier::SHORT_INT;
							break;

						case 32 :
							returnValue = isUnsigned ? CPPModel::TypeInfo::Specifier::UNSIGNED_INT : CPPModel::TypeInfo::Specifier::INT;
							break;

						case 64 :
							returnValue = isUnsigned ? CPPModel::TypeInfo::Specifier::UNSIGNED_LONG_INT : CPPModel::TypeInfo::Specifier::LONG_INT;
							break;
					}
				}
				break;

			case REAL_TYPE :
				{
					const std::unique_ptr<const CPPModel::ConstantValue> typeLengthInBits( ConstantTree( TYPE_SIZE( m_tree ) ).value() );

					assert( typeLengthInBits->kind() == CPPModel::ConstantValue::Kind::INTEGER );

					switch( dynamic_cast<const CPPModel::IntegerConstant*>( typeLengthInBits.get() )->value() )
					{
						case 32 :
							returnValue = CPPModel::TypeInfo::Specifier::FLOAT;
							break;

						case 64 :
							returnValue = CPPModel::TypeInfo::Specifier::DOUBLE;
							break;

						case 128 :
							returnValue = CPPModel::TypeInfo::Specifier::LNG_DOUBLE;
							break;
					}
				}
				break;

			case POINTER_TYPE :
				returnValue = CPPModel::TypeInfo::Specifier::POINTER;
				break;

			case REFERENCE_TYPE :
				returnValue = CPPModel::TypeInfo::Specifier::REFERENCE;
				break;

			case NULLPTR_TYPE :
				returnValue = CPPModel::TypeInfo::Specifier::NULL_POINTER;
				break;

			case ARRAY_TYPE :
				returnValue = CPPModel::TypeInfo::Specifier::ARRAY;
				break;

			case RECORD_TYPE :
				returnValue = CPPModel::TypeInfo::Specifier::CLASS;
				break;

			case UNION_TYPE :
				returnValue = CPPModel::TypeInfo::Specifier::UNION;
				break;

			case QUAL_UNION_TYPE :
				//	We should not see this type in a C/C++ program.  QUAL_UNION_TYPE is associated with Ada only.
				returnValue = CPPModel::TypeInfo::Specifier::UNRECOGNIZED;
				break;

			case FUNCTION_TYPE :
				returnValue = CPPModel::TypeInfo::Specifier::FUNCTION;
				break;

			case METHOD_TYPE :
				returnValue = CPPModel::TypeInfo::Specifier::METHOD;
				break;

			default :
				returnValue = CPPModel::TypeInfo::Specifier::UNRECOGNIZED;
				break;
		}

		return( returnValue );
	}



	std::unique_ptr<const CPPModel::Type>			TypeTree::type( const CPPModel::ASTDictionary&		dictionary ) const
	{
		CPPModel::TypeInfo::Specifier				currentType = typeSpecifier();
		std::unique_ptr<const CPPModel::Type>		returnValue( new CPPModel::UnrecognizedType());

		//	TODO add other enumerations

		switch( CPPModel::CPPTypes[(int)currentType].classification )
		{
			case CPPModel::TypeInfo::Classification::FUNDAMENTAL :
			{
				returnValue.reset( new CPPModel::FundamentalType( currentType ));
			}
			break;

			case CPPModel::TypeInfo::Classification::CLASS :
			{
				const CPPModel::UID typeUID = UID();

				const CPPModel::ASTDictionary::UIDIndexConstIterator	entry = dictionary.UIDIdx().find( typeUID );

				if( entry != dictionary.UIDIdx().end() )
				{
					if( ((CPPModel::ASTDictionary::DictionaryEntryPtr)(*entry))->entryKind() == CPPModel::DictionaryEntry::EntryKind::CLASS )
					{
						const CPPModel::DictionaryClassEntry*	classEntry = dynamic_cast<const CPPModel::DictionaryClassEntry*>(&(*(*entry)));

						if( classEntry != NULL )
						{
							CPPModel::ConstListPtr<CPPModel::Attribute>	attributes( CPPModel::Attributes::deepCopy( classEntry->attributes() ));

							returnValue.reset( new CPPModel::UserDefinedType( classEntry->typeSpec(),
																			  classEntry->name(),
																			  typeUID,
																			  classEntry->enclosingNamespace(),
																			  classEntry->sourceLocation(),
																			  attributes ));
						}
					}
					else
					{
						std::cerr << "TypeInfo Classification does not match dictionary entry kind\n" << std::endl;
					}
				}
				else
				{
					//	TODO get attributes

					CPPModel::ConstListPtr<CPPModel::Attribute>		attributes( new boost::ptr_list<CPPModel::Attribute>() );

					returnValue.reset( new CPPModel::UserDefinedType( currentType,
																	  convert( TYPE_NAME( m_tree ))->identifier(),
																	  UID(),
																	  Namespace(),
																	  CPPModel::SourceLocation( "", 1, 1, 1 ),
																	  attributes ));
				}
			}
			break;

			case CPPModel::TypeInfo::Classification::DERIVED :
			{
				//	TODO get attributes

				CPPModel::ConstListPtr<CPPModel::Attribute>		attributes( new boost::ptr_list<CPPModel::Attribute>() );

				TypeTree		nextType( TREE_TYPE( m_tree ));

				returnValue.reset( new CPPModel::DerivedType( currentType, attributes, nextType.type( dictionary ) ));
			}
			break;

			case CPPModel::TypeInfo::Classification::UNION :
			{
				const CPPModel::UID typeUID = UID();

				const CPPModel::ASTDictionary::UIDIndexConstIterator	entry = dictionary.UIDIdx().find( typeUID );

				if( entry != dictionary.UIDIdx().end() )
				{
					//	TODO handle if failure mode
					if( ((CPPModel::ASTDictionary::DictionaryEntryPtr)(*entry))->entryKind() == CPPModel::DictionaryEntry::EntryKind::UNION )
					{
						const CPPModel::DictionaryUnionEntry*	unionEntry = dynamic_cast<const CPPModel::DictionaryUnionEntry*>(&(*(*entry)));

						if( unionEntry != NULL )
						{
							CPPModel::ConstListPtr<CPPModel::Attribute>	attributes( CPPModel::Attributes::deepCopy( unionEntry->attributes() ));

							returnValue.reset( new CPPModel::UnionType( unionEntry->typeSpec(),unionEntry->name().c_str(), typeUID, unionEntry->enclosingNamespace().c_str(), unionEntry->sourceLocation(), attributes ));
						}
					}
				}
				else
				{
					//	TODO get attributes

					CPPModel::ConstListPtr<CPPModel::Attribute>		attributes( new boost::ptr_list<CPPModel::Attribute>() );

					std::string		enclosingNamespace = Namespace();

					returnValue.reset( new CPPModel::UnionType( currentType,
																convert( TYPE_NAME( m_tree ))->identifier(),
																UID(),
																enclosingNamespace,
																CPPModel::SourceLocation( "", 1, 1, 1 ),
																attributes ) );
				}
			}

			break;
		}

		//	TODO Finish the implementation

		return( returnValue );
	}


}
