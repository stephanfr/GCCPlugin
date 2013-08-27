/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/


#ifndef ATTRIBUTES_H_
#define ATTRIBUTES_H_


#include "TreeList.h"


namespace GCCInternalsTools
{
	//	Forward Declarations for this namespace

	class IdentifierTree;


	std::unique_ptr<const CPPModel::Attribute>			GetAttribute( const IdentifierTree&			identifier,
	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  const PurposeValueList&		arguments );

	CPPModel::ConstListPtr<CPPModel::Attribute>			GetAttributes( const PurposeValueList&		treeList );

}



#endif /* ATTRIBUTES_H_ */
