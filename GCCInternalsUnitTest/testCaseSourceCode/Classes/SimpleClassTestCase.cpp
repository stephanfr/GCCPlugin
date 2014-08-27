

#define _GLIBCXX_GTHREAD_USE_WEAK 0



namespace TestNamespace
{
	class SimpleClassWithOnlyDataMembers
	{
	public :

		bool			m_booleanValue;
		char			m_charValue;
		char*			m_stringValue;
		int				m_intValue;
		long			m_longValue;
		float			m_floatValue;
		double			m_doubleValue;

	};


	static SimpleClassWithOnlyDataMembers		g_testInstance;



	class SimpleClassWithOnlyMethods
	{
	public :

		SimpleClassWithOnlyMethods()
		{}


		bool			returnBool()
		{
			return( true );
		}

		char			returnChar()
		{
			return( 'a' );
		}

		char*			returnCharPointer()
		{
			return( "Test String" );
		}

		int				returnInt()
		{
			return( 1 );
		}

		long			returnLong()
		{
			return( 2L );
		}

		float			returnFloat()
		{
			return( 1.0 );
		}

		double			returnDouble()
		{
			return( 2.0 );
		}

	};

}




int main()
{
	TestNamespace::g_testInstance.m_booleanValue = true;
}
