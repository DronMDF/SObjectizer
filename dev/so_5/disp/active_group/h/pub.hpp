/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ������� ��� �������� ���������� �������� �����.
*/

#if !defined( _SO_5__DISP__ACTIVE_GROUP__PUB_HPP_ )
#define _SO_5__DISP__ACTIVE_GROUP__PUB_HPP_

#include <string>

#include <so_5/h/declspec.hpp>

#include <so_5/rt/h/disp.hpp>
#include <so_5/rt/h/disp_binder.hpp>

namespace so_5
{

namespace disp
{

namespace active_group
{

//! �������� ����������.
SO_5_EXPORT_FUNC_SPEC( so_5::rt::dispatcher_unique_ptr_t )
create_disp();

//! �������� ������ ��� �������� ������ � ����������.
SO_5_EXPORT_FUNC_SPEC( so_5::rt::disp_binder_unique_ptr_t )
create_disp_binder(
	//! ��� ����������.
	const std::string & disp_name,
	//! ��� ������.
	const std::string & group_name );

} /* namespace active_group */

} /* namespace disp */

} /* namespace so_5 */

#endif
