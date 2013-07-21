/*
 * Result.cpp
 *
 *  Created on: Jul 6, 2013
 *      Author: steve
 */

#include <string>

#include "Result.h"

namespace GCCInternalsTools
{

	Result::Result( ResultCode		resultCode,
					const char*		message )
		: m_resultCode( resultCode ),
		  m_message( message )
	{}


} /* namespace GCCInternalsTools */
