/*
 * HelperFunctions.h
 *
 *  Created on: Jun 13, 2013
 *      Author: steve
 */

#ifndef HELPERFUNCTIONS_H_
#define HELPERFUNCTIONS_H_


void ForkGCCTestCase( const char*	testDirectoryName,
					  const char*	testCaseName );

void ForkGCCTestCase( const char*	testDirectoryName,
					  const char*	testCaseName,
					  const char*	option1 );

void ForkGCCTestCase( const char*	testDirectoryName,
					  const char*	testCaseName,
					  const char*	option1,
					  const char*	option2 );

#endif /* HELPERFUNCTIONS_H_ */
