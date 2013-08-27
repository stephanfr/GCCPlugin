/*
 * Result.h
 *
 *  Created on: Jul 6, 2013
 *      Author: steve
 */

#ifndef RESULT_H_
#define RESULT_H_

namespace CPPModel
{
	enum class ResultCode
	{
		SUCCESS = 1,

		NAMESPACE_ALREADY_EXISTS = 100
	};


	class Result
	{
	public:

		Result() = delete;

		Result( const Result&	resultToCopy )
			: m_resultCode( resultToCopy.m_resultCode ),
			  m_message( resultToCopy.m_message )
		{}

		~Result()
		{};


		Result&		operator=( const Result&	resultToCopy )
		{
			m_resultCode = resultToCopy.m_resultCode;
			m_message = resultToCopy.m_message;

			return( *this );
		}


		ResultCode				code() const
		{
			return( m_resultCode );
		}


		const std::string&		message() const
		{
			return( m_message );
		}

		const bool				isSuccess() const
		{
			return( m_resultCode == ResultCode::SUCCESS );
		}

		const bool				isFailure() const
		{
			return( !isSuccess() );
		}

		static Result			Success()
		{
			return( Result( ResultCode::SUCCESS, "Success" ) );
		}

		static Result			Failed( ResultCode			failureCode,
										const char*			message )
		{
			return( Result( failureCode, message ) );
		}

	private :


		Result( ResultCode		resultCode,
				const char*		message )
			: m_resultCode( resultCode ),
			  m_message( message )
		{}


		ResultCode			m_resultCode;
		std::string			m_message;
	};

} /* namespace CPPModel */
#endif /* RESULT_H_ */
