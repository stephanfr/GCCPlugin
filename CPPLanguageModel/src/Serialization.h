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


#include <string>
#include <iosfwd>




namespace CPPModel
{

	enum class SerializationOptions : std::int64_t
		{ NONE = 0,
		  NO_ATTRIBUTES = 1,
		  NO_BUILT_INS = 2,
		  NO_ARTIFICIALS = 4
		};




	bool						MatchOptions( SerializationOptions			firstOption,
							  	  	  	  	  SerializationOptions			secondOption );

	SerializationOptions		AddOption( SerializationOptions				option,
						   	   	   	   	   SerializationOptions				optionToAdd );




	class IXMLSerializable
	{
	public :

		virtual ~IXMLSerializable()
		{}

		virtual std::ostream&	toXML( std::ostream&			outputStream,
									   SerializationOptions		options ) const = 0;
	};

}	//	namespace CPPModel

#endif /* SERIALIZATION_H_ */
