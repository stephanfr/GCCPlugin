/*
 * HelperFunctions.h
 *
 *  Created on: Jun 13, 2013
 *      Author: steve
 */

#ifndef HELPERFUNCTIONS_H_
#define HELPERFUNCTIONS_H_


void ForkGCCTestCase( const char*	gccOptions,
					  const char*	testDirectoryName,
					  const char*	testCaseName );

void ForkGCCTestCase( const char*	gccOptions,
		  	  	  	  const char*	testDirectoryName,
					  const char*	testCaseName,
					  const char*	option1 );

void ForkGCCTestCase( const char*	gccOptions,
		  	  	  	  const char*	testDirectoryName,
					  const char*	testCaseName,
					  const char*	option1,
					  const char*	option2 );

void ForkGCCTestCase( const char*	gccOptions,
		  	  	  	  const char*	testDirectoryName,
					  const char*	testCaseName,
					  const char*	option1,
					  const char*	option2,
					  const char*	option3 );

void ForkGCCTestCase( const char*	gccOptions,
		  	  	  	  const char*	testDirectoryName,
					  const char*	testCaseName,
					  const char*	option1,
					  const char*	option2,
					  const char*	option3,
					  const char*	option4 );

#endif /* HELPERFUNCTIONS_H_ */
