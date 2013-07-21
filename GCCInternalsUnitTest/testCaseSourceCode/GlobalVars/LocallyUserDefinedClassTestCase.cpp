/*
 * NonIntrisicTestCase.cpp
 *
 *  Created on: Jun 16, 2013
 *      Author: steve
 */


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
	static LocalNamespace::LocalClass		globalClass;

	LocalNamespace::LocalClass				globalClassWithoutStaticKeyword;
}



