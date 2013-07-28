/*
	SObjectizer 5.
*/

/*!
	\file
	\brief �����, ������� ����������, ��� ����������� �� ������.
*/

#if !defined( _SO_5__THROWING_STRATEGY_HPP_ )
#define _SO_5__THROWING_STRATEGY_HPP_

namespace so_5
{

//
// throwing_strategy_t
//

//! �����, ������� ���������, ��� ������ � ������ ������
//! � ������ ��������� ������� ������� SObjrctizer.
enum throwing_strategy_t
{
	//! ������� ���������� � ������ ������������� ������.
	THROW_ON_ERROR,
	//! ��� ������������� ������, �������� � ��� ����� ��� ��������.
	DO_NOT_THROW_ON_ERROR
};

} /* namespace so_5  */

#endif
