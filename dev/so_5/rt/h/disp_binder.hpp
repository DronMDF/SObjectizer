/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ��������� �������� ������ � ����������,
	� ��� ����������� ����������.
*/

#if !defined( _SO_5__RT__DISP_BINDER_HPP_ )
#define _SO_5__RT__DISP_BINDER_HPP_

#include <memory>

#include <so_5/h/declspec.hpp>
#include <so_5/rt/h/agent_ref.hpp>
#include <so_5/rt/h/disp.hpp>

namespace so_5
{

namespace rt
{

namespace impl
{

class so_environment_impl_t;

} /* namespace impl */


class so_environment_t;

//
// disp_binder_t
//

//! ����� - ��������� �������� ������ � ����������.
/*!
	����� ����������, � ������� ������ �����, ��������������
	������� ���� ����������� � ����������, ������� ����� ��������
	�� ���������� ������� ������.

	�� ��������� ���������� ����������� ��������������,
	������� ��������� ������� � ����������� ���������
	������� (�� ��������� - 1 ���� �� ����). ��, ���� ������ ���� ���������
	� ������������ ����������, �������� � �������� ������� ��������������
	��������, �� ��� ������ ���������� ����������� ��������� �������������,
	������� �������������� ����� �������� � ���������� �� ��������.
*/
class SO_5_TYPE disp_binder_t
{
	public:
		disp_binder_t();
		virtual ~disp_binder_t();

		//! ������� �������� ������ � ����������.
		virtual void
		bind_agent(
			//! ����� SO � ������� ����������� �����,
			//! ��� ���������� � � ���������� ������� ���������� ��������.
			impl::so_environment_impl_t & env,
			//! ����� �������� ���� ��������� � ����������.
			agent_ref_t & agent_ref ) = 0;

		//! ���������� �� �������� ������ � ����������.
		virtual void
		unbind_agent(
			//! ����� SO � ������� ����������� �����,
			//! ��� ���������� � � ���������� ������� ����� ��������.
			impl::so_environment_impl_t & env,
			//! �����, ������� �������� � ����������.
			agent_ref_t & agent_ref ) = 0;
};

//! ��� ��� ������ ��������� �� disp_binder_t.
typedef std::unique_ptr< disp_binder_t > disp_binder_unique_ptr_t;

//! ��� ��� ������ ��������� �� dispatcher_t.
typedef std::shared_ptr< disp_binder_t >
	disp_binder_ref_t;

//! �������� ������������� �� ���������.
SO_5_EXPORT_FUNC_SPEC( disp_binder_unique_ptr_t )
create_default_disp_binder();

} /* namespace rt */

} /* namespace so_5 */

#endif
