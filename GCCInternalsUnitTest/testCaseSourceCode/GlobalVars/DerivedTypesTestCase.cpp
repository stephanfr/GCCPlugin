/*
 * NonIntrisicTestCase.cpp
 *
 *  Created on: Jun 16, 2013
 *      Author: steve
 */


//	Without the symbol defined below, this example will not compile with gcc 4.8.0
//		I expect that might change in the future.


namespace LocalNamespace
{
	class LocalClass
	{
		public :

		LocalClass()
			: m_integerDataMember( 1 )
		{};

		~LocalClass()
		{};


		int		integerDataMember() const
		{
			return( m_integerDataMember );
		}


		private :

			int			m_integerDataMember;
	};
};


namespace TestNamespace
{
	static LocalNamespace::LocalClass*		ptrGlobalClass;

	LocalNamespace::LocalClass*				ptrGlobalClassWithoutStaticKeyword;

	static long*							ptrGlobalLong;

	long*									ptrGlobalLongWithoutStaticKeyword;

	static float**							hdlGlobalFloat;

	float**									hdlGlobalFloatWithoutStaticKeyword;

	static int								globalInt = 5;
	int&									refGlobalInt = globalInt;
}



