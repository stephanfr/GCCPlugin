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

#include "GCCInternalsTools.h"




namespace GCCInternalsTools
{

	CPPModel::TypeInfo::Specifier			TypeSpecifier( const tree&	treeToType )
	{
		CPPModel::TypeInfo::Specifier		returnValue = CPPModel::TypeInfo::Specifier::UNRECOGNIZED;


		switch( TREE_CODE( treeToType ) )
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
					bool			isUnsigned = TYPE_UNSIGNED( treeToType );

					const std::unique_ptr<const CPPModel::ConstantValue> typeLengthInBits( ConstantTree( TYPE_SIZE( treeToType ) ).value() );

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
					const std::unique_ptr<const CPPModel::ConstantValue> typeLengthInBits( ConstantTree( TYPE_SIZE( treeToType ) ).value() );

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


	CPPModel::ConstListPtr<CPPModel::BaseClassIdentifier>		GetBaseClasses( const DictionaryClassEntryImpl&		dictionaryEntry,
																				const CPPModel::ParseOptions&		options )
	{
		CPPModel::ListPtr<CPPModel::BaseClassIdentifier>		baseClassIdentifierList( new boost::ptr_list<CPPModel::BaseClassIdentifier>() );

		//	Get the list of base classes and its length.  This is not a TREE_LIST, so we iterate in a different fashion.

		const tree&				baseClasses( TYPE_BINFO( dictionaryEntry.getTree() ));
		size_t 					numBases ( baseClasses ? BINFO_N_BASE_BINFOS( baseClasses ) : 0 );

		if( numBases > 0 )
		{
			bool					hasAccessSpecs = ( baseClasses != 0 );

			//	Iterate over the list

			for( size_t i = 0; i < numBases; i++ )
			{
				const tree&			currentBaseClass( BINFO_BASE_BINFO( baseClasses, i ));

				//	Get the name and determine if the class is virtual

				const std::string 	baseClassName = DeclTree( TYPE_NAME( TYPE_MAIN_VARIANT( BINFO_TYPE( currentBaseClass )))).identifier();

				std::string			baseNamespace = TypeTree( TYPE_MAIN_VARIANT( BINFO_TYPE( currentBaseClass )) ).enclosingNamespace();
//	TODO lookup base class by GUID and add namespace
				bool				isVirtual( BINFO_VIRTUAL_P( currentBaseClass ));

				//	Get the access specifier
				//	This looks a little goofy as we first have to check if there is an access spec list
				//		and then if there is we have to pull the spec out by index.  We default to public
				//		if the list simply does not exist (which I'd expect for C).

				CPPModel::AccessSpecifier		accessSpec( hasAccessSpecs ? IdentifierTree( BINFO_BASE_ACCESS( baseClasses, i )).AccessSpecifier() : CPPModel::AccessSpecifier::PUBLIC );

				//	Create the identifier and add it to our list

				baseClassIdentifierList->push_back( new CPPModel::BaseClassIdentifier( baseClassName, isVirtual, accessSpec ) );
			}
		}

		//	Return a moved unique pointer to a const-ed version of the list

		return( CPPModel::MakeConst<CPPModel::BaseClassIdentifier>( baseClassIdentifierList ));
	}


	CPPModel::ConstListPtr<CPPModel::FriendIdentifier>			GetFriends( const DictionaryClassEntryImpl&		dictionaryEntry,
																			const CPPModel::ParseOptions&		options )
	{
		CPPModel::ListPtr<CPPModel::FriendIdentifier>		friendList( new boost::ptr_list<CPPModel::FriendIdentifier>() );

		//	Start by getting the classes which are friends

		PurposeValueList		friendClassList( CLASSTYPE_FRIEND_CLASSES( dictionaryEntry.getTree() ));

		for( PurposeValueList::iterator itrFriendClass = friendClassList.begin(); itrFriendClass != friendClassList.end(); ++itrFriendClass )
		{
			friendList->push_back( new CPPModel::FriendClassIdentifier( TypeTree( TYPE_MAIN_VARIANT( itrFriendClass->value() )).type( dictionaryEntry.Dictionary() )));
		}

		//	Now, get the friend functions - some of which may be declared within the class
		//
		//	I must confess that I don't understand why this is a nested list but the following appears to work

		for( tree& friendFunctions = DECL_FRIENDLIST( TYPE_MAIN_DECL( dictionaryEntry.getTree() ) ); friendFunctions != 0; friendFunctions = TREE_CHAIN( friendFunctions ) )
		{
			for( tree& itrFriendFunction = TREE_VALUE( friendFunctions ); itrFriendFunction != 0 ; itrFriendFunction = TREE_CHAIN ( itrFriendFunction ))
			{
				const tree&	friendNode = TREE_VALUE( itrFriendFunction );

				if( DECL_FUNCTION_MEMBER_P( friendNode ) )
				{
					//	We have a class member, i.e. method, as a friend function

					const tree& methodContext = DECL_CONTEXT( friendNode );

					friendList->push_back( new CPPModel::FriendClassMemberIdentifier( DeclOrTypeBaseTree::convert( methodContext )->type( dictionaryEntry.Dictionary() ), DeclTree( friendNode ).identifier() ));
				}
				else
				{
					//	We have a regular friend function

					friendList->push_back( new CPPModel::FriendFunctionIdentifier( DeclTree( friendNode ).identifier() ));
				}
			}
		}

		//	Return a moved unique pointer to a const-ed version of the list

		return( CPPModel::MakeConst<CPPModel::FriendIdentifier>( friendList ));
	}


	CPPModel::ConstListPtr<CPPModel::UnionMember>		GetUnionMembers( const tree&						treeNode,
												 	 	 	 	 	 	 const CPPModel::ASTDictionary&		dictionary,
												 	 	 	 	 	 	 const CPPModel::ParseOptions&		options )
	{
		CPPModel::ListPtr<CPPModel::UnionMember>		memberList( new boost::ptr_list<CPPModel::UnionMember>() );

		//	Iterate over the fields

		UnionMemberList		members( TYPE_FIELDS( TREE_TYPE( treeNode ) ));

		for( UnionMemberList::iterator itrMember = members.begin(); itrMember != members.end(); ++itrMember )
		{
			if( itrMember->isArtificial() )
			{
				continue;
			}

			//	Create the field based on the tree code

			CPPModel::SourceLocation					sourceLocation = itrMember->sourceLocation();

			CPPModel::AccessSpecifier					accessSpec = itrMember->accessSpecifier();

			switch( itrMember->treeCode() )
			{
				case FIELD_DECL:
				{
					memberList->push_back( new CPPModel::UnionMember( itrMember->identifier(), itrMember->uid(), itrMember->type( dictionary ), accessSpec, sourceLocation ) );
					break;
				}
				default:
				{
					memberList->push_back( new CPPModel::UnionMember( itrMember->identifier(), itrMember->uid(), itrMember->type( dictionary ), accessSpec, sourceLocation ) );
					break;
				}
			}
		}

		//	Return the list

		return( CPPModel::MakeConst<CPPModel::UnionMember>( memberList ));
	}



	CPPModel::ConstListPtr<CPPModel::FieldDeclaration>		GetFields( const DictionaryClassEntryImpl&		dictionaryEntry,
															   	   	   const CPPModel::ParseOptions&		options )
	{
		CPPModel::ListPtr<CPPModel::FieldDeclaration>		fieldList( new boost::ptr_list<CPPModel::FieldDeclaration>());

		//	Iterate over the fields

		FieldList		fields( TYPE_FIELDS( dictionaryEntry.getTree() ) );

		for( FieldList::iterator itrField = fields.begin(); itrField != fields.end(); ++itrField )
		{
			if( itrField->isArtificial() )
			{
				continue;
			}

			//	Create the field based on the tree code

			bool												isStatic = !DECL_NONSTATIC_MEMBER_P( (const tree&)*itrField );

			CPPModel::SourceLocation							sourceLocation( itrField->sourceLocation() );

			CPPModel::AccessSpecifier							accessSpec = itrField->accessSpecifier();

			CPPModel::ConstListPtr<CPPModel::Attribute>			attributes( GetAttributes( PurposeValueList( DECL_ATTRIBUTES( (const tree&)*itrField ) )));

			switch( itrField->treeCode() )
			{
				case TYPE_DECL:
				{
					//	TODO I think this is wrong

					if( !DECL_SELF_REFERENCE_P( (const tree&)*itrField ))
					{
						fieldList->push_back( new CPPModel::FieldDeclaration( itrField->identifier(), itrField->uid(), sourceLocation, itrField->type( dictionaryEntry.Dictionary() ), isStatic, accessSpec, attributes ) );
					}
					break;
				}
				case FIELD_DECL:
				{
					if( !itrField->isArtificial() )
					{
						fieldList->push_back( new CPPModel::FieldDeclaration( itrField->identifier(), itrField->uid(), sourceLocation, itrField->type( dictionaryEntry.Dictionary() ), isStatic, accessSpec, attributes ) );
					}
					break;
				}
				default:
				{
					fieldList->push_back( new CPPModel::FieldDeclaration( itrField->identifier(), itrField->uid(), sourceLocation, itrField->type( dictionaryEntry.Dictionary() ), isStatic, accessSpec, attributes ) );
					break;
				}
			}
		}

		return( CPPModel::MakeConst<CPPModel::FieldDeclaration>( fieldList ));
	}



	CPPModel::ConstListPtr<CPPModel::MethodDeclaration>		GetMethods( const DictionaryClassEntryImpl&		dictionaryEntry,
																		const CPPModel::ParseOptions&		options )
	{
		CPPModel::ListPtr<CPPModel::MethodDeclaration>		methodList( new boost::ptr_list<CPPModel::MethodDeclaration>() );

		//	Iterate over the methods

		MethodList		methods( TYPE_METHODS( dictionaryEntry.getTree() ) );

		for( MethodList::iterator itrMethod = methods.begin(); itrMethod != methods.end(); ++itrMethod )
		{
			//	Don't add artificial or built-in methods, gcc appears to use 'artificial' methods as placeholders it creates in the AST
			//		and built-in methods are added for RTTI and a couple other language features.

			if( itrMethod->isArtificial() || itrMethod->isBuiltIn() )
			{
				continue;
			}

			//	OK, cloned functions.  GCC may generate 3 versions of every constructor and 3 versions of every destructor.
			//		(I don't fully understand this at the moment but this is my current 'best guess'.)
			//
			//		__base_ctor		- 'base' constructor initializes all the non-virtual stuff in a class
			//		__comp_ctor		- 'complete' constructor does everything the 'base' constructor does plus virtual base class initialization
			//		allocating complete constructor - does everything the 'complete' constructor does plus allocates heap
			//
			//		__base_dtor		- 'base' destructor that handles all non-virtual destruction
			//		__comp_ctor		- 'complete' destructor that does everything the base does and handles virtual base class destruction
			//		deleting complete destructor - does everything the 'complete' destructor does plus free any heap held by the object
			//
			//	Include cloned functions only if instructed.  The default is to omit them as they don't actually appear in the source code.

			if( !options.includeClonedFunctions && DECL_CLONED_FUNCTION_P( (const tree&)*itrMethod ))
			{
				continue;
			}

			//	Get the method name and create the parameter list for the method

			const std::string								methodName = itrMethod->identifier();
			bool											isStatic = !DECL_NONSTATIC_MEMBER_P( (const tree&)*itrMethod );

			CPPModel::AccessSpecifier						accessSpec = itrMethod->accessSpecifier();

			CPPModel::ConstListPtr<CPPModel::Attribute>		attributes( GetAttributes( PurposeValueList( DECL_ATTRIBUTES( (const tree&)*itrMethod ) )));

			CPPModel::ListPtr<CPPModel::FunctionParameter>	parameterList( new boost::ptr_list<CPPModel::FunctionParameter>() );

			//	Choose the first argument to convert conditionally on the compiler generated arguments option
			//		The compiler will add 'this' to methods, and potentially other arguments for handling virtual calls

			ParameterList		parameters = options.includeCompilerGeneratedFunctionArguments ? DECL_ARGUMENTS( (const tree&)*itrMethod ) : FUNCTION_FIRST_USER_PARM( (const tree&)*itrMethod );

			//	Loop over all the arguments

			for( ParameterList::iterator itrParameter = parameters.begin(); itrParameter != parameters.end(); ++itrParameter )
			{
				//	Add the argument to the list

				parameterList->push_back( new CPPModel::FunctionParameter( itrParameter->identifier(), DeclOrTypeBaseTree::convert( TREE_TYPE( (const tree&)itrParameter ) )->type( dictionaryEntry.Dictionary() )));
			}

			//	Add the method to our list

			methodList->push_back( new CPPModel::MethodDeclaration( methodName, itrMethod->uid(), itrMethod->sourceLocation(), isStatic, accessSpec, DeclOrTypeBaseTree::convert( TREE_TYPE( DECL_RESULT( (const tree&)*itrMethod ) ) )->type( dictionaryEntry.Dictionary() ), attributes, parameterList ) );
		}

		//	Return the list of methods and we are done

		return( CPPModel::MakeConst<CPPModel::MethodDeclaration>( methodList ));
	}



	bool		DictionaryClassEntryImpl::GetClassDefinition( const CPPModel::ParseOptions&							options,
												   	     	  std::unique_ptr<const CPPModel::ClassDefinition>&		classDef ) const
	{
		//	Let's get base classes, friends, fields, methods and attributes

		CPPModel::ConstListPtr<CPPModel::BaseClassIdentifier>		baseClasses( GetBaseClasses( *this, options ));
		CPPModel::ConstListPtr<CPPModel::FriendIdentifier>	 		friends( GetFriends( *this, options ));
		CPPModel::ConstListPtr<CPPModel::FieldDeclaration>	 		fields( GetFields( *this, options ));
		CPPModel::ConstListPtr<CPPModel::MethodDeclaration>	 		methods( GetMethods( *this, options ));

		CPPModel::ConstListPtr<CPPModel::Attribute>					attr( CPPModel::Attributes::deepCopy( attributes() ));

		//	Build the class definition and return a shared pointer to it

		classDef.reset( new CPPModel::ClassDefinition( name(),
													   uid(),
													   enclosingNamespace(),
													   !CLASSTYPE_DECLARED_CLASS( getTree() ),
													   attr,
													   baseClasses,
													   friends,
													   fields,
													   methods,
													   sourceLocation() ));

		return( true );
	}



	bool		DictionaryUnionEntryImpl::GetUnionDefinition( const CPPModel::ParseOptions&							options,
															  std::unique_ptr<const CPPModel::UnionDefinition>&		unionDef ) const
	{
		CPPModel::ConstListPtr<CPPModel::UnionMember>	unionMembers(  );

		unionDef.reset( new CPPModel::UnionDefinition( name(),
													   uid(),
													   enclosingNamespace(),
													   sourceLocation(),
													   CPPModel::Attributes::deepCopy( attributes() ),
													   GetUnionMembers( getTree(), Dictionary(), options ) ) );

		return( true );
	}



	bool		DictionaryFunctionEntryImpl::GetFunctionDefinition( const CPPModel::ParseOptions&							options,
											   	   	   	   	   	   	std::unique_ptr<const CPPModel::FunctionDefinition>&	functionDef ) const
	{
		CPPModel::ListPtr<CPPModel::FunctionParameter>	parameterList( new boost::ptr_list<CPPModel::FunctionParameter>() );

		ParameterList		parameters = options.includeCompilerGeneratedFunctionArguments ? DECL_ARGUMENTS( getTree() ) : FUNCTION_FIRST_USER_PARM( getTree() );

		//	Loop over all the arguments

		for( ParameterList::iterator itrParameter = parameters.begin(); itrParameter != parameters.end(); ++itrParameter )
		{
			//	Add the argument to the list

			parameterList->push_back( new CPPModel::FunctionParameter( itrParameter->identifier(), DeclOrTypeBaseTree::convert( TREE_TYPE( (const tree&)itrParameter ) )->type( Dictionary() )));
		}

		functionDef.reset( new CPPModel::FunctionDefinition( name(),
															 uid(),
															 enclosingNamespace(),
															 sourceLocation(),
															 isHiddenFriend(),
															 CPPModel::Attributes::deepCopy( attributes() ),
															 DeclOrTypeBaseTree::convert( TREE_TYPE( DECL_RESULT( getTree() )))->type( Dictionary() ),
															 CPPModel::MakeConst<CPPModel::FunctionParameter>( parameterList )));

		return( true );
	}


	bool		DictionaryGlobalVarEntryImpl::GetGlobalVarDefinition( const CPPModel::ParseOptions&								options,
											   	   	   	   	   	   	  std::unique_ptr<const CPPModel::GlobalVarDefinition>&		globalVarDef ) const
	{
		globalVarDef.reset( new CPPModel::GlobalVarDefinition( name(),
															   uid(),
															   enclosingNamespace(),
															   sourceLocation(),
															   isStatic(),
															   CPPModel::Attributes::deepCopy( attributes() ),
															   DeclOrTypeBaseTree::convert( TREE_TYPE( getTree() ))->type( Dictionary() ) ));

		return( true );
	}



	void 	ASTDictionaryImpl::DecodingPass( const tree&			namespaceNode,
							  	  	  	  	 ITreeDecoder&			decoder )
	{
		cp_binding_level*		currentLevel = NAMESPACE_LEVEL( namespaceNode );

		for( tree currentDecl = currentLevel->names; currentDecl != 0; currentDecl = TREE_CHAIN( currentDecl ) )
		{
			if( DECL_IS_BUILTIN( currentDecl ) )
			{
				continue;
			}

			decoder.Decode( currentDecl, *this );
		}

		for( tree currentDecl = currentLevel->namespaces; currentDecl != 0; currentDecl = TREE_CHAIN( currentDecl ) )
		{
			if( DECL_IS_BUILTIN( currentDecl ) )
			{
				continue;
			}

			DecodingPass( currentDecl, decoder );
		}
	}





	bool	ClassDecoder::Decode( const tree&					classTree,
								  CPPModel::ASTDictionary&		dictionary ) const
	{
		{
			const tree&		treeType = TREE_TYPE( classTree );

			if( !treeType )
			{
				return( false );
			}

			//	Classes appear in the AST as a RECORD_TYPE that is ARTIFICIAL.
			//		The ARTIFICIAL flag distinguishes between a 'typedef' and a 'class' or 'struct' declaration,
			//		internally GCC treats class declarations like a typedef and attaches the ARTIFICIAL
			//		flag to indicate that it has artificially overloaded this typedef as a class declaration.

			if( !(( TREE_CODE( classTree ) == TYPE_DECL ) && ( TREE_CODE( treeType ) == RECORD_TYPE ) && ( DECL_ARTIFICIAL( classTree ) )))
			{
				return( false );
			}
		}


		const CPPModel::SourceLocation	sourceLocation = DeclTree( classTree ).sourceLocation();
		const TypeTree					typeDeclared = TypeTree( TREE_TYPE( classTree ));

		const TypeTree					mainTypeDecl = TypeTree( TYPE_MAIN_VARIANT( (const tree&)typeDeclared ));

		std::string						className = DeclTree( TYPE_NAME( (const tree&)mainTypeDecl )).identifier();

		//	Don't decode templates here

		if( CLASSTYPE_TEMPLATE_INFO( (const tree&)mainTypeDecl ) )
		{
			return( false );
		}

		CPPModel::TypeInfo::Specifier	typeSpec = typeDeclared.typeSpecifier();

		if( CPPModel::CPPTypes[(int)typeSpec].classification == CPPModel::TypeInfo::Classification::CLASS )
		{
			if( dictionary.UIDIdx().find( typeDeclared.uid() ) == dictionary.UIDIdx().end() )
			{
				dictionary.insert( new DictionaryClassEntryImpl( dictionary,
																 mainTypeDecl.uid(),
																 className,
																 mainTypeDecl.enclosingNamespace(),
																 sourceLocation,
																 GetAttributes( PurposeValueList( TYPE_ATTRIBUTES( (const tree&)mainTypeDecl ) )),
																 typeSpec,
																 mainTypeDecl ));
			}
		}

		//	We decoded the class, return true

		return( true );
	}





	bool	UnionDecoder::Decode( const tree&					unionNode,
								  CPPModel::ASTDictionary&		dictionary ) const
	{
		const tree&		treeType = TREE_TYPE( unionNode );

		if( !treeType )
		{
			return( false );
		}

		if( !(( TREE_CODE( unionNode ) == TYPE_DECL ) && ( TREE_CODE( treeType ) == UNION_TYPE ) ))
		{
			return( false );
		}


		DeclTree							unionTree( unionNode );

		CPPModel::TypeInfo::Specifier		typeSpec = unionTree.typeSpecifier();

		const CPPModel::UID					unionUID = unionTree.uid();

		if( CPPModel::CPPTypes[(int)typeSpec].classification == CPPModel::TypeInfo::Classification::UNION )
		{
			if( dictionary.UIDIdx().find( unionUID ) == dictionary.UIDIdx().end() )
			{
				dictionary.insert(  new DictionaryUnionEntryImpl( dictionary,
																  unionUID,
																  unionTree.identifier(),
																  unionTree.enclosingNamespace(),
																  unionTree.sourceLocation(),
																  unionTree.treeType().attributes(),
																  typeSpec,
																  unionNode ));
			}
		}

		return( true );
	}





	bool	FunctionDecoder::Decode( const tree&					functionNode,
  	  	  	    					 CPPModel::ASTDictionary&		dictionary ) const
	{
		if( !( TREE_CODE( (const tree&)functionNode ) == FUNCTION_DECL ))
		{
			return( false );
		}

		//	We have a DECL_TREE

		DeclTree			functionTree( functionNode );

		if( functionTree.isArtificial() || functionTree.isBuiltIn() )
		{
			return( false );
		}

//		if( !options.includeClonedFunctions && DECL_CLONED_FUNCTION_P( (const tree&)currentMethod ) )
//		{
//			continue;
//		}


		CPPModel::TypeInfo::Specifier		typeSpec = functionTree.typeSpecifier();

		const CPPModel::UID					functionUID = functionTree.uid();

		//	Make sure at a finer grained level that we have a function

		if( CPPModel::CPPTypes[(int)typeSpec].classification == CPPModel::TypeInfo::Classification::FUNCTION )
		{
			if( dictionary.UIDIdx().find( functionUID ) == dictionary.UIDIdx().end() )
			{
				dictionary.insert(  new DictionaryFunctionEntryImpl( dictionary,
																	 functionUID,
																	 functionTree.identifier(),
																	 functionTree.enclosingNamespace(),
																	 functionTree.sourceLocation(),
																	 functionTree.attributes(),
																	 typeSpec,
																	 DECL_HIDDEN_FRIEND_P( functionNode ),
																	 functionNode ));
			}
		}

		//	Return success

		return( true );
	}




	bool	GlobalVarDecoder::Decode( const tree&					globalVarNode,
								  	  CPPModel::ASTDictionary&		dictionary ) const
	{
		//	Make sure we have a VAR_DECL node

		if( TREE_CODE( globalVarNode ) != VAR_DECL )
		{
			return( false );
		}

		//	We have a DECL_TREE

		DeclTree			globalVarTree( globalVarNode );

		if( globalVarTree.isArtificial() )
		{
			return( false );
		}

		CPPModel::UID			globalVarUID = globalVarTree.uid();

		if( dictionary.UIDIdx().find( globalVarUID ) == dictionary.UIDIdx().end() )
		{
			//	Build and insert the dictionary entry

			dictionary.insert( new DictionaryGlobalVarEntryImpl( dictionary,
																 globalVarUID,
																 globalVarTree.identifier(),
																 globalVarTree.enclosingNamespace(),
																 TREE_STATIC( globalVarNode ) != 0,
																 globalVarTree.sourceLocation(),
																 globalVarTree.attributes(),
																 globalVarTree.typeSpecifier(),
																 globalVarNode ));
		}

		//	Finished with success

		return( true );
	}


	void	ASTDictionaryImpl::Build()
	{
		{
			ClassDecoder		classDecoder;

			DecodingPass( global_namespace, (ITreeDecoder&)classDecoder );
		}

		{
			UnionDecoder		unionDecoder;

			DecodingPass( global_namespace, (ITreeDecoder&)unionDecoder );
		}

		{
			FunctionDecoder		functionDecoder;

			DecodingPass( global_namespace, (ITreeDecoder&)functionDecoder );
		}

		{
			GlobalVarDecoder	globalVarDecoder;

			DecodingPass( global_namespace, (ITreeDecoder&)globalVarDecoder );
		}
	}

}
