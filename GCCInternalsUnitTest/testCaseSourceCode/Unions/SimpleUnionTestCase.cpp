

#define _GLIBCXX_GTHREAD_USE_WEAK 0

#include <string>
#include <iostream>




namespace TestNamespace
{
	union SimpleUnion
	{
		int				m_intValue;
		double			m_doubleValue;

		char*			m_charPointer;
	}
	SimpleUnionInstance;

}



