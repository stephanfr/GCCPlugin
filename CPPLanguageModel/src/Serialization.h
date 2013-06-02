/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/
#ifndef SERIALIZATION_H_
#define SERIALIZATION_H_



namespace CPPModel
{

	class XMLIndentTable
	{
	public :

		static const int	XML_INDENT_SIZE = 4;
		static const int	NUM_ENTRIES_IN_TABLE = 64;


		static const std::string&		GetIndent( int		desiredLevel )
		{
			//	Make sure desiredLevel is >= 0 and <= NUM_ENTRIES_IN_TABLE
			//		This caps indents at 64 levels for now - that should be plenty enough.
			//
			//		I had to add the local variable, maxEntries as the compiler was griping about
			//		NUM_ENTRIES_IN_TABLE being an undefined reference

			int		maxEntries = NUM_ENTRIES_IN_TABLE;

			desiredLevel = std::max( std::min( desiredLevel, maxEntries ), 0 );

			return( IndentTable[desiredLevel] );
		}


	private :

		static const std::string	IndentTable[NUM_ENTRIES_IN_TABLE+1];
	};





	class XMLSerializable
	{
	public :


		enum class SerializationOptions : std::int64_t { NONE = 0, NO_ATTRIBUTES = 1 };


		virtual ~XMLSerializable()
		{}



		static bool						matchOptions( SerializationOptions		firstOption,
								  	  	  	  	  	  SerializationOptions		secondOption )
		{
			return( (std::int64_t)firstOption && (std::int64_t)secondOption );
		}

		static SerializationOptions		addOption( SerializationOptions			option,
							   	   	   	   	   	   SerializationOptions			optionToAdd )
		{
			return( (SerializationOptions)((std::int64_t)option | (std::int64_t)optionToAdd) );
		}



		virtual std::ostream&	toXML( std::ostream&			outputStream,
									   int						indentLevel,
									   SerializationOptions		options ) const = 0;
	};

}	//	namespace CPPModel

#endif /* SERIALIZATION_H_ */
