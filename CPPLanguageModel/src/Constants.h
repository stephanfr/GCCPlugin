/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/


#ifndef CONSTANTS_H_
#define CONSTANTS_H_


namespace CPPModel
{
	enum class AccessSpecifier
	{
		PUBLIC,
		PROTECTED,
		PRIVATE
	};


	const char*		toString( AccessSpecifier	accessSpec );


	struct TypeInfo
	{
		enum class Specifier
		{
			UNRECOGNIZED = 0,
			VOID,
			ENUM,
			CHAR,
			UNSIGNED_CHAR,
			SHORT_INT,
			UNSIGNED_SHORT_INT,
			INT,
			UNSIGNED_INT,
			LONG_INT,
			UNSIGNED_LONG_INT,
			FLOAT,
			DOUBLE,
			LNG_DOUBLE,
			BOOLEAN,
			FUNCTION,
			METHOD,
			CLASS,
			POINTER,
			REFERENCE,
			ARRAY,
			UNION,
			NULL_POINTER		//	NULL_POINTER MUST always be the last entry in the enumeration.
		};

		enum class Classification { FUNDAMENTAL, CLASS, DERIVED, FUNCTION, UNION, UNRECOGNIZED };

		const Specifier			specifier;
		const Classification	classification;
		const std::string		label;
	};

	static const TypeInfo		CPPTypes[(int)TypeInfo::Specifier::NULL_POINTER + 1] =
	{
			{ TypeInfo::Specifier::UNRECOGNIZED, TypeInfo::Classification::UNRECOGNIZED, "unrecognized" },
			{ TypeInfo::Specifier::VOID, TypeInfo::Classification::FUNDAMENTAL, "void" },
			{ TypeInfo::Specifier::ENUM, TypeInfo::Classification::FUNDAMENTAL, "enum" },
			{ TypeInfo::Specifier::CHAR, TypeInfo::Classification::FUNDAMENTAL, "char" },
			{ TypeInfo::Specifier::UNSIGNED_CHAR, TypeInfo::Classification::FUNDAMENTAL, "unsigned char" },
			{ TypeInfo::Specifier::SHORT_INT, TypeInfo::Classification::FUNDAMENTAL, "short int" },
			{ TypeInfo::Specifier::UNSIGNED_SHORT_INT, TypeInfo::Classification::FUNDAMENTAL, "unsigned short int" },
			{ TypeInfo::Specifier::INT, TypeInfo::Classification::FUNDAMENTAL, "int" },
			{ TypeInfo::Specifier::UNSIGNED_INT, TypeInfo::Classification::FUNDAMENTAL, "unsigned int" },
			{ TypeInfo::Specifier::LONG_INT, TypeInfo::Classification::FUNDAMENTAL, "long int" },
			{ TypeInfo::Specifier::UNSIGNED_LONG_INT, TypeInfo::Classification::FUNDAMENTAL, "unsigned long int" },
			{ TypeInfo::Specifier::FLOAT, TypeInfo::Classification::FUNDAMENTAL, "float" },
			{ TypeInfo::Specifier::DOUBLE, TypeInfo::Classification::FUNDAMENTAL, "double" },
			{ TypeInfo::Specifier::LNG_DOUBLE, TypeInfo::Classification::FUNDAMENTAL, "long double" },
			{ TypeInfo::Specifier::BOOLEAN, TypeInfo::Classification::FUNDAMENTAL, "boolean" },
			{ TypeInfo::Specifier::FUNCTION, TypeInfo::Classification::FUNCTION, "function" },
			{ TypeInfo::Specifier::METHOD, TypeInfo::Classification::FUNCTION, "method" },
			{ TypeInfo::Specifier::CLASS, TypeInfo::Classification::CLASS, "class" },
			{ TypeInfo::Specifier::POINTER, TypeInfo::Classification::DERIVED, "pointer" },
			{ TypeInfo::Specifier::REFERENCE, TypeInfo::Classification::DERIVED, "reference" },
			{ TypeInfo::Specifier::ARRAY, TypeInfo::Classification::DERIVED, "array" },
			{ TypeInfo::Specifier::UNION, TypeInfo::Classification::UNION, "union" },
			{ TypeInfo::Specifier::NULL_POINTER, TypeInfo::Classification::FUNDAMENTAL, "null pointer" }
	};

}	//	namespace CPPModel



#endif /* CONSTANTS_H_ */
