/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/
#ifndef CONSTANTVALUE_H_
#define CONSTANTVALUE_H_


namespace CPPModel
{

//
//	Classes for AST constants
//

	class ConstantValue : public virtual XMLSerializable
	{
	public :

		enum class Kind { STRING, INTEGER, REAL, UNRECOGNIZED };

		virtual ~ConstantValue() {};

		virtual Kind		kind() const = 0;


		static ConstantValue*		Copy( const ConstantValue&		valueToCopy );
	};


	class UnrecognizedConstant : public ConstantValue
	{
	public :

		UnrecognizedConstant()
		{}


		Kind		kind() const
		{
			return( Kind::UNRECOGNIZED );
		}

		std::ostream&	toXML( std::ostream&			outputStream,
							   int						indentLevel,
							   SerializationOptions		options ) const;

	};

	class StringConstant : public ConstantValue
	{
	public :

		StringConstant( const char*			value )
			: m_value( value )
		{}

		StringConstant( const std::string&	value )
			: m_value( value )
		{}


		Kind		kind() const
		{
			return( ConstantValue::Kind::STRING );
		}

		const std::string&	value() const
		{
			return( m_value );
		}


		std::ostream&	toXML( std::ostream&			outputStream,
							   int						indentLevel,
							   SerializationOptions		options ) const;


	private :

		std::string			m_value;
	};

	class IntegerConstant : public ConstantValue
	{
	public :

		IntegerConstant( long		value )
			: m_value( value )
		{}


		Kind		kind() const
		{
			return( ConstantValue::Kind::INTEGER );
		}

		long		value() const
		{
			return( m_value );
		}

		std::ostream&	toXML( std::ostream&			outputStream,
							   int						indentLevel,
							   SerializationOptions		options ) const;


	private :

		long			m_value;
	};

	class RealConstant : public ConstantValue
	{
	public :

		RealConstant( double		value )
			: m_value( value )
		{}


		Kind		kind() const
		{
			return( ConstantValue::Kind::REAL );
		}

		double		value() const
		{
			return( m_value );
		}

		std::ostream&	toXML( std::ostream&			outputStream,
							   int						indentLevel,
							   SerializationOptions		options ) const;


	private :

		double			m_value;
	};

}


#endif /* CONSTANTVALUE_H_ */
