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


	class AttributeSpec
	{
	public :

		enum class RequiredElement { DECL_REQUIRED, TYPE_REQUIRED, FUNCTION_TYPE_REQUIRED, ANY };

		AttributeSpec( const char*			name,
					   int					minNumArguments,
					   int					maxNumArguments,
					   RequiredElement		elementRequired,
					   bool					affectsTypeIdentity )
			: m_name( name ),
			  m_minNumArgs( minNumArguments ),
			  m_maxNumArgs( maxNumArguments ),
			  m_elementRequired( elementRequired ),
			  m_affectsTypeIdentity( affectsTypeIdentity )
		{}



		const std::string&		name() const
		{
			return( m_name );
		}

		int						minNumArguments() const
		{
			return( m_minNumArgs );
		}

		int						maxNumArguments() const
		{
			return( m_maxNumArgs );
		}

		const RequiredElement	elementRequired() const
		{
			return( m_elementRequired );
		}

		bool					affectsTypeIdentity() const
		{
			return( m_affectsTypeIdentity );
		}



	private :

		const std::string			m_name;
		const int					m_minNumArgs;
		const int					m_maxNumArgs;
		const RequiredElement		m_elementRequired;
		const bool					m_affectsTypeIdentity;

	};


	typedef std::list<AttributeSpec>		AttributeSpecList;


	class CallbackIfx
	{
	public :

		virtual ~CallbackIfx()
		{}

		virtual std::unique_ptr<AttributeSpecList>		RegisterAttributes() = 0;

		virtual void									ASTReady() = 0;

		virtual void									CreateNamespaces() = 0;

		virtual void									InjectCode() = 0;
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
