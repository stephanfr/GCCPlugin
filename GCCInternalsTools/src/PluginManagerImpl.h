/*
 * PassManager.h
 *
 *  Created on: May 31, 2014
 *      Author: steve
 */

#ifndef PLUGINMANAGERIMPL_H_
#define PLUGINMANAGERIMPL_H_



#include "PluginManager.h"

#include "GCCInternalsTools.h"
#include "DeclOrTypeBaseTree.h"

#include "plugin.h"
#include "tree-pass.h"



namespace GCCInternalsTools
{

	class PluginManagerImpl : public CPPModel::PluginManager
	{
	public:

		PluginManagerImpl();

		virtual ~PluginManagerImpl();



		void							Initialize( const char* 		pluginName );


		bool							dictionaryBuilt() const
		{
			return( m_ASTBuilt );
		}

		CPPModel::ASTDictionary&		GetASTDictionary()
		{
			return( m_ASTDictionary );
		}



		bool							BuildASTGate( void );
		unsigned int 					BuildASTCallback( void );

		unsigned int 					GlobalDeclarationCallback( void );

	private :


		ASTDictionaryImpl				m_ASTDictionary;

		bool							m_ASTBuilt = false;

		bool 							m_globalsGenerated;


		void							RegisterCallbacks( const char* 		pluginName );


	};

} /* namespace GCCInternalsTools */

#endif /* PLUGINMANAGERIMPL_H_ */
