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
#include "AttributeParser.h"





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



	CPPModel::ListPtr<CPPModel::FunctionParameter>			GetFunctionParameters( ParameterList						rawParameters,
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


	CPPModel::ListPtr<CPPModel::TemplateParameter>			GetTemplateParameters( const tree&							functionTree,
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




	bool		DictionaryClassEntryImpl::GetClassDefinition( const CPPModel::ParseOptions&							options,
												   	     	  std::unique_ptr<const CPPModel::ClassDefinition>&		classDef ) const
	{
		//	Let's get base classes, friends, fields, methods and attributes

		CPPModel::ConstListPtr<CPPModel::BaseClassIdentifier>			baseClasses( GetBaseClasses( *this, options ));
		CPPModel::ConstListPtr<CPPModel::FriendIdentifier>	 			friends( GetFriends( *this, options ));
		CPPModel::ConstListPtr<CPPModel::FieldDeclaration>	 			fields( GetFields( *this, options ));
		CPPModel::ConstListPtr<CPPModel::MethodDeclaration>	 			methods( GetMethods( *this, options ));
		CPPModel::ConstListPtr<CPPModel::TemplateMethodDeclaration>	 	templateMethods( GetTemplateMethods( *this, options ));

		CPPModel::ConstListPtr<CPPModel::Attribute>					attr( CPPModel::Attributes::deepCopy( attributes() ));

		//	Build the class definition and return a shared pointer to it

		classDef.reset( new CPPModel::ClassDefinition( name(),
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



	void 	ASTDictionaryImpl::DecodingPass( const tree&			namespaceNode,
							  	  	  	  	 ITreeDecoder&			decoder )
	{
		cp_binding_level*		currentLevel = NAMESPACE_LEVEL( namespaceNode );

		DeclList				elements = currentLevel->names;

		for( DeclList::iterator itrDecl = elements.begin(); itrDecl != elements.end(); ++itrDecl )
		{
			decoder.Decode( (tree&)itrDecl, *this );
		}

		NamespaceList	nestedNamespaces = currentLevel->namespaces;

		for( NamespaceList::iterator itrNested = nestedNamespaces.begin(); itrNested != nestedNamespaces.end(); ++itrNested )
		{
			DecodingPass( (tree&)itrNested, decoder );
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

			//	Classes, structs and typedefed flavors of each are RECORD_TYPEs

			if( !(( TREE_CODE( classTree ) == TYPE_DECL ) && ( TREE_CODE( treeType ) == RECORD_TYPE ) ))
			{
				return( false );
			}
		}

		const CPPModel::SourceLocation	sourceLocation = DeclTree( classTree ).sourceLocation();
		const TypeTree					typeDeclared = TypeTree( TREE_TYPE( classTree ));

		std::string						className = typeDeclared.identifier();

		CPPModel::TypeSpecifier	typeSpec = typeDeclared.typeSpecifier();

		if( CPPModel::CPPTypes[(int)typeSpec].classification == CPPModel::TypeInfo::Classification::CLASS )
		{
			if( dictionary.UIDIdx().find( typeDeclared.uid() ) == dictionary.UIDIdx().end() )
			{
				std::string			scope = typeDeclared.enclosingNamespace();

				const CPPModel::Namespace*		namespaceScope;

				dictionary.GetNamespace( scope, namespaceScope );

				dictionary.Insert( new DictionaryClassEntryImpl( dictionary,
																 typeDeclared.uid(),
																 className,
																 *namespaceScope,
																 sourceLocation,
																 DeclTree( classTree ).compilerSpecificFlags(),
																 GetAttributes( PurposeValueList( TYPE_ATTRIBUTES( (const tree&)typeDeclared ) )),
																 typeSpec,
																 typeDeclared ));
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

		CPPModel::TypeSpecifier		typeSpec = unionTree.typeSpecifier();

		const CPPModel::UID					unionUID = unionTree.uid();

		if( CPPModel::CPPTypes[(int)typeSpec].classification == CPPModel::TypeInfo::Classification::UNION )
		{
			if( dictionary.UIDIdx().find( unionUID ) == dictionary.UIDIdx().end() )
			{
				const CPPModel::Namespace*		namespaceScope;

				dictionary.GetNamespace( unionTree.enclosingNamespace(), namespaceScope );

				dictionary.Insert(  new DictionaryUnionEntryImpl( dictionary,
																  unionUID,
																  unionTree.identifier(),
																  *namespaceScope,
																  unionTree.sourceLocation(),
																  unionTree.compilerSpecificFlags(),
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

		CPPModel::TypeSpecifier		typeSpec = functionTree.typeSpecifier();

		const CPPModel::UID					functionUID = functionTree.uid();

		//	 Special case here for function return types.  If the DECL_RESULT for the tree is NULL,
		//		then the function has been declared 'noreturn'.  Set the type spec accordingly.


		tree								declResult = DECL_RESULT( functionNode );

		CPPModel::TypeSpecifier		returnTypeSpec = declResult != NULL ? DeclTree( declResult ).typeSpecifier() : CPPModel::TypeSpecifier::NO_RETURN;


		//	Make sure at a finer grained level that we have a function

		if( CPPModel::CPPTypes[(int)typeSpec].classification == CPPModel::TypeInfo::Classification::FUNCTION )
		{
			if( dictionary.UIDIdx().find( functionUID ) == dictionary.UIDIdx().end() )
			{
				const CPPModel::Namespace*		namespaceScope;

				dictionary.GetNamespace( functionTree.enclosingNamespace(), namespaceScope );

				dictionary.Insert(  new DictionaryFunctionEntryImpl( dictionary,
																	 functionUID,
																	 functionTree.identifier(),
																	 *namespaceScope,
																	 functionTree.sourceLocation(),
																	 functionTree.compilerSpecificFlags(),
																	 functionTree.attributes(),
																	 returnTypeSpec,
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

			const CPPModel::Namespace*		namespaceScope;

			dictionary.GetNamespace( globalVarTree.enclosingNamespace(), namespaceScope );

			dictionary.Insert( new DictionaryGlobalVarEntryImpl( dictionary,
																 globalVarUID,
																 globalVarTree.identifier(),
																 *namespaceScope,
																 TREE_STATIC( globalVarNode ) != 0,
																 globalVarTree.sourceLocation(),
																 globalVarTree.compilerSpecificFlags(),
																 globalVarTree.attributes(),
																 globalVarTree.typeSpecifier(),
																 globalVarNode ));
		}

		//	Finished with success

		return( true );
	}


	void	ASTDictionaryImpl::Build()
	{
		//	Start by building out the namespace map

		std::function<void (const tree&, CPPModel::Namespace&)> 		recurseOnNamespace = [&]( const tree&				namespaceNode,
																		  	  	  	  	  	  	  CPPModel::Namespace&		parentNamespace )
		{
			DeclTree		namespaceTree( namespaceNode );

			CPPModel::ConstListPtr<CPPModel::Attribute>			noAttributes( CPPModel::Attributes::emptyList() );

			CPPModel::Namespace*	currentNamespace = new NestedNamespaceImpl( namespaceTree.uid(),
																				namespaceTree.identifier(),
																				parentNamespace,
																				namespaceTree.sourceLocation(),
																				namespaceTree.compilerSpecificFlags(),
																				noAttributes,
																				namespaceNode );

			AddNamespace( currentNamespace );

			NamespaceList	nestedNamespaces = NAMESPACE_LEVEL( namespaceNode )->namespaces;

			for( NamespaceList::iterator itrNested = nestedNamespaces.begin(); itrNested != nestedNamespaces.end(); ++itrNested )
			{
				recurseOnNamespace( (const tree&)itrNested, *currentNamespace );
			}
		};


		CPPModel::Namespace*	globalNamespace = new GlobalNamespaceImpl();

		AddNamespace( globalNamespace );


		NamespaceList	nestedNamespaces = NAMESPACE_LEVEL( global_namespace )->namespaces;

		for( NamespaceList::iterator itrNested = nestedNamespaces.begin(); itrNested != nestedNamespaces.end(); ++itrNested )
		{
			recurseOnNamespace( (const tree&)itrNested, *globalNamespace );
		}


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



	CPPModel::CreateNamespaceResult			ASTDictionaryImpl::CreateNamespace( const std::string&				namespaceToAdd )
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
		tree	declType;

		switch( globalDecl.typeVariant().which() )
		{
			case 0:
				declType = ASTTreeForType( boost::get<const CPPModel::Type&>( globalDecl.typeVariant() ).typeSpec() );
				break;

			case 1:
				declType = dynamic_cast<const DictionaryClassEntryImpl&>( boost::get<const CPPModel::DictionaryClassEntry&>( globalDecl.typeVariant() )).getTree();
				break;

			case 2:
				declType = dynamic_cast<const DictionaryUnionEntryImpl&>( boost::get<const CPPModel::DictionaryUnionEntry&>( globalDecl.typeVariant() )).getTree();
				break;
		}

		//	Create the global declaration

		tree globalDeclaration = build_decl( UNKNOWN_LOCATION,
											 VAR_DECL,
											 get_identifier( globalDecl.name().c_str() ),
											 declType );

		/* allocate static storage for this variable */
		TREE_STATIC( globalDeclaration ) = true;

		/* static: internal linkage */
		TREE_PUBLIC( globalDeclaration ) = false;

		/* the context of this declaration: namespace scope */

		DECL_CONTEXT( globalDeclaration ) = (dynamic_cast<const GCCInternalsTools::DictionaryTreeMixin&>( globalDecl.namespaceScope() )).getTree();

		/* this declaration is used in its scope */
		TREE_USED( globalDeclaration ) = true;

		/* initialization to 0 */
//		DECL_INITIAL( globalDeclaration ) = build_real_from_int_cst( declType, build_int_cstu(unsigned_type_node, 1));

		layout_decl( globalDeclaration, false );
		rest_of_decl_compilation( globalDeclaration, 1, 0 );

		//	TODO fix root namespace

		GCCInternalsTools::GlobalVarDecoder().Decode( globalDeclaration, *this );

		//	If we are down here, all went well so return SUCCESS

  		return( CPPModel::CreateGlobalVarResult::Success( DeclTree( globalDeclaration ).uid() ));
	}

}
