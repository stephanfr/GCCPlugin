

#define _GLIBCXX_GTHREAD_USE_WEAK 0


#include <iostream>




namespace TestNamespace
{
	extern int		fieldOffsets[];
	extern char*	fieldNames[];



	class SimpleClass
	{
	public:

		bool			booleanValue() const
		{
			return( m_booleanValue );
		}

		char			charValue() const
		{
			return( m_charValue );
		}

		char*			stringValue() const
		{
			return( m_stringValue );
		}

		int				intValue() const
		{
			return( m_intValue );
		}

		long			longValue() const
		{
			return( m_longValue );
		}

		float			floatValue() const
		{
			return( m_floatValue );
		}

		double			doubleValue() const
		{
			return( m_doubleValue );
		}



	private :

		bool			m_booleanValue;
		char			m_charValue;
		char*			m_stringValue;
		int				m_intValue;
		long			m_longValue;
		float			m_floatValue;
		double			m_doubleValue;

	};

}




int main()
{
	TestNamespace::SimpleClass			testInstance;


	void*			testInstancePointer = &testInstance;

	*((bool*)(testInstancePointer + TestNamespace::fieldOffsets[0])) = true;
	*((char*)(testInstancePointer + TestNamespace::fieldOffsets[1])) = 'b';
	*((char**)(testInstancePointer + TestNamespace::fieldOffsets[2])) = "A quick brown fox";
	*((int*)(testInstancePointer + TestNamespace::fieldOffsets[3])) = 12345;
	*((long*)(testInstancePointer + TestNamespace::fieldOffsets[4])) = 67890;
	*((float*)(testInstancePointer + TestNamespace::fieldOffsets[5])) = 1.234;
	*((double*)(testInstancePointer + TestNamespace::fieldOffsets[6])) = 5.678E-100;

	std::cout << TestNamespace::fieldNames[0] << " : " << testInstance.booleanValue() << std::endl;
	std::cout << TestNamespace::fieldNames[1] << " : " << testInstance.charValue() << std::endl;
	std::cout << TestNamespace::fieldNames[2] << " : " << testInstance.stringValue() << std::endl;
	std::cout << TestNamespace::fieldNames[3] << " : " << testInstance.intValue() << std::endl;
	std::cout << TestNamespace::fieldNames[4] << " : " << testInstance.longValue() << std::endl;
	std::cout << TestNamespace::fieldNames[5] << " : " << testInstance.floatValue() << std::endl;
	std::cout << TestNamespace::fieldNames[6] << " : " << testInstance.doubleValue() << std::endl;

}
