/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/


#include "GCCInternalsTools.h"

#include "ConstantTree.h"
#include "IdentifierTree.h"
#include "DeclTree.h"
#include "NamespaceTree.h"
#include "AttributeParser.h"

#include "gimple.h"

#include "rtl.h"

#include "real.h"
#include "float.h"

#include "gimple-pretty-print.h"





namespace GCCInternalsTools
{

	CPPModel::TypeSpecifier			TypeSpecifier( const tree&	treeToType )
	{
		CPPModel::TypeSpecifier		returnValue = CPPModel::TypeSpecifier::UNRECOGNIZED;


		switch( TREE_CODE( treeToType ) )
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
					bool			isUnsigned = TYPE_UNSIGNED( treeToType );

					const std::unique_ptr<const CPPModel::ConstantValue> typeLengthInBits( ConstantTree( TYPE_SIZE( treeToType ) ).value() );

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
					const std::unique_ptr<const CPPModel::ConstantValue> typeLengthInBits( ConstantTree( TYPE_SIZE( treeToType ) ).value() );

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


	tree				ASTTreeForType( CPPModel::TypeSpecifier		typeSpec )
	{
		tree		returnValue;

		switch( typeSpec )
		{
			case	CPPModel::TypeSpecifier::UNRECOGNIZED :
				returnValue = NULL;
				break;

			case	CPPModel::TypeSpecifier::VOID :
				returnValue = void_type_node;
				break;

			case	CPPModel::TypeSpecifier::ENUM :
				returnValue = NULL;
				break;

			case	CPPModel::TypeSpecifier::CHAR :
				returnValue = signed_char_type_node;
				break;

			case	CPPModel::TypeSpecifier::UNSIGNED_CHAR :
				returnValue = unsigned_char_type_node;
				break;

			case	CPPModel::TypeSpecifier::SHORT_INT :
				returnValue = short_integer_type_node;
				break;

			case	CPPModel::TypeSpecifier::UNSIGNED_SHORT_INT :
				returnValue = short_unsigned_type_node;
				break;

			case	CPPModel::TypeSpecifier::INT :
				returnValue = integer_type_node;
				break;

			case	CPPModel::TypeSpecifier::UNSIGNED_INT :
				returnValue = unsigned_type_node;
				break;

			case	CPPModel::TypeSpecifier::LONG_INT :
				returnValue = long_integer_type_node;
				break;

			case	CPPModel::TypeSpecifier::UNSIGNED_LONG_INT :
				returnValue = long_unsigned_type_node;
				break;

			case	CPPModel::TypeSpecifier::FLOAT :
				returnValue = float_type_node;
				break;

			case	CPPModel::TypeSpecifier::DOUBLE :
				returnValue = double_type_node;
				break;

			case	CPPModel::TypeSpecifier::LNG_DOUBLE :
				returnValue = long_double_type_node;
				break;

			case	CPPModel::TypeSpecifier::BOOLEAN :
				returnValue = boolean_type_node;
				break;

			case	CPPModel::TypeSpecifier::FUNCTION :
				returnValue = NULL;
				break;

			case	CPPModel::TypeSpecifier::METHOD :
				returnValue = NULL;
				break;

			case	CPPModel::TypeSpecifier::CLASS :
				returnValue = NULL;
				break;

			case	CPPModel::TypeSpecifier::POINTER :
				returnValue = NULL;
				break;

			case	CPPModel::TypeSpecifier::REFERENCE :
				returnValue = NULL;
				break;

			case	CPPModel::TypeSpecifier::ARRAY :
				returnValue = NULL;
				break;

			case	CPPModel::TypeSpecifier::UNION :
				returnValue = NULL;
				break;

			case	CPPModel::TypeSpecifier::NULL_POINTER :
				returnValue = NULL;
				break;

			case	CPPModel::TypeSpecifier::NO_RETURN :
				returnValue = NULL;
				break;

			case	CPPModel::TypeSpecifier::STRING :
				returnValue = build_pointer_type( signed_char_type_node );
				break;
		}

		return( returnValue );
	}



	tree			AsTree( bool		booleanValue )
	{
		return( build_int_cst( boolean_type_node, booleanValue ));
	}

	tree			AsTree( char		charValue )
	{
		return( build_int_cst( char_type_node, charValue ));
	}

	tree			AsTree( int			intValue )
	{
		return( build_int_cst( integer_type_node, intValue ));
	}

	tree			AsTree( long			longValue )
	{
		return( build_int_cst( long_integer_type_node, longValue ));
	}

	tree			AsTree( float			floatValue )
	{
		//	I don't care for the conversion technique of going to a string and then back into a real_value,
		//		but I could find no other way to make the conversion for doubles so I replicated it here.

		real_value		r;

		char			buffer[256];

		sprintf( buffer, "%g", floatValue );

		real_from_string( &r, buffer );

		return( build_real( float_type_node, r ));
	}

	tree			AsTree( double			doubleValue )
	{
		//	I could find no other way to convert a double to a real_value - except by going through a string conversion.

		real_value		r;

		char			buffer[256];

		sprintf( buffer, "%g", doubleValue );

		real_from_string( &r, buffer );

		return( build_real( double_type_node, r ));
	}

	tree			AsTree( std::string		stringValue )
	{
	    tree			stringConstant = build_string_literal( stringValue.length() + 1, stringValue.c_str() + '\0' );

	    TREE_STATIC( stringConstant ) = true;
	    TREE_READONLY( stringConstant ) = true;

		return( stringConstant );
	}




	enum class ConvertParameterResultCodes { SUCCESS, UNSUPPORTED_TYPE };

	typedef SEFUtility::ResultWithReturnValue<ConvertParameterResultCodes,tree>						ConvertParameterValueResult;


	ConvertParameterValueResult			ConvertParameterValue( const CPPModel::ParameterValueBase&		value )
	{

		tree		returnValue;

		switch( CPPModel::AsTypeSpecifier( value.type() ))
		{
			case CPPModel::TypeSpecifier::BOOLEAN :
				returnValue = AsTree( dynamic_cast<const CPPModel::BooleanConstantParameter&>(value).value() );
				break;

			case CPPModel::TypeSpecifier::CHAR :
				returnValue = AsTree( dynamic_cast<const CPPModel::CharConstantParameter&>(value).value() );
				break;

			case CPPModel::TypeSpecifier::INT :
				returnValue = AsTree( dynamic_cast<const CPPModel::IntConstantParameter&>(value).value() );
				break;

			case CPPModel::TypeSpecifier::LONG_INT :
				returnValue = AsTree( dynamic_cast<const CPPModel::LongConstantParameter&>(value).value() );
				break;

			case CPPModel::TypeSpecifier::FLOAT :
				returnValue = AsTree( dynamic_cast<const CPPModel::FloatConstantParameter&>(value).value() );
				break;

			case CPPModel::TypeSpecifier::DOUBLE :
				returnValue = AsTree( dynamic_cast<const CPPModel::DoubleConstantParameter&>(value).value() );
				break;

			case CPPModel::TypeSpecifier::STRING :
				returnValue = AsTree( dynamic_cast<const CPPModel::StringConstantParameter&>(value).value() );
				break;

			default :
				return( ConvertParameterValueResult::Failure( ConvertParameterResultCodes::UNSUPPORTED_TYPE, "Internal Error - Unsupported Type for Parameter Value Conversion" ));
		}

		return( ConvertParameterValueResult( returnValue ) );
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

				std::string			baseNamespace = TypeTree( TYPE_MAIN_VARIANT( BINFO_TYPE( currentBaseClass )) ).fullyQualifiedNamespace().asString();
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


	CPPModel::ConstListPtr<CPPModel::FriendIdentifier>			GetFriends( const DictionaryClassEntryImpl&			dictionaryEntry,
																			const CPPModel::ParseOptions&			options )
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


	CPPModel::ConstListPtr<CPPModel::UnionMember>				GetUnionMembers( const tree&						treeNode,
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


	CPPModel::ConstListPtr<CPPModel::FieldDeclaration>			GetFields( const DictionaryClassEntryImpl&			dictionaryEntry,
															   	   	   	   const CPPModel::ParseOptions&			options )
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
					//	TODO This is wrong - it should handle things like: typedef struct { char __arr[2]; } __two;

//					if( !DECL_SELF_REFERENCE_P( (const tree&)*itrField ))
//					{
//						fieldList->push_back( new CPPModel::FieldDeclaration( itrField->identifier(), itrField->uid(), sourceLocation, itrField->type( dictionaryEntry.Dictionary() ), isStatic, accessSpec, attributes ) );
//					}
					break;
				}
				case FIELD_DECL:
				{
					if( !itrField->isArtificial() )
					{
						fieldList->push_back( new CPPModel::FieldDeclaration( itrField->identifier(), itrField->uid(), sourceLocation, itrField->type( dictionaryEntry.Dictionary() ), isStatic, accessSpec, itrField->offsetInfo(), attributes ) );
					}
					break;
				}
				default:
				{
					fieldList->push_back( new CPPModel::FieldDeclaration( itrField->identifier(), itrField->uid(), sourceLocation, itrField->type( dictionaryEntry.Dictionary() ), isStatic, accessSpec, itrField->offsetInfo(), attributes ) );
					break;
				}
			}
		}

		return( CPPModel::MakeConst<CPPModel::FieldDeclaration>( fieldList ));
	}


	CPPModel::ListPtr<CPPModel::FunctionParameter>				GetFunctionParameters( ParameterList						rawParameters,
																				   	   const CPPModel::ASTDictionary&		dictionary )
	{
		CPPModel::ListPtr<CPPModel::FunctionParameter>	parameterList( new boost::ptr_list<CPPModel::FunctionParameter>() );

		for( ParameterList::iterator itrParameter = rawParameters.begin(); itrParameter != rawParameters.end(); ++itrParameter )
		{
			//	Add the argument to the list

			std::unique_ptr<DeclOrTypeBaseTree>		typeDecl = DeclOrTypeBaseTree::convert( TREE_TYPE( (const tree&)itrParameter ) );

			parameterList->push_back( new CPPModel::FunctionParameter( itrParameter->identifier(), typeDecl->type( dictionary ), itrParameter->compilerSpecificFlags() ) );
		}

		return( parameterList );
	}


	CPPModel::ListPtr<CPPModel::TemplateParameter>				GetTemplateParameters( const tree&							functionTree,
																				   	   const CPPModel::ASTDictionary&		dictionary )
	{
		CPPModel::ListPtr<CPPModel::TemplateParameter>	parameterList( new boost::ptr_list<CPPModel::TemplateParameter>() );

		PurposeValueList		topLevelList = PurposeValueList( DECL_TEMPLATE_PARMS( functionTree ) );

		for( PurposeValueList::iterator itrVector = topLevelList.begin(); itrVector != topLevelList.end(); ++itrVector )
		{
			const tree&		templateParamVec = itrVector->value();

			int				vecLength = TREE_VEC_LENGTH( templateParamVec );

			std::cerr << "Vec Length: " << vecLength << std::endl;

			for( int i = 0; i < vecLength; ++i )
			{
				const tree&		innerListTree = TREE_VEC_ELT( templateParamVec, i );

				PurposeValueList		innerList = PurposeValueList( innerListTree );

				for( PurposeValueList::iterator itrParam = innerList.begin(); itrParam != innerList.end(); ++itrParam )
				{
					DeclTree				paramDecl( itrParam->value() );

					CPPModel::CompilerSpecific		compilerSpecificAttr( DECL_BUILT_IN( (const tree&)paramDecl ), DECL_ARTIFICIAL( (const tree&)paramDecl ) );

					parameterList->push_back( new CPPModel::TemplateParameter( DeclOrTypeBaseTree::convert( itrParam->value() )->identifier(), compilerSpecificAttr ) );
				}
			}
		}

		return( parameterList );
	}



	CPPModel::ConstListPtr<CPPModel::MethodDeclaration>			GetMethods( const DictionaryClassEntryImpl&		dictionaryEntry,
																			const CPPModel::ParseOptions&		options )
	{
		CPPModel::ListPtr<CPPModel::MethodDeclaration>		methodList( new boost::ptr_list<CPPModel::MethodDeclaration>() );

		//	Iterate over the methods

		MethodList		methods( TYPE_METHODS( dictionaryEntry.getTree() ) );

		for( MethodList::iterator itrMethod = methods.begin(); itrMethod != methods.end(); ++itrMethod )
		{
			//	Don't add artificial or built-in methods, gcc appears to use 'artificial' methods as placeholders it creates in the AST
			//		and built-in methods are added for RTTI and a couple other language features.
			//
			//	Also, we are only interested in function decls in this method

			if( itrMethod->isArtificial() || itrMethod->isBuiltIn() || ( TREE_CODE( (const tree&)*itrMethod ) != FUNCTION_DECL ) )
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

			CPPModel::ListPtr<CPPModel::FunctionParameter>	parameterList( GetFunctionParameters( ParameterList( DECL_ARGUMENTS( (const tree&)*itrMethod )), dictionaryEntry.Dictionary() ) );

			//	Add the method to our list.  A null DECL_RESULT indicates no return value (such as from a destructor or exception handler).

			if( DECL_RESULT( (const tree&)*itrMethod ) != NULL )
			{
				methodList->push_back( new CPPModel::MethodDeclaration( methodName, itrMethod->uid(), itrMethod->sourceLocation(), isStatic, accessSpec, DeclOrTypeBaseTree::convert( TREE_TYPE( DECL_RESULT( (const tree&)*itrMethod ) ) )->type( dictionaryEntry.Dictionary() ), attributes, parameterList ) );
			}
			else
			{
				methodList->push_back( new CPPModel::MethodDeclaration( methodName, itrMethod->uid(), itrMethod->sourceLocation(), isStatic, accessSpec, std::unique_ptr<const CPPModel::Type>( new CPPModel::FundamentalType( CPPModel::TypeSpecifier::NO_RETURN ) ), attributes, parameterList ) );
			}
		}

		//	Return the list of methods and we are done

		return( CPPModel::MakeConst<CPPModel::MethodDeclaration>( methodList ));
	}



	CPPModel::ConstListPtr<CPPModel::TemplateMethodDeclaration>		GetTemplateMethods( const DictionaryClassEntryImpl&		dictionaryEntry,
																						const CPPModel::ParseOptions&		options )
	{
		CPPModel::ListPtr<CPPModel::TemplateMethodDeclaration>		templateMethodList( new boost::ptr_list<CPPModel::TemplateMethodDeclaration>() );

		//	Iterate over the methods

		MethodList		methods( TYPE_METHODS( dictionaryEntry.getTree() ) );

		for( MethodList::iterator itrMethod = methods.begin(); itrMethod != methods.end(); ++itrMethod )
		{
			//	Don't add artificial or built-in methods, gcc appears to use 'artificial' methods as placeholders it creates in the AST
			//		and built-in methods are added for RTTI and a couple other language features.
			//
			//	Also, we are only interested in template decls in this method

			if( itrMethod->isArtificial() || itrMethod->isBuiltIn() || ( TREE_CODE( (const tree&)*itrMethod ) != TEMPLATE_DECL ) )
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

			CPPModel::ListPtr<CPPModel::TemplateParameter>	parameterList( GetTemplateParameters( (const tree&)*itrMethod, dictionaryEntry.Dictionary() ) );

			//	Add the method to our list.  A null DECL_RESULT indicates no return value (such as from a destructor or exception handler).

			if( DECL_RESULT( (const tree&)*itrMethod ) != NULL )
			{
				templateMethodList->push_back( new CPPModel::TemplateMethodDeclaration( methodName, itrMethod->uid(), itrMethod->sourceLocation(), isStatic, accessSpec, DeclOrTypeBaseTree::convert( TREE_TYPE( DECL_RESULT( (const tree&)*itrMethod ) ) )->type( dictionaryEntry.Dictionary() ), attributes, parameterList ) );
			}
			else
			{
				templateMethodList->push_back( new CPPModel::TemplateMethodDeclaration( methodName, itrMethod->uid(), itrMethod->sourceLocation(), isStatic, accessSpec, std::unique_ptr<const CPPModel::Type>( new CPPModel::FundamentalType( CPPModel::TypeSpecifier::NO_RETURN ) ), attributes, parameterList ) );
			}
		}

		//	Return the list of methods and we are done

		return( CPPModel::MakeConst<CPPModel::TemplateMethodDeclaration>( templateMethodList ));
	}




	CPPModel::GetClassDefinitionResult			DictionaryClassEntryImpl::GetClassDefinition( const CPPModel::ParseOptions&							options ) const
	{
		//	Let's get base classes, friends, fields, methods and attributes

		CPPModel::ConstListPtr<CPPModel::BaseClassIdentifier>			baseClasses( GetBaseClasses( *this, options ));
		CPPModel::ConstListPtr<CPPModel::FriendIdentifier>	 			friends( GetFriends( *this, options ));
		CPPModel::ConstListPtr<CPPModel::FieldDeclaration>	 			fields( GetFields( *this, options ));
		CPPModel::ConstListPtr<CPPModel::MethodDeclaration>	 			methods( GetMethods( *this, options ));
		CPPModel::ConstListPtr<CPPModel::TemplateMethodDeclaration>	 	templateMethods( GetTemplateMethods( *this, options ));

		CPPModel::ConstListPtr<CPPModel::Attribute>					attr( CPPModel::Attributes::deepCopy( attributes() ));

		//	Build the class definition and return a shared pointer to it

		std::unique_ptr<CPPModel::ClassDefinition>		classDef( new CPPModel::ClassDefinition( name(),
																								 uid(),
																								 enclosingNamespace(),
																								 compilerSpecific(),
																								 !CLASSTYPE_DECLARED_CLASS( getTree() ),
																								 attr,
																								 baseClasses,
																								 friends,
																								 fields,
																								 methods,
																								 templateMethods,
																								 sourceLocation() ));

		return( CPPModel::GetClassDefinitionResult( std::move( classDef ) ) );
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

			parameterList->push_back( new CPPModel::FunctionParameter( itrParameter->identifier(), DeclOrTypeBaseTree::convert( TREE_TYPE( (const tree&)itrParameter ) )->type( Dictionary() ), itrParameter->compilerSpecificFlags() ));
		}

		//	Special case for 'noreturn' functions

		std::unique_ptr<const CPPModel::Type>		returnType;

		if( returnTypeSpec() == CPPModel::TypeSpecifier::NO_RETURN )
		{
			returnType.reset( new CPPModel::FundamentalType( CPPModel::TypeSpecifier::NO_RETURN ) );
		}
		else
		{
			returnType = DeclOrTypeBaseTree::convert( TREE_TYPE( DECL_RESULT( getTree() )))->type( Dictionary() );
		}

		//	Build out the function definition

		functionDef.reset( new CPPModel::FunctionDefinition( name(),
															 uid(),
															 enclosingNamespace(),
															 sourceLocation(),
															 isHiddenFriend(),
															 CPPModel::Attributes::deepCopy( attributes() ),
															 std::move( returnType ),
															 CPPModel::MakeConst<CPPModel::FunctionParameter>( parameterList )));

		//	Finished with success

		return( true );
	}


	bool		DictionaryGlobalVarEntryImpl::GetGlobalVarEntry( const CPPModel::ParseOptions&							options,
											   	   	   	   	   	 std::unique_ptr<const CPPModel::GlobalVarEntry>&		globalVarEntry ) const
	{
		globalVarEntry.reset( new CPPModel::GlobalVarEntry( name(),
															uid(),
															enclosingNamespace(),
															sourceLocation(),
															isStatic(),
															attributes(),
															DeclOrTypeBaseTree::convert( TREE_TYPE( getTree() ))->type( Dictionary() ) ));

		return( true );
	}





	ASTDictionaryImpl::ASTDictionaryImpl()
	{
		//	Create the global namespace

		CPPModel::Namespace*	globalNamespace = new GlobalNamespaceImpl();

		AddNamespace( globalNamespace );
	}



/*
	void	ASTDictionaryImpl::FixupNamespaceTree()
	{
		//	Start by building out the namespace map

		std::function<void (const tree&, const CPPModel::Namespace&)> 		recurseOnNamespace = [&]( const tree&						namespaceNode,
																		  	  	  	  	  	  	  	  const CPPModel::Namespace&		parentNamespace )
		{
			NamespaceTree					fqNamespace( namespaceNode );

			std::string						fqNamespaceString = fqNamespace.asString();

			if( !ContainsNamespace( fqNamespaceString ) )
			{
				AddFQNamespace( fqNamespace );
			}

			NamespaceList	nestedNamespaces = NAMESPACE_LEVEL( namespaceNode )->namespaces;

			const CPPModel::Namespace*			newParentNamespace;

			GetNamespace( fqNamespaceString, newParentNamespace );

			for( NamespaceList::iterator itrNested = nestedNamespaces.begin(); itrNested != nestedNamespaces.end(); ++itrNested )
			{
				recurseOnNamespace( (const tree&)itrNested, *newParentNamespace );
			}
		};

		NamespaceList	nestedNamespaces = NAMESPACE_LEVEL( global_namespace )->namespaces;

		const CPPModel::Namespace*			globalNamespace;

		GetNamespace( CPPModel::SCOPE_RESOLUTION_OPERATOR, globalNamespace );


		for( NamespaceList::iterator itrNested = nestedNamespaces.begin(); itrNested != nestedNamespaces.end(); ++itrNested )
		{
			recurseOnNamespace( (const tree&)itrNested, *globalNamespace );
		}

	}
*/



	void	ASTDictionaryImpl::Build()
	{
		//	Start by building out the namespace map

		std::function<void (const tree&, const CPPModel::Namespace&)> 		recurseOnNamespace = [&]( const tree&						namespaceNode,
																		  	  	  	  	  	  	  	  const CPPModel::Namespace&		parentNamespace )
		{
			NamespaceTree					fqNamespace( namespaceNode );

			std::string						fqNamespaceString = fqNamespace.asString();

			//	Add this namespace first

			AddFQNamespace( fqNamespace );

			//	Next, traverse all the declarations in the tree

			DeclList		declsInNamespace = NAMESPACE_LEVEL( namespaceNode )->names;

			for( DeclList::iterator itrDecl = declsInNamespace.begin(); itrDecl != declsInNamespace.end(); ++itrDecl )
			{
				GCCInternalsTools::DecodeNodeResult			decodedNode = DecodeASTNode( *itrDecl );

				if( decodedNode.Succeeded() )
				{
					Insert( decodedNode.ReturnPtr() );
				}
			}

			//	Recurse over the namespaces

			NamespaceList	nestedNamespaces = NAMESPACE_LEVEL( namespaceNode )->namespaces;

			const CPPModel::Namespace*			newParentNamespace;

			GetNamespace( fqNamespaceString, newParentNamespace );

			for( NamespaceList::iterator itrNested = nestedNamespaces.begin(); itrNested != nestedNamespaces.end(); ++itrNested )
			{
				recurseOnNamespace( (const tree&)itrNested, *newParentNamespace );
			}
		};

		NamespaceList	nestedNamespaces = NAMESPACE_LEVEL( global_namespace )->namespaces;

		const CPPModel::Namespace*			globalNamespace;

		GetNamespace( CPPModel::SCOPE_RESOLUTION_OPERATOR, globalNamespace );


		for( NamespaceList::iterator itrNested = nestedNamespaces.begin(); itrNested != nestedNamespaces.end(); ++itrNested )
		{
			recurseOnNamespace( (const tree&)itrNested, *globalNamespace );
		}

	}



	//	Function to split a fully qualified namespace into an ordered list of individual scopes

	std::list<std::string>		ParseFQNamespace( const std::string& 		fqNamespace )
	{
		std::list<std::string>		individualNamespaces;

		std::string					workingNamespace = fqNamespace;

		for( size_t itrScopeOperator = workingNamespace.find( CPPModel::SCOPE_RESOLUTION_OPERATOR ); ( itrScopeOperator != 0 ) && ( itrScopeOperator != std::string::npos); itrScopeOperator = workingNamespace.find( CPPModel::SCOPE_RESOLUTION_OPERATOR ))
		{
			individualNamespaces.push_back( workingNamespace.substr( 0, itrScopeOperator ) );

			workingNamespace.erase( 0, itrScopeOperator + CPPModel::SCOPE_RESOLUTION_OPERATOR.size() );
		}

		return( individualNamespaces );
	}




	void		ASTDictionaryImpl::AddFQNamespace( const NamespaceTree&			fqNamespace )
	{

		std::string						currentFQNamespaceString;

		const CPPModel::Namespace*		parentNamespace;

		GetNamespace( CPPModel::SCOPE_RESOLUTION_OPERATOR, parentNamespace );

		//	Step through the namespace list from start to finish

		for( const tree& currentNamespace : fqNamespace.nodes() )
		{
			if( currentNamespace == global_namespace )
			{
				continue;
			}

			currentFQNamespaceString += DeclTree( currentNamespace ).identifier() + CPPModel::SCOPE_RESOLUTION_OPERATOR;

			if( !ContainsNamespace( currentFQNamespaceString ) )
			{
				DeclTree		namespaceTree( currentNamespace );

				CPPModel::ConstListPtr<CPPModel::Attribute>			noAttributes( CPPModel::Attributes::emptyList() );

				CPPModel::Namespace*	newNamespace = new NestedNamespaceImpl( namespaceTree.uid(),
																				namespaceTree.identifier(),
																				*parentNamespace,
																				namespaceTree.sourceLocation(),
																				namespaceTree.compilerSpecificFlags(),
																				noAttributes,
																				currentNamespace );

				AddNamespace( newNamespace );
			}

			GetNamespace( currentFQNamespaceString, parentNamespace );
		}
	}



	DecodeNodeResult			ASTDictionaryImpl::DecodeASTNode( const tree&					ASTNode )
	{
		const tree&		treeType = TREE_TYPE( ASTNode );

		if( !treeType )
		{
			return( DecodeNodeResult::Failure( DecodeNodeResultCodes::NULL_NODE, "Node is NULL" ) );
		}

		//	Classes, structs and typedefed flavors of each are RECORD_TYPEs

		if(( TREE_CODE( ASTNode ) == TYPE_DECL ) && ( TREE_CODE( treeType ) == RECORD_TYPE ))
		{
			return( DecodeClass( ASTNode ) );
		}

		//	Unions next

		if(( TREE_CODE( ASTNode ) == TYPE_DECL ) && ( TREE_CODE( treeType ) == UNION_TYPE ))
		{
			return( DecodeUnion( ASTNode ) );
		}

		//	Global Variables

		if( TREE_CODE( ASTNode ) == VAR_DECL )
		{
			return( DecodeGlobalVar( ASTNode ) );
		}

		//	Functions

		if( TREE_CODE( (const tree&)ASTNode ) == FUNCTION_DECL )
		{
			return( DecodeFunction( ASTNode ) );
		}

		return( DecodeNodeResult::Failure( DecodeNodeResultCodes::UNRECOGNIZED_NODE_TYPE, "Unrecognized Node Type" ) );
	}



	DecodeNodeResult	ASTDictionaryImpl::DecodeClass( const tree&					classTree )
	{
		{
			const tree&		treeType = TREE_TYPE( classTree );

			if( !treeType )
			{
				return( DecodeNodeResult::Failure( DecodeNodeResultCodes::NULL_NODE, "Node is NULL" ) );
			}

			//	Classes, structs and typedefed flavors of each are RECORD_TYPEs

			if( !(( TREE_CODE( classTree ) == TYPE_DECL ) && ( TREE_CODE( treeType ) == RECORD_TYPE ) ))
			{
				return( DecodeNodeResult::Failure( DecodeNodeResultCodes::NOT_A_CLASS, "Node is not a class" ) );
			}
		}

		const CPPModel::SourceLocation	sourceLocation = DeclTree( classTree ).sourceLocation();
		const TypeTree					typeDeclared = TypeTree( TREE_TYPE( classTree ));

		std::string						className = typeDeclared.identifier();

		CPPModel::TypeSpecifier	typeSpec = typeDeclared.typeSpecifier();

		if( CPPModel::CPPTypes[(int)typeSpec].classification == CPPModel::TypeInfo::Classification::CLASS )
		{
			if( UIDIdx().find( typeDeclared.uid() ) == UIDIdx().end() )
			{
				std::string				scope = typeDeclared.fullyQualifiedNamespace().asString();

//				if( !dictionary.ContainsNamespace( scope ) )
//				{
//					AddNamespace( scope, dictionary );
//				}

				const CPPModel::Namespace*		namespaceScope;

				GetNamespace( scope, namespaceScope );

				return DecodeNodeResult( std::unique_ptr<CPPModel::DictionaryEntry>(
											new DictionaryClassEntryImpl( *this,
																		  typeDeclared.uid(),
																		  className,
																		  *namespaceScope,
																		  false,
																		  sourceLocation,
																		  DeclTree( classTree ).compilerSpecificFlags(),
																		  GetAttributes( PurposeValueList( TYPE_ATTRIBUTES( (const tree&)typeDeclared ) )),
																		  typeSpec,
																		  typeDeclared )));
			}
		}

		//	We decoded the class, return true

		return( DecodeNodeResult::Failure( DecodeNodeResultCodes::ERROR_DECODING_CLASS, "Error decoding class" ));
	}



	DecodeNodeResult	ASTDictionaryImpl::DecodeUnion( const tree&					unionNode )
	{
		const tree&		treeType = TREE_TYPE( unionNode );

		if( !treeType )
		{
			return( DecodeNodeResult::Failure( DecodeNodeResultCodes::NULL_NODE, "Node is NULL" ) );
		}

		if( !(( TREE_CODE( unionNode ) == TYPE_DECL ) && ( TREE_CODE( treeType ) == UNION_TYPE ) ))
		{
			return( DecodeNodeResult::Failure( DecodeNodeResultCodes::NOT_A_UNION, "Node is not a union" ) );
		}


		DeclTree							unionTree( unionNode );

		CPPModel::TypeSpecifier		typeSpec = unionTree.typeSpecifier();

		const CPPModel::UID					unionUID = unionTree.uid();

		if( CPPModel::CPPTypes[(int)typeSpec].classification == CPPModel::TypeInfo::Classification::UNION )
		{
			if( UIDIdx().find( unionUID ) == UIDIdx().end() )
			{
				std::string				scope = unionTree.fullyQualifiedNamespace().asString();

//				if( !dictionary.ContainsNamespace( scope ) )
//				{
//					AddNamespace( scope, dictionary );
//				}

				const CPPModel::Namespace*		namespaceScope;

				GetNamespace( scope, namespaceScope );

				return DecodeNodeResult( std::unique_ptr<CPPModel::DictionaryEntry>(
											new DictionaryUnionEntryImpl( *this,
																		  unionUID,
																		  unionTree.identifier(),
																		  *namespaceScope,
																		  false,
																		  unionTree.sourceLocation(),
																		  unionTree.compilerSpecificFlags(),
																		  unionTree.treeType().attributes(),
																		  typeSpec,
																		  unionNode )));
			}
		}

		return( DecodeNodeResult::Failure( DecodeNodeResultCodes::ERROR_DECODING_UNION, "Error decoding union" ));
	}



	DecodeNodeResult		ASTDictionaryImpl::DecodeGlobalVar( const tree&					globalVarNode )
	{
		//	Make sure we have a VAR_DECL node

		if( TREE_CODE( globalVarNode ) != VAR_DECL )
		{
			return( DecodeNodeResult::Failure( DecodeNodeResultCodes::NOT_A_GLOBAL_VARIABLE, "Node is not a global variable" ) );
		}

		//	We have a DECL_TREE

		DeclTree			globalVarTree( globalVarNode );

		if( globalVarTree.isArtificial() )
		{
			return( DecodeNodeResult::Failure( DecodeNodeResultCodes::GLOBAL_VARIABLE_IS_ARTIFICIAL, "Global variable is artificial" ) );
		}

		CPPModel::UID			globalVarUID = globalVarTree.uid();

		if( UIDIdx().find( globalVarUID ) == UIDIdx().end() )
		{
			//	Build and insert the dictionary entry

			NamespaceTree			fqNamespace = globalVarTree.fullyQualifiedNamespace();

			std::string				fqNamespaceString = fqNamespace.asString();

			if( !ContainsNamespace( fqNamespaceString ) )
			{
				AddFQNamespace( fqNamespace );
			}

			const CPPModel::Namespace*		namespaceScope;

			GetNamespace( fqNamespaceString, namespaceScope );

			return DecodeNodeResult( std::unique_ptr<CPPModel::DictionaryEntry>(
										new DictionaryGlobalVarEntryImpl( *this,
																		  globalVarUID,
																		  globalVarTree.identifier(),
																		  *namespaceScope,
																		  TREE_STATIC( globalVarNode ) != 0,
																		  DECL_EXTERNAL( globalVarNode ) != 0,
																		  globalVarTree.sourceLocation(),
																		  globalVarTree.compilerSpecificFlags(),
																		  globalVarTree.attributes(),
																		  globalVarTree.typeSpecifier(),
																		  globalVarNode )));
		}

		//	If we are here, something has gone wrong

		return( DecodeNodeResult::Failure( DecodeNodeResultCodes::ERROR_DECODING_GLOBAL_VARIABLE, "Error decoding global variable" ));
	}





	DecodeNodeResult	ASTDictionaryImpl::DecodeFunction( const tree&					functionNode )
	{
		if( !( TREE_CODE( (const tree&)functionNode ) == FUNCTION_DECL ))
		{
			return( DecodeNodeResult::Failure( DecodeNodeResultCodes::NOT_A_FUNCTION, "Node is not a function" ) );
		}

		//	We have a DECL_TREE

		DeclTree			functionTree( functionNode );

		if( functionTree.isArtificial() || functionTree.isBuiltIn() )
		{
			return( DecodeNodeResult::Failure( DecodeNodeResultCodes::ARTIFICIAL_OR_BUILTIN_FUNCTION, "Function is artificial or built-in" ) );
		}

//		if( !options.includeClonedFunctions && DECL_CLONED_FUNCTION_P( (const tree&)currentMethod ) )
//		{
//			continue;
//		}

		CPPModel::TypeSpecifier			typeSpec = functionTree.typeSpecifier();

		const CPPModel::UID				functionUID = functionTree.uid();

		//	 Special case here for function return types.  If the DECL_RESULT for the tree is NULL,
		//		then the function has been declared 'noreturn'.  Set the type spec accordingly.


		tree							declResult = DECL_RESULT( functionNode );

		CPPModel::TypeSpecifier			returnTypeSpec = declResult != NULL ? DeclTree( declResult ).typeSpecifier() : CPPModel::TypeSpecifier::NO_RETURN;


		//	Make sure at a finer grained level that we have a function

		if( CPPModel::CPPTypes[(int)typeSpec].classification == CPPModel::TypeInfo::Classification::FUNCTION )
		{
			if( UIDIdx().find( functionUID ) == UIDIdx().end() )
			{
				NamespaceTree			fqNamespace = functionTree.fullyQualifiedNamespace();

				std::string				fqNamespaceString = fqNamespace.asString();

				if( !ContainsNamespace( fqNamespaceString ) )
				{
					AddFQNamespace( fqNamespace );
				}

				const CPPModel::Namespace*		namespaceScope;

				GetNamespace( functionTree.fullyQualifiedNamespace().asString(), namespaceScope );

				return DecodeNodeResult( std::unique_ptr<CPPModel::DictionaryEntry>(
											new DictionaryFunctionEntryImpl( *this,
																			 functionUID,
																			 functionTree.identifier(),
																			 *namespaceScope,
																			 false,
																			 functionTree.sourceLocation(),
																			 functionTree.compilerSpecificFlags(),
																			 functionTree.attributes(),
																			 returnTypeSpec,
																			 DECL_HIDDEN_FRIEND_P( functionNode ),
																			 functionNode )));
			}
		}

		//	Return success

		return( DecodeNodeResult::Failure( DecodeNodeResultCodes::ERROR_DECODING_FUNCTION, "Error decoding function" ) );
	}








	CPPModel::CreateNamespaceResult		ASTDictionaryImpl::CreateNamespace( const std::string&				namespaceToAdd )
	{
		//	Return an error now if the namespace already exists

		if( ContainsNamespace( namespaceToAdd ) )
		{
			return( CPPModel::CreateNamespaceResult::Failure( CPPModel::CreateNamespaceResultCodes::NAMESPACE_ALREADY_EXISTS, "Namespace Already Exists"  ) );
		}

		//	If this is a nested namespace, get the individual nested elements

		const std::list<std::string>		individualNamespaces( ParseFQNamespace( namespaceToAdd ) );

		//	Create an empty string that we will use to re-build the nested namespace.
		//		Initialize the parent namespace to the global namespace.

		std::string							currentFQNamespace = "";

  		const CPPModel::Namespace*			parentNamespace;

  		GetNamespace( "::", parentNamespace );

		//	Iterate over each namespace nesting level

  		for( std::list<std::string>::const_iterator itrIndividualNamespace = individualNamespaces.begin(); itrIndividualNamespace != individualNamespaces.end(); itrIndividualNamespace++ )
		{
			//	Rebuild the fully qualified name and just loop again if the current fq namespace already exists.  Update the parent namespace accordingly.

  			currentFQNamespace += *itrIndividualNamespace;
			currentFQNamespace += CPPModel::SCOPE_RESOLUTION_OPERATOR;

			if( ContainsNamespace( currentFQNamespace ) )
			{
				GetNamespace( currentFQNamespace, parentNamespace );

				continue;
			}

			//	Pretty simple in GCC to create a new namespace.
			//		Create an identifier (it is not fully qualified), push the namespace then pop it.

			tree 	newNamespaceID = get_identifier( itrIndividualNamespace->c_str() );

			push_namespace( newNamespaceID );

			pop_namespace();

			//	Lookup the new namespace node.

			tree namespaceNode = lookup_name_prefer_type( newNamespaceID, 1 );

			//	Build the namespace object for the CPP model and add it to the dictionary

			DeclTree	namespaceTree( namespaceNode );

			CPPModel::ConstListPtr<CPPModel::Attribute>			noAttributes( CPPModel::Attributes::emptyList() );

			CPPModel::Namespace*	newNamespace = new NestedNamespaceImpl( namespaceTree.uid(),
																			namespaceTree.identifier(),
																			*parentNamespace,
																			namespaceTree.sourceLocation(),
																			namespaceTree.compilerSpecificFlags(),
																			noAttributes,
																			namespaceNode );

			AddNamespace( newNamespace );

			//	Update the parent namespace

			parentNamespace = newNamespace;
		}

		//	If we are down here, all went well so return SUCCESS

  		return( CPPModel::CreateNamespaceResult::Success() );
	}






	CPPModel::CreateGlobalVarResult			ASTDictionaryImpl::CreateGlobalVar( const CPPModel::GlobalVarDeclaration&			globalDecl )
	{

  		switch( globalDecl.kind() )
		{
			case CPPModel::IDeclarationType::Kind::FUNDAMENTAL :
				return( CreateGlobalFundamentalTypeVar( dynamic_cast<const CPPModel::FundamentalGlobalVarDeclarationBase&>( globalDecl ) ));
				break;

			case CPPModel::IDeclarationType::Kind::CLASS :
				return( CreateGlobalClassInstanceVar( dynamic_cast<const CPPModel::ClassGlobalVarDeclaration&>( globalDecl )));
				break;

			case CPPModel::IDeclarationType::Kind::UNION :
				return( CPPModel::CreateGlobalVarResult::Failure( CPPModel::CreateGlobalVarResultCodes::UNRECOGNIZED_TYPE_TO_CREATE, "Unions not yet supported" ) );
				break;
		}

		return( CPPModel::CreateGlobalVarResult::Failure( CPPModel::CreateGlobalVarResultCodes::UNRECOGNIZED_TYPE_TO_CREATE, "Unrecognized type for variable to create" ) );
	}



	static const tree			GetGlobalType( const CPPModel::FundamentalGlobalVarDeclarationBase&			globalDecl )
	{
		tree			globalType;


		switch( globalDecl.modifier() )
		{
		case CPPModel::IDeclarationType::Modifier::VALUE :
			globalType = ASTTreeForType( globalDecl.typeSpecifier() );
			break;

		case CPPModel::IDeclarationType::Modifier::POINTER :
			globalType = build_pointer_type( ASTTreeForType( globalDecl.typeSpecifier() ));
			break;

		case CPPModel::IDeclarationType::Modifier::ARRAY :
			globalType = build_array_type( ASTTreeForType( globalDecl.typeSpecifier() ), build_index_type( build_int_cst( NULL_TREE, globalDecl.arrayDimensions().dimensions() )) );
			break;
		}

		return( globalType );
	}




	AsInitialValueResult			ASTDictionaryImpl::AsInitialValue( const tree								globalType,
																	   const CPPModel::ParameterValueBase&		value )
	{
		tree		initValue;

		if( value.modifier() == CPPModel::ParameterModifier::CONSTANT )
		{
			ConvertParameterValueResult			initialValue = ConvertParameterValue( value );

			if( initialValue.Succeeded() )
			{
				initValue = initialValue.ReturnValue();
			}
			else
			{
				return( AsInitialValueResult::Failure( AsInitialValueResultCodes::ERROR_CONVERTING_TYPE, "Error converting initial value to a tree value.", initialValue ) );
			}
		}
		else if( value.modifier() == CPPModel::ParameterModifier::POINTER )
		{
			const CPPModel::ParameterPointerBase&	pointerInitialValue = dynamic_cast<const CPPModel::ParameterPointerBase&>( value );

			CPPModel::UID 		entryUID = pointerInitialValue.value();

			UIDIndexConstIterator		dictionaryEntry = UIDIdx().find( entryUID );



			if( dictionaryEntry == UIDIdx().end() )
			{
				return( AsInitialValueResult::Failure( AsInitialValueResultCodes::DICTIONARY_ENTRY_NOT_FOUND_FOR_POINTER, "Dictionary entry not found for pointer type parameter" ));
			}

			if( (*dictionaryEntry)->entryKind() != CPPModel::DictionaryEntry::EntryKind::GLOBAL_VAR )
			{
				return( AsInitialValueResult::Failure( AsInitialValueResultCodes::CAN_ONLY_MAKE_POINTER_TO_GLOBAL_VARIABLE, "Dictionary element for pointer must be a global variable" ));
			}

			const DictionaryGlobalVarEntryImpl&			globalVarEntry = dynamic_cast<const DictionaryGlobalVarEntryImpl&>( *(*dictionaryEntry) );

			switch( CPPModel::AsTypeSpecifier( value.type() ))
			{
				case CPPModel::TypeSpecifier::BOOLEAN :
				case CPPModel::TypeSpecifier::CHAR :
				case CPPModel::TypeSpecifier::INT :
				case CPPModel::TypeSpecifier::LONG_INT :
				case CPPModel::TypeSpecifier::FLOAT :
				case CPPModel::TypeSpecifier::DOUBLE :
				case CPPModel::TypeSpecifier::STRING :
					initValue = build1( ADDR_EXPR, globalType, globalVarEntry.getTree() );
				break;

				default :
					return( AsInitialValueResult::Failure( AsInitialValueResultCodes::UNSUPPORTED_TYPE, "Unsupported Type for Initialization Parameter Value Conversion" ));
			}
		}
		else if( value.modifier() == CPPModel::ParameterModifier::ARRAY )
		{
			vec<constructor_elt, va_gc> *v;

			switch( CPPModel::AsTypeSpecifier( value.type() ))
			{
				case CPPModel::TypeSpecifier::BOOLEAN :
				{
					std::vector<bool>	boolArray = dynamic_cast<const CPPModel::BooleanArrayParameter&>( value ).value();

					vec_alloc ( v, boolArray.size() );

					for( unsigned int i = 0; i < boolArray.size(); ++i )
					{
						constructor_elt elt = { build_int_cst( integer_type_node, i ), AsTree( boolArray[i] ) };
						v->quick_push (elt);
					}
				}
				break;

				case CPPModel::TypeSpecifier::CHAR :
				{
					std::vector<char>	charArray = dynamic_cast<const CPPModel::CharArrayParameter&>( value ).value();

					vec_alloc ( v, charArray.size() );

					for( unsigned int i = 0; i < charArray.size(); ++i )
					{
						constructor_elt elt = { build_int_cst( integer_type_node, i ), AsTree( charArray[i] ) };
						v->quick_push (elt);
					}
				}
				break;

				case CPPModel::TypeSpecifier::INT :
				{
					std::vector<int>	intArray = dynamic_cast<const CPPModel::IntArrayParameter&>( value ).value();

					vec_alloc ( v, intArray.size() );

					for( unsigned int i = 0; i < intArray.size(); ++i )
					{
						constructor_elt elt = { build_int_cst( integer_type_node, i ), AsTree( intArray[i] ) };
						v->quick_push (elt);
					}
				}
				break;

				case CPPModel::TypeSpecifier::LONG_INT :
				{
					std::vector<long>	longArray = dynamic_cast<const CPPModel::LongArrayParameter&>( value ).value();

					vec_alloc ( v, longArray.size() );

					for( unsigned int i = 0; i < longArray.size(); ++i )
					{
						constructor_elt elt = { build_int_cst( integer_type_node, i ), AsTree( longArray[i] ) };
						v->quick_push (elt);
					}
				}
				break;

				case CPPModel::TypeSpecifier::FLOAT :
				{
					std::vector<float>	floatArray = dynamic_cast<const CPPModel::FloatArrayParameter&>( value ).value();

					vec_alloc ( v, floatArray.size() );

					for( unsigned int i = 0; i < floatArray.size(); ++i )
					{
						constructor_elt elt = { build_int_cst( integer_type_node, i ), AsTree( floatArray[i] ) };
						v->quick_push (elt);
					}
				}
				break;

				case CPPModel::TypeSpecifier::DOUBLE :
				{
					std::vector<double>	doubleArray = dynamic_cast<const CPPModel::DoubleArrayParameter&>( value ).value();

					vec_alloc ( v, doubleArray.size() );

					for( unsigned int i = 0; i < doubleArray.size(); ++i )
					{
						constructor_elt elt = { build_int_cst( integer_type_node, i ), AsTree( doubleArray[i] ) };
						v->quick_push (elt);
					}
				}
				break;

				case CPPModel::TypeSpecifier::STRING :
				{
					std::vector<std::string>	stringArray = dynamic_cast<const CPPModel::StringArrayParameter&>( value ).value();

					vec_alloc ( v, stringArray.size() );

					for( unsigned int i = 0; i < stringArray.size(); ++i )
					{
						constructor_elt elt = { build_int_cst( integer_type_node, i ), AsTree( stringArray[i] ) };
						v->quick_push (elt);
					}
				}
				break;

				default :
					vec_free( v );
					return( AsInitialValueResult::Failure( AsInitialValueResultCodes::UNSUPPORTED_TYPE, "Unsupported Type for Initialization Parameter Value Conversion" ));
			}

			initValue = build_constructor( globalType, v );

			vec_free( v );
		}

		return( AsInitialValueResult( initValue ));
	}



	CPPModel::CreateGlobalVarResult			ASTDictionaryImpl::CreateGlobalFundamentalTypeVar( const CPPModel::FundamentalGlobalVarDeclarationBase&			globalDecl )
	{
		const tree			globalType = GetGlobalType( globalDecl );

		//	Create the global declaration

		tree globalDeclaration = build_decl( UNKNOWN_LOCATION,
											 VAR_DECL,
											 get_identifier( globalDecl.name().c_str() ),
											 globalType );

		//	Set the static, public, addressable and used flags.  Insure the external flag is false.

		TREE_STATIC( globalDeclaration ) = 1;
		TREE_PUBLIC( globalDeclaration ) = 1;
		TREE_ADDRESSABLE( globalDeclaration ) = 1;
		TREE_USED( globalDeclaration ) = 1;

		DECL_EXTERNAL( globalDeclaration ) = 0;

		//	Set the context for the variable, it will be a namespace scope

		DECL_CONTEXT( globalDeclaration ) = (dynamic_cast<const GCCInternalsTools::DictionaryTreeMixin&>( globalDecl.namespaceScope() )).getTree();

		//	Initialize the variable if we have an initializer

		if( globalDecl.hasInitialValue() )
		{
			const CPPModel::ParameterValueBase&			paramValue = globalDecl.initialValue();

			AsInitialValueResult						initialValue = AsInitialValue( globalType, paramValue );

			if( initialValue.Succeeded() )
			{
				DECL_INITIAL( globalDeclaration ) = initialValue.ReturnValue();
			}
			else
			{
				return( CPPModel::CreateGlobalVarResult::Failure( CPPModel::CreateGlobalVarResultCodes::INTERNAL_ERROR, "Error encountered converting parameter value to a GCC tree.", initialValue ));
			}
		}

		//	This is a global so go to the top level scope, create the gimple code and then return to whatever scope we are currently in

		push_to_top_level();

		layout_decl( globalDeclaration, false );
		rest_of_decl_compilation( globalDeclaration, 1, 0 );

		pop_from_top_level();

		//	Now, add the global to the AST Dictionary

		GCCInternalsTools::DecodeNodeResult			decodedNode = DecodeGlobalVar( globalDeclaration );

		if( decodedNode.Failed() )
		{
			return( CPPModel::CreateGlobalVarResult::Failure( CPPModel::CreateGlobalVarResultCodes::ERROR_ADDING_GLOBAL_TO_DICTIONARY, "Error Adding Fundamental Typed Global Variable to the AST Dictionary." ) );
		}

		//	If we are down here, all went well so return SUCCESS

		const CPPModel::UID									globalUID = decodedNode.ReturnPtr()->uid();

		if( !Insert( decodedNode.ReturnPtr() ) )
		{
			//	Insertion into the dictionary failed - let's assume this is a conflict on the FQName with an extern.

			std::string						fqName = decodedNode.ReturnPtr()->fullyQualifiedName();

			FQNameIndexConstIterator		itrExistingEntry = FQNameIdx().find( decodedNode.ReturnPtr()->fullyQualifiedName() );

			if( itrExistingEntry == FQNameIdx().end() )
			{
				//	No duplicate name - something else bad has happened

				return( CPPModel::CreateGlobalVarResult::Failure( CPPModel::CreateGlobalVarResultCodes::ERROR_ADDING_GLOBAL_TO_DICTIONARY, "Error Adding Fundamental Typed Global Variable to the AST Dictionary.  Insertion failed but no existing FQNamed entry was found." ) );
			}

			//	OK, check to see if the declaration is an extern

			const CPPModel::DictionaryEntry&	existingEntry = *(*itrExistingEntry);

			if( existingEntry.isExtern() )
			{
				deleteEntry( existingEntry.uid() );
			}

			if( !Insert( decodedNode.ReturnPtr() ) )
			{
				return( CPPModel::CreateGlobalVarResult::Failure( CPPModel::CreateGlobalVarResultCodes::ERROR_ADDING_GLOBAL_TO_DICTIONARY, "Error Adding Fundamental Typed Global Variable to the AST Dictionary.  Insertion failed after prior entry deletion." ) );
			}
		}

  		return( CPPModel::CreateGlobalVarResult( globalUID ) );
	}




	bool				EquivalentTypesWithIndex( const tree&						type1,
										 	 	  const CPPModel::ParameterType&	type2,
										 	 	  const int 						paramTypeIndex )
	{
		tree		type1Type = TREE_TYPE( type1 );

		if( type2.at( paramTypeIndex ) == CPPModel::TypeSpecifier::STRING )
		{
			return( ( TypeSpecifier( type1Type ) == CPPModel::TypeSpecifier::POINTER ) && ( TypeSpecifier( TREE_TYPE( type1Type )) == CPPModel::TypeSpecifier::CHAR ));
		}

//			if(( TypeSpecifier( type1Type ) == CPPModel::TypeSpecifier::POINTER ) && ( type2.typeSpecifier() == CPPModel::ParameterTypeSpecifier::POINTER ))
//			{
//				return( EquivalentTypes( TREE_TYPE( type1Type ), dynamic_cast<const CPPModel::ParameterPointer&>( type2 ).value() ) );
//			}

		return( TypeSpecifier( type1Type ) == type2.at( paramTypeIndex ) );
	}


	bool				EquivalentTypes( const tree&						type1,
										 const CPPModel::ParameterType&		type2 )
	{

		return( EquivalentTypesWithIndex( type1, type2, 0 ) );
	}




	enum class FindMethodResultCodes { SUCCESS, NO_METHOD_FOUND };

	typedef SEFUtility::ResultWithReturnValue<FindMethodResultCodes,tree>						FindMethodValueResult;



	FindMethodValueResult			FindMethod( const MethodList&						methods,
												const char*								methodName,
												const CPPModel::ParameterValueList&		parameterValues )
	{
		tree		currentMethod;


		for( MethodList::iterator itrMethod = methods.begin(); itrMethod != methods.end(); ++itrMethod )
		{
			currentMethod = (const tree&)*itrMethod;

			if( strcmp( IDENTIFIER_POINTER( DECL_NAME( currentMethod )), methodName ) == 0 )
			{
				const ParameterList			methodParams( DECL_ARGUMENTS( currentMethod ));

				ParameterList::iterator 	itrMethodParam = methodParams.begin();

				++itrMethodParam;

				if( itrMethodParam == methodParams.end() )
				{
				   if( parameterValues.empty() )
				   {
					   return( FindMethodValueResult( currentMethod ));
				   }
				}
				else
				{
					CPPModel::ParameterValueList::const_iterator itrParamValue = parameterValues.begin();

					for( ; (itrMethodParam != methodParams.end()) && (itrParamValue != parameterValues.end()); ++itrMethodParam, ++itrParamValue )
					{
						if( !EquivalentTypes( (const tree&)itrMethodParam, itrParamValue->type() ))
						{
							break;
						}
					}

					if( (itrMethodParam == methodParams.end()) && (itrParamValue == parameterValues.end()) )
					{
						return( FindMethodValueResult( currentMethod ));
					}
				}
			}
		}

		return( FindMethodValueResult::Failure( FindMethodResultCodes::NO_METHOD_FOUND,  "Method with arguments matching parameters not found." ) );
	}


	CPPModel::CreateGlobalVarResult			ASTDictionaryImpl::CreateGlobalClassInstanceVar( const CPPModel::ClassGlobalVarDeclaration&			globalDecl )
	{
		tree		declType = dynamic_cast<const DictionaryClassEntryImpl&>( globalDecl.classType() ).getTree();

		TREE_USED( declType ) = 1;

		//	Create the global declaration

		tree globalDeclaration = build_decl( UNKNOWN_LOCATION,
											 VAR_DECL,
											 get_identifier( globalDecl.name().c_str() ),
											 declType );

		//	Set the flags

		TREE_STATIC( globalDeclaration ) = 1;
		TREE_PUBLIC( globalDeclaration ) = 1;
		DECL_EXTERNAL( globalDeclaration ) = 0;
		TREE_ADDRESSABLE( globalDeclaration ) = 1;
		TREE_USED( globalDeclaration ) = 1;

		DECL_CONTEXT( globalDeclaration ) = (dynamic_cast<const GCCInternalsTools::DictionaryTreeMixin&>( globalDecl.namespaceScope() )).getTree();

		//	Pop up the to the global context, compile the declaration, then return to the context where we started

		push_to_top_level();

		layout_decl( globalDeclaration, false );
		rest_of_decl_compilation( globalDeclaration, 1, 0 );

		pop_from_top_level();

		//	Find the correct location in the initializer to enter the initializator for any classes or structs with constructors.
		//		We want to go past a sequence of cond, label, cond, label, zero or more calls, then finally a goto.
		//		We will insert before the goto.

		gimple_stmt_iterator		insertionPoint;

		{
			enum gimple_code			requiredCodes[5] = { GIMPLE_COND, GIMPLE_LABEL, GIMPLE_COND, GIMPLE_LABEL, GIMPLE_GOTO };
			int							currentGimpleCodeIndex = 0;

			bool						foundInsertionPoint = false;

			gimple_seq					functionBody = gimple_body ( cfun->decl );
			gimple_stmt_iterator		gsi;

			for( gsi = gsi_start( functionBody ); !gsi_end_p( gsi ); gsi_next( &gsi ) )
			{
//				debug_gimple_stmt( gsi_stmt( gsi ) );

				if( gimple_code( gsi_stmt( gsi ) ) == requiredCodes[currentGimpleCodeIndex] )
				{
					if( ++currentGimpleCodeIndex == 5 )
					{
						insertionPoint = gsi;
						foundInsertionPoint = true;
						break;
					}
				}
			}

			if( !foundInsertionPoint )
			{
				return( CPPModel::CreateGlobalVarResult::Failure( CPPModel::CreateGlobalVarResultCodes::UNABLE_TO_FIND_INITIALIZATION_INSERTION_POINT, "Could not identify global initialization code insertion point" ));
			}
		}

		//	Find the correct constructor based on the parameter types

		FindMethodValueResult		ctorMethod = FindMethod( MethodList( TYPE_METHODS( declType ) ), "__comp_ctor ", globalDecl.initialValues() );

		if( !ctorMethod.Succeeded() )
		{
			return( CPPModel::CreateGlobalVarResult::Failure( CPPModel::CreateGlobalVarResultCodes::UNABLE_TO_FIND_CORRECT_CONSTRUCTOR, "Could not find correct constructor for given parameters.", ctorMethod ));
		}

		TREE_USED( ctorMethod.ReturnValue() ) = 1;			//	Mark the constructor as used just in case it has not bee nreferenced elsewhere in the source code

		//	Build the call to the constructor

		gimple			initializationStatement;

		vec<tree>		callParams;

		callParams.create( globalDecl.initialValues().size() + 1 );

		callParams.quick_push( build_fold_addr_expr( globalDeclaration ) );


		for( const CPPModel::ParameterValueBase&	currentParam : globalDecl.initialValues() )
		{
			ConvertParameterValueResult			initialValue = ConvertParameterValue( currentParam );

			callParams.quick_push( initialValue.ReturnValue() );
		}

		initializationStatement = gimple_build_call_vec( ctorMethod.ReturnValue(), callParams );

//		debug_gimple_stmt( initializationStatement );

		//	Insert the constructor call into the initialization function

		gsi_insert_before( &insertionPoint, initializationStatement, GSI_SAME_STMT );

		//	TODO fix root namespace

		GCCInternalsTools::DecodeNodeResult			decodedNode = DecodeGlobalVar( globalDeclaration );

		if( decodedNode.Failed() )
		{
			return( CPPModel::CreateGlobalVarResult::Failure( CPPModel::CreateGlobalVarResultCodes::INTERNAL_ERROR, "Error adding new global class to dictionary.", decodedNode ) );
		}

		const CPPModel::UID			globalUID = decodedNode.ReturnPtr()->uid();

		Insert( decodedNode.ReturnPtr() );

		//	If we are down here, all went well so return SUCCESS

  		return( CPPModel::CreateGlobalVarResult( globalUID ) );
	}

}
