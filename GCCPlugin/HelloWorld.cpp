

#include <iostream>



namespace TestNamespace
{
	class TestClass
	{
	public :

		int			publicInt;

		int			getPublicInt() const
		{
			return( publicInt );
		}

	protected :

		double		getPrivateDouble() const
		{
			return( privateDouble );
		}


	private :

		double		privateDouble;
	};


	char*		globalString = "This is a global string";

	TestClass	globalTestClassInstance;
}



int main()
{
	std::cout << "!!!Hello World!!!" << std::endl; // prints !!!Hello World!!!

	return 0;
}
