//============================================================================
// Name        : PluginTest.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include <string>


namespace TestJunkNamespace
{

}


namespace TestNamespace
{

	int			g_globalIntVariable;


	class FriendClass
	{
	public :

		long				longValue;
	};


	union [[json::generalized_attribute( "union" )]] TestUnion
	{
		int				intValue;
		double			doubleValue;
		std::string		stringValue;

		FriendClass		classValue;
		FriendClass*	classPtr;
	};


	class [[json::generalized_attribute( "arg1", "arg2", 3 )]] SecondFriendClass
	{
	public :

		[[json::generalized_attribute]]
		long FriendFunction( long	value )
		{
			std::cerr << value << std::endl;

			return( value );
		}

		long				longValue;
	};


	[[json::generalized_attribute( "function" )]]
	long FriendFunction( long	value )
	{
		std::cerr << value << std::endl;

		return( value );
	}


	class SimpleBase
	{
	public :

		std::string			stringValue;
		double				doubleValue;
	};


	class MarkerInterface
	{
	};


	class [[json::json_serialize("arg1")]] ClassWithAttribute : public SimpleBase, public virtual MarkerInterface
	{
	public :

		ClassWithAttribute()
		{
			m_intValue = 5;
		}


		int				intValue() const
		{
			return( m_intValue );
		}

		class InnerClass
		{
			public:

			int		innerClassIntValue;
		};

	private :


		int					m_intValue;

		std::string			m_stringValue;

		double				m_doubleValue [[json::generalized_attribute("double value")]];


		friend class FriendClass;

		friend long FriendFunction( long	value );

		friend long SecondFriendClass::FriendFunction( long	value );

		friend void InternalFriendFunction() {}

		friend void InternalFriendFunctionWithParameter( ClassWithAttribute&	classWithAttr ) {}

		friend class SecondFriendClass;
	};
}




int main()
{

	TestNamespace::ClassWithAttribute		testClass();

	std::cout << "!!!Hello World!!!" << std::endl; // prints !!!Hello World!!!
	return 0;
}


