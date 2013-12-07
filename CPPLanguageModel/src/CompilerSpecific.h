/*
 * CompilerSpecific.h
 *
 *  Created on: Jul 13, 2013
 *      Author: steve
 */

#ifndef COMPILERSPECIFIC_H_
#define COMPILERSPECIFIC_H_


#include "Serialization.h"



namespace CPPModel
{

	class CompilerSpecific : public IXMLSerializable
	{
	public:

		CompilerSpecific() = delete;

		CompilerSpecific( bool		builtIn,
						  bool		artificial )
			: m_builtIn( builtIn ),
			  m_artificial( artificial )
		{}

		CompilerSpecific( const CompilerSpecific&	attributesToCopy )
			: m_builtIn( attributesToCopy.m_builtIn ),
			  m_artificial( attributesToCopy.m_artificial )
		{}

		virtual ~CompilerSpecific()
		{}



		bool			isBuiltIn() const
		{
			return( m_builtIn );
		}

		bool			isArtificial() const
		{
			return( m_artificial );
		}


		SerializationOptions		SerializationMask() const
		{
			std::int64_t		mask;

			mask = ( isBuiltIn() ? (std::int64_t)SerializationOptions::NO_BUILT_INS : (std::int64_t)0 ) |
				   ( isArtificial() ? (std::int64_t)SerializationOptions::NO_ARTIFICIALS : (std::int64_t)0 );

			return( (SerializationOptions)mask );
		}



		std::ostream&	toXML( std::ostream&			outputStream,
							   SerializationOptions		options ) const;



	private :

		bool			m_builtIn;
		bool			m_artificial;
	};


	class ICompilerSpecific
	{
	public :

		virtual ~ICompilerSpecific ()
		{};

		virtual const CompilerSpecific&			compilerSpecific() const = 0;
	};

} /* namespace CPPLanguageModel */
#endif /* COMPILERSPECIFIC_H_ */
