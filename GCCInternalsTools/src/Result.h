/*
 * Result.h
 *
 *  Created on: Jul 6, 2013
 *      Author: steve
 */

#ifndef RESULT_H_
#define RESULT_H_

namespace GCCInternalsTools
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

		~Result()
		{};



		ResultCode				code() const
		{
			return( m_resultCode );
		}


		const std::string&		message() const
		{
			return( m_message );
		}


		static Result		Success()
		{
			return( Result( ResultCode::SUCCESS, "Success" ) );
		}

		static Result		Failed( ResultCode			failureCode,
									const char*			message )
		{
			return( Result( failureCode, message ) );
		}

	private :

		Result( ResultCode			resultCode,
				const char*			message );



		ResultCode				m_resultCode;
		const std::string		m_message;
	};

} /* namespace GCCInternalsTools */
#endif /* RESULT_H_ */
