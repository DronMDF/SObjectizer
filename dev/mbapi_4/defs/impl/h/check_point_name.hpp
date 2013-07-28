/*
	MBAPI 4.
*/

/*!
	\file
	\brief ������� ��� �������� ����� �������� ���� � ������.
*/

#if !defined( _MBAPI_4__DEFS__IMPL__CHECK_POINT_NAME_HPP_ )
#define _MBAPI_4__DEFS__IMPL__CHECK_POINT_NAME_HPP_

#include <string>

namespace mbapi_4
{

namespace impl
{

//
// check_point_name
//

//! ������� �������� ����� �����.
/*!
	���� ��� ����� ������������� �������,
	�� ������� ������������ ��� ����������,
	� ��������� ������, ������������� ����������.

	��� �������� ����� ����� �������� ������ ��:
	\li ���� ����������� �������� a..zA..Z;
	\li ���� 0..9;
	\li �������� "_-|#$".
*/

const std::string &
check_point_name(
	const std::string & point_name );

} /* namespace impl */

} /* namespace mbapi_4 */

#endif
