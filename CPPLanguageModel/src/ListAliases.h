/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/


#ifndef LISTALIASES_H_
#define LISTALIASES_H_



namespace CPPModel
{
	template <class T> using ListPtr = std::unique_ptr<boost::ptr_list<T>>;
	template <class T> using ConstListPtr = std::unique_ptr<const boost::ptr_list<T>>;

	template <class T> using ListRef = const boost::ptr_list<T>&;

	template <class T> ConstListPtr<T> MakeConst( ListPtr<T>&	nonConstList ) { return( ConstListPtr<T>( std::move( nonConstList ) ) ); }
}

#endif /* LISTALIASES_H_ */
