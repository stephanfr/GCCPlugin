

#include <sys/types.h>
#include <unistd.h>

#include <stdlib.h>

#include <iostream>

#include "../contrib/gtest/gtest.h"



void ForkGCCTestCase( const char*	testDirectoryName,
		  	  	  	  const char*	testCaseName )
{
	  pid_t		childPid = fork();

	  if( childPid == 0 )
	  {
		  execl( "./gcc_wrapper.sh", "./gcc_wrapper.sh", testDirectoryName, testCaseName, (char*)0 );
	  }
	  else
	  {
		  int		status;

		  wait( &status );

		  ASSERT_TRUE( WEXITSTATUS( status ) == 0 );
	  }
}

void ForkGCCTestCase( const char*	testDirectoryName,
		  	  	  	  const char*	testCaseName,
		  	  	  	  const char*	option1 )
{
	  pid_t		childPid = fork();

	  if( childPid == 0 )
	  {
		  execl( "./gcc_wrapper.sh", "./gcc_wrapper.sh", testDirectoryName, testCaseName, option1, (char*)0 );
	  }
	  else
	  {
		  int		status;

		  wait( &status );

		  ASSERT_TRUE( WEXITSTATUS( status ) == 0 );
	  }
}
