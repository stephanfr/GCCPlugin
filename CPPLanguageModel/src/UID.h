/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/
#ifndef UID_H_
#define UID_H_


namespace CPPModel
{

	class UID
	{
	public :

		enum class UIDType { TYPE, DECLARATION };

		UID() = delete;
		UID( UID& ) = delete;

		explicit UID( long		uid,
					  UIDType	type )
			: m_uid( uid ),
			  m_type( type )
		{}

		UID( const UID&		uidToCopy )
			: m_uid( uidToCopy.m_uid ),
			  m_type( uidToCopy.m_type )
		{}

		virtual ~UID()
		{}


		long			value() const
		{
			return( m_uid );
		}

		UIDType			type() const
		{
			return( m_type );
		}


		bool operator==( const UID&	entryToCompare ) const
		{
			return( m_uid == entryToCompare.m_uid );
		}

		bool operator!=( const UID&	entryToCompare ) const
		{
			return( m_uid != entryToCompare.m_uid );
		}

		bool operator<( const UID&	entryToCompare ) const
		{
			return( m_uid < entryToCompare.m_uid );
		}

	private :

		const long			m_uid;
		const UIDType		m_type;
	};


}	//	namespace CPPModel


#endif /* UID_H_ */
