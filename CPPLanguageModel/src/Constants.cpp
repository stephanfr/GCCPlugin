/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/



#include <string>


#include "Constants.h"


namespace CPPModel
{

	const char*		toString( AccessSpecifier	accessSpec )
	{
		switch( accessSpec )
		{
			case AccessSpecifier::PUBLIC :
				return( "PUBLIC" );
				break;

			case AccessSpecifier::PROTECTED :
				return( "PROTECTED" );
				break;

			case AccessSpecifier::PRIVATE :
				return( "PRIVATE" );
				break;
		}

		return( "PRIVATE" );
	}

}


