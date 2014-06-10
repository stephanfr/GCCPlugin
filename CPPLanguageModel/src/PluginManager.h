/*
 * PluginManager.h
 *
 *  Created on: May 31, 2014
 *      Author: steve
 */

#ifndef PLUGINMANAGER_H_
#define PLUGINMANAGER_H_


#include "ASTDictionary.h"



namespace CPPModel
{
	class PluginManager
	{
	public :

		virtual ~PluginManager()
		{}


		virtual void				Initialize( const char* 		pluginName ) = 0;


		virtual ASTDictionary&		GetASTDictionary() = 0;

	};


	extern PluginManager&		GetPluginManager();
}


#endif /* PLUGINMANAGER_H_ */
