/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/


#include "ASTDictionary.h"

#include "config.h"
#include "gcc-plugin.h"
#include "tree.h"
#include "cp/cp-tree.h"

#include "ConstantTree.h"
#include "IdentifierTree.h"
#include "DeclOrTypeBaseTree.h"

#include "TypeTree.h"

#include "DeclTree.h"
#include "NamespaceTree.h"
#include "AttributeParser.h"




namespace GCCInternalsTools
{

	CPPModel::ConstListPtr<CPPModel::Attribute>			TypeTree::attributes()
	{
		return( GetAttributes( PurposeValueList( TYPE_ATTRIBUTES( m_tree )) ));
	}


	const NamespaceTree			TypeTree::fullyQualifiedNamespace() const
	{
/*		std::list<tree>			namespaceNodes;

		//	For types, we have to start with the CP_TYPE_CONTEXT, and then move over to the CP_DECL_CONTEXT
		//		as we iterate over the nested contexts.  Also, namespaces can be aliased, make sure we have the original.

		tree&			startingScope = ORIGINAL_NAMESPACE( CP_TYPE_CONTEXT( m_tree ));

		//	If this is the global namespace, return the scope resolution operator

		if( startingScope == global_namespace )
		{
			namespaceNodes.push_back( global_namespace );
		}
		else
		{
			//	We have to build out the full namespace context by context

			for( tree& currentScope = startingScope; currentScope != global_namespace; currentScope = ORIGINAL_NAMESPACE( CP_DECL_CONTEXT( currentScope ) ))
			{
				//	We might be inside of a class or struct.  If so, that is the current scope level name.

				if( TREE_CODE( currentScope ) == RECORD_TYPE )
				{
					namespaceNodes.push_front( TYPE_NAME( currentScope ) );
				}
				else
				{
					namespaceNodes.push_front( currentScope );
				}

			}
		}

		//	Return the full scope

		return( namespaceNodes );
*/

		return( NamespaceTree( ORIGINAL_NAMESPACE( CP_TYPE_CONTEXT( m_tree )) ) );
	}

/*
	const std::string			TypeTree::enclosingNamespace() const
	{
		//	For types, we have to start with the CP_TYPE_CONTEXT, and then move over to the CP_DECL_CONTEXT
		//		as we iterate over the nested contexts.  Also, namespaces can be aliased, make sure we have the original.

		tree&			startingScope = ORIGINAL_NAMESPACE( CP_TYPE_CONTEXT( m_tree ));

		//	If this is the global namespace, return the scope resolution operator

		if( startingScope == global_namespace )
		{
			return( CPPModel::SCOPE_RESOLUTION_OPERATOR );
		}

		//	We have to build out the full namespace context by context

		std::string			fullScope = "";

		for( tree& currentScope = startingScope; currentScope != global_namespace; currentScope = ORIGINAL_NAMESPACE( CP_DECL_CONTEXT( currentScope ) ))
		{
			//	We might be inside of a class or struct.  If so, that is the current scope level name.

			if( TREE_CODE( currentScope ) == RECORD_TYPE )
			{
				fullScope = DeclTree( TYPE_NAME( currentScope ) ).identifier() + CPPModel::SCOPE_RESOLUTION_OPERATOR + fullScope;
			}
			else
			{
				fullScope = DeclTree( currentScope ).identifier() + CPPModel::SCOPE_RESOLUTION_OPERATOR + fullScope;
			}

		}

		//	Return the full scope

		return( fullScope );
	}
*/


	CPPModel::TypeSpecifier			TypeTree::typeSpecifier() const
	{
		CPPModel::TypeSpecifier		returnValue = CPPModel::TypeSpecifier::UNRECOGNIZED;

		//	If this is a TYPE, then use the m_tree value as-is.  If it is a DECL, then we need the TREE_TYPE.

		switch( TREE_CODE( m_tree ))
		{
			case VOID_TYPE :
				returnValue = CPPModel::TypeSpecifier::VOID;
				break;

			case ENUMERAL_TYPE :
				returnValue = CPPModel::TypeSpecifier::ENUM;
				break;

			case BOOLEAN_TYPE :
				returnValue = CPPModel::TypeSpecifier::BOOLEAN;
				break;

			case INTEGER_TYPE :
				{
					bool			isUnsigned = TYPE_UNSIGNED( m_tree );

					const std::unique_ptr<const CPPModel::ConstantValue> typeLengthInBits( ConstantTree( TYPE_SIZE( m_tree ) ).value() );

					assert( typeLengthInBits->kind() == CPPModel::ConstantValue::Kind::INTEGER );


					switch( dynamic_cast<const CPPModel::IntegerConstant*>( typeLengthInBits.get() )->value() )
					{
						case 8 :
							returnValue = isUnsigned ? CPPModel::TypeSpecifier::UNSIGNED_CHAR : CPPModel::TypeSpecifier::CHAR;
							break;

						case 16 :
							returnValue = isUnsigned ? CPPModel::TypeSpecifier::UNSIGNED_SHORT_INT : CPPModel::TypeSpecifier::SHORT_INT;
							break;

						case 32 :
							returnValue = isUnsigned ? CPPModel::TypeSpecifier::UNSIGNED_INT : CPPModel::TypeSpecifier::INT;
							break;

						case 64 :
							returnValue = isUnsigned ? CPPModel::TypeSpecifier::UNSIGNED_LONG_INT : CPPModel::TypeSpecifier::LONG_INT;
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
							returnValue = CPPModel::TypeSpecifier::FLOAT;
							break;

						case 64 :
							returnValue = CPPModel::TypeSpecifier::DOUBLE;
							break;

						case 128 :
							returnValue = CPPModel::TypeSpecifier::LNG_DOUBLE;
							break;
					}
				}
				break;

			case POINTER_TYPE :
				returnValue = CPPModel::TypeSpecifier::POINTER;
				break;

			case REFERENCE_TYPE :
				returnValue = CPPModel::TypeSpecifier::REFERENCE;
				break;

			case NULLPTR_TYPE :
				returnValue = CPPModel::TypeSpecifier::NULL_POINTER;
				break;

			case ARRAY_TYPE :
				returnValue = CPPModel::TypeSpecifier::ARRAY;
				break;

			case RECORD_TYPE :
				returnValue = CPPModel::TypeSpecifier::CLASS;
				break;

			case UNION_TYPE :
				returnValue = CPPModel::TypeSpecifier::UNION;
				break;

			case QUAL_UNION_TYPE :
				//	We should not see this type in a C/C++ program.  QUAL_UNION_TYPE is associated with Ada only.
				returnValue = CPPModel::TypeSpecifier::UNRECOGNIZED;
				break;

			case FUNCTION_TYPE :
				returnValue = CPPModel::TypeSpecifier::FUNCTION;
				break;

			case METHOD_TYPE :
				returnValue = CPPModel::TypeSpecifier::METHOD;
				break;

			default :
				returnValue = CPPModel::TypeSpecifier::UNRECOGNIZED;
				break;
		}

		return( returnValue );
	}



	std::unique_ptr<const CPPModel::Type>			TypeTree::type( const CPPModel::ASTDictionary&		dictionary ) const
	{
		CPPModel::TypeSpecifier						currentTypeSpec = typeSpecifier();
		std::unique_ptr<const CPPModel::Type>		returnValue( new CPPModel::UnrecognizedType());

		//	TODO add other enumerations

		switch( CPPModel::CPPTypes[(int)currentTypeSpec].classification )
		{
			case CPPModel::TypeInfo::Classification::FUNDAMENTAL :
			{
				returnValue.reset( new CPPModel::FundamentalType( currentTypeSpec ));
			}
			break;

			case CPPModel::TypeInfo::Classification::CLASS :
			{
				const CPPModel::UID typeUID = uid();

//				const FullyQualifiedNamespace	fqName = fullyQualifiedNamespace() + identifier();

				const CPPModel::ASTDictionary::UIDIndexConstIterator		entry = dictionary.UIDIdx().find( typeUID );

				if( entry != dictionary.UIDIdx().end() )
				{
					if( ((CPPModel::ASTDictionary::DictionaryEntryPtr)(*entry))->entryKind() == CPPModel::DictionaryEntry::EntryKind::CLASS )
					{
						const CPPModel::DictionaryClassEntry*	classEntry = dynamic_cast<const CPPModel::DictionaryClassEntry*>(&(*(*entry)));

						if( classEntry != NULL )
						{
							CPPModel::ConstListPtr<CPPModel::Attribute>	attributes( CPPModel::Attributes::deepCopy( classEntry->attributes() ));

							returnValue.reset( new CPPModel::ClassOrStructType( classEntry->typeSpec(),
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

					const CPPModel::Namespace*		namespaceScope;

					std::string			scope = fullyQualifiedNamespace().asString();

					dictionary.GetNamespace( fullyQualifiedNamespace().asString(), namespaceScope );

					if( namespaceScope == NULL )
					{
						std::cerr << "Null Scope" << std::endl;
					}

					returnValue.reset( new CPPModel::ClassOrStructType( currentTypeSpec,
																	    convert( TYPE_NAME( m_tree ))->identifier(),
																	    uid(),
																	    *namespaceScope,
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

				returnValue.reset( new CPPModel::DerivedType( currentTypeSpec, attributes, nextType.type( dictionary ) ));
			}
			break;

			case CPPModel::TypeInfo::Classification::UNION :
			{
				const CPPModel::UID typeUID = uid();

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

							returnValue.reset( new CPPModel::UnionType( unionEntry->typeSpec(),unionEntry->name().c_str(), typeUID, unionEntry->enclosingNamespace(), unionEntry->sourceLocation(), attributes ));
						}
					}
				}
				else
				{
					//	TODO get attributes

					CPPModel::ConstListPtr<CPPModel::Attribute>		attributes( new boost::ptr_list<CPPModel::Attribute>() );

					const CPPModel::Namespace*		namespaceScope;

					dictionary.GetNamespace( fullyQualifiedNamespace().asString(), namespaceScope );

					returnValue.reset( new CPPModel::UnionType( currentTypeSpec,
																convert( TYPE_NAME( m_tree ))->identifier(),
																uid(),
																*namespaceScope,
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
