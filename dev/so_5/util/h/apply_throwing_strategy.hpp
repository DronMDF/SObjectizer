/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ������� ��� ����������� ������.
*/

#if !defined( _SO_5__API__IMPL__APPLY_THROWING_STRATEGY_HPP_ )
#define _SO_5__API__IMPL__APPLY_THROWING_STRATEGY_HPP_

#include <string>

#include <so_5/h/ret_code.hpp>
#include <so_5/h/throwing_strategy.hpp>
#include <so_5/h/exception.hpp>

#include <so_5/h/declspec.hpp>

namespace so_5
{

namespace util
{

//! ������� ��� ����������� ������.
/*!
	���� ���� throwing_strategy ��������� ������� ����������, ��
	������ ������� ������������� ����������, � ��������� ������
	������ ������������ ����� ��� ������.
	\return ��� ������.
*/
SO_5_EXPORT_FUNC_SPEC( ret_code_t )
apply_throwing_strategy(
	ret_code_t error_code,
	throwing_strategy_t throwing_strategy,
	const std::string & error_msg );

//! ������� ��� ����������� ������.
/*!
	���� ���� throwing_strategy ��������� ������� ����������, ��
	������ ������� ������������� ���������� ex, � ��������� ������
	������ ������������ ����� ��� ������ error_code.
	\return ��� ������ error_code.
*/
SO_5_EXPORT_FUNC_SPEC( ret_code_t )
apply_throwing_strategy(
	const so_5::exception_t & ex,
	throwing_strategy_t throwing_strategy );

} /* namespace util */

} /* namespace so_5 */

#endif
