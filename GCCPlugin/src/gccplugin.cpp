/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/



#include "config.h"

#include "ASTDictionary.h"
#include "PluginManager.h"

#include "gcc-plugin.h"







int	plugin_is_GPL_compatible;





class Callbacks : public CPPModel::CallbackIfx
{
public :

	Callbacks()
	{}

	virtual ~Callbacks()
	{}


	void		ASTReady()
	{
		std::list<std::string>			namespacesToDump( { "TestNamespace::" } );

		CPPModel::GetPluginManager().GetASTDictionary().DumpASTXMLByNamespaces( std::cerr, namespacesToDump );
	};

	void		CreateNamespaces()
	{
	};

	void		InjectCode()
	{
	};

};



Callbacks		g_pluginCallbacks;





int plugin_init( plugin_name_args*		info,
			 	 plugin_gcc_version*	ver )
{
	std::cerr << "Starting Plugin: "<< info->base_name << std::endl;

	CPPModel::GetPluginManager().Initialize( "HelloWorld Plugin", &g_pluginCallbacks );

	return( 0 );
}


