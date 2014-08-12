

#include <sys/types.h>
#include <unistd.h>

#include <stdlib.h>

#include <cstdarg>
#include <iostream>

#include <string>

#include "gtest/gtest.h"



static void ForkGCCTestCaseInternal( const char*	gccOptions,
		  	  	  	  	  	  	  	 const char*	testDirectoryName,
		  	  	  	  	  	 	     const char*	testCaseName,
		  	  	  	  	  	 	     int			numOptions,
		  	  	  	  	  	 	     ... )
{
	//	First, pass through the variable argument list.
	//		The list must end with an empty string.

	va_list			arguments;
	char* 			execvArgArray[1024];
	int				argIndex = 0;

	execvArgArray[argIndex++] = (char*)"./gcc_wrapper.sh";
	execvArgArray[argIndex++] = (char*)gccOptions;
	execvArgArray[argIndex++] = (char*)testDirectoryName;
	execvArgArray[argIndex++] = (char*)testCaseName;

	va_start( arguments, numOptions );
	char*	currentArgument = va_arg( arguments, char* );

	for( int i = 0; i < numOptions; i++ )
	{
		execvArgArray[argIndex++] = currentArgument;
		currentArgument = va_arg( arguments, char* );
	}

	execvArgArray[argIndex] = NULL;

	//	Next, launch the child process.  The fork() function creates a new
	//		process identical to the existing process, so effectively we have
	//		two processes executing the code after 'fork'.  If the process id
	//		is zero, then we are in the child process, otherwise we are still
	//		in the parent process.
	//
	//	The child process launches the gcc_wrapper script and the parent process
	//		simply waits for the child to exit.

	pid_t		childPid = fork();

	if( childPid == 0 )
	{
		execv( "./gcc_wrapper.sh", execvArgArray );
	}
	else
	{
		int		status;

		wait( &status );

		ASSERT_TRUE( WEXITSTATUS( status ) == 0 );
	}
}


void ForkGCCTestCase( const char*	gccOptions,
	  	  	  	 	  const char*	testDirectoryName,
		  	  	  	  const char*	testCaseName )
{
	ForkGCCTestCaseInternal( gccOptions, testDirectoryName, testCaseName, 0 );
}


void ForkGCCTestCase( const char*	gccOptions,
	  	  	  	 	  const char*	testDirectoryName,
		  	  	  	  const char*	testCaseName,
		  	  	  	  const char*	option1 )
{
	ForkGCCTestCaseInternal( gccOptions, testDirectoryName, testCaseName, 1, option1 );
}


void ForkGCCTestCase( const char*	gccOptions,
	  	  	  	 	  const char*	testDirectoryName,
		  	  	  	  const char*	testCaseName,
		  	  	  	  const char*	option1,
		  	  	  	  const char*	option2 )
{
	ForkGCCTestCaseInternal( gccOptions, testDirectoryName, testCaseName, 2, option1, option2 );
}


void ForkGCCTestCase( const char*	gccOptions,
	  	  	  	 	  const char*	testDirectoryName,
		  	  	  	  const char*	testCaseName,
		  	  	  	  const char*	option1,
		  	  	  	  const char*	option2,
		  	  	  	  const char*	option3 )
{
	ForkGCCTestCaseInternal( gccOptions, testDirectoryName, testCaseName, 3, option1, option2, option3 );
}


void ForkGCCTestCase( const char*	gccOptions,
	  	  	  	 	  const char*	testDirectoryName,
		  	  	  	  const char*	testCaseName,
		  	  	  	  const char*	option1,
		  	  	  	  const char*	option2,
		  	  	  	  const char*	option3,
		  	  	  	  const char*	option4 )
{
	ForkGCCTestCaseInternal( gccOptions, testDirectoryName, testCaseName, 4, option1, option2, option3, option4 );
}


