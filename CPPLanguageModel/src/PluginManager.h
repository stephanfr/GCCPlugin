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

	class CallbackIfx
	{
	public :

		virtual ~CallbackIfx()
		{}

		virtual void		ASTReady() = 0;

		virtual void		CreateNamespaces() = 0;

		virtual void		InjectCode() = 0;
	};



	class PluginManager
	{
	public :

		virtual ~PluginManager()
		{}



		virtual void				Initialize( const char* 		pluginName,
				   	   	   	   	   	   	   	    CallbackIfx*		callbacks ) = 0;

		virtual ASTDictionary&		GetASTDictionary() = 0;

	};


	extern PluginManager&		GetPluginManager();
}


#endif /* PLUGINMANAGER_H_ */
