/*-------------------------------------------------------------------------------
Copyright (c) 2013 Stephan Friedl.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Public License v3.0
which accompanies this distribution, and is available at
http://www.gnu.org/licenses/gpl.html

Contributors:
    Stephan Friedl
-------------------------------------------------------------------------------*/


#ifndef CONSTANTTREE_H_
#define CONSTANTTREE_H_



namespace GCCInternalsTools
{

	class ConstantTree
	{
	public :

		ConstantTree( const tree&		constantTree )
			: m_tree( constantTree )
		{
			assert( CONSTANT_CLASS_P( constantTree ) );
		}


		const ConstantTree&	operator= ( const tree&		constantTree )
		{
			assert( CONSTANT_CLASS_P( constantTree ) );

			(tree&)m_tree = constantTree;

			return( *this );
		}

		operator const tree&() const
		{
			return( m_tree );
		}

		std::unique_ptr<CPPModel::ConstantValue>			value()
		{
			if( TREE_CODE( m_tree ) == INTEGER_CST )
			{
				long	fullValue = (( TREE_INT_CST_HIGH( m_tree ) <<  HOST_BITS_PER_WIDE_INT ) + TREE_INT_CST_LOW( m_tree ) );

				return( std::unique_ptr<CPPModel::ConstantValue>( new CPPModel::IntegerConstant( fullValue ) ));
			}
			else if( TREE_CODE( m_tree ) == STRING_CST )
			{
				const char*	stringValue = TREE_STRING_POINTER( m_tree );

				return( std::unique_ptr<CPPModel::ConstantValue>( new CPPModel::StringConstant( stringValue ? stringValue : "" )));
			}
			else if( TREE_CODE( m_tree ) == REAL_CST )
			{
				// TODO FIX REAL CONSTANT CONVERSION

				return( std::unique_ptr<CPPModel::ConstantValue>( new CPPModel::RealConstant( 0 ) ));
			}

			return( std::unique_ptr<CPPModel::ConstantValue>( new CPPModel::UnrecognizedConstant() ));
		}


	private :

		const tree&			m_tree;
	};

}


#endif /* CONSTANTTREE_H_ */
