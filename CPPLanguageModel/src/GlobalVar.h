/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/



#ifndef GLOBALVAR_H_
#define GLOBALVAR_H_

namespace CPPModel
{


class GlobalVarDefinition : public SourceElement, public Namespace, public Static, public Attributes
	{
	public :

		GlobalVarDefinition() = delete;
		GlobalVarDefinition( GlobalVarDefinition& ) = delete;
		GlobalVarDefinition( const GlobalVarDefinition& ) = delete;

		GlobalVarDefinition( const std::string&					name,
							 const UID&							uid,
							 const std::string&					enclosingNamespace,
							 const SourceLocation&				sourceLocation,
							 bool								isStatic,
							 ConstListPtr<Attribute>			attributes,
							 std::unique_ptr<const Type>		varType )
			: SourceElement( name, uid, sourceLocation ),
			  Namespace( enclosingNamespace ),
			  Static( isStatic ),
			  Attributes( attributes ),
			  m_varType( std::move( varType ) )
		{}


		const Type&			type() const
		{
			return( *m_varType );
		}


		std::ostream&	toXML( std::ostream&			outputStream,
							   int						indentLevel,
							   SerializationOptions		options ) const;

	private :

		const std::unique_ptr<const Type>				m_varType;

	};

}



#endif /* GLOBALVAR_H_ */
