/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ��������� timer_thread_t.
*/

#if !defined( _SO_5__TIMER_THREAD__TIMER_THREAD_HPP_ )
#define _SO_5__TIMER_THREAD__TIMER_THREAD_HPP_

#include <memory>

#include <so_5/h/declspec.hpp>
#include <so_5/h/ret_code.hpp>

#include <so_5/timer_thread/h/timer_act.hpp>
#include <so_5/timer_thread/h/timer_id.hpp>

namespace so_5
{

namespace rt
{

class mbox_ref_t;
class message_ref_t;

} /* namespace rt */

namespace timer_thread
{

//
// timer_thread_t
//

//! ��������� ���� �������.
/*!
	���������� run-time SObjectizer-� ���������� ��������
	��� ������ � ����������� � �������������� �����������.
	�������� ������� ������� ��� ��������� ����� �����
	������ �����������. ��� ��������� ����� ����������
	�������� �� �����- ��� �����������,
	��� ��������� ���������� �������� � ��������.
	��������� ��������������� ��� ������� ������� �����������
	�� ������ ����.

	��� �������� ������� ��� SObjectizer-� ������ ����
	������������ �� timer_thread_t.

	��������� ������ �� ����������� ������ ���� ����������
	� ���� ����. �������� timer_thread_t ��������� �����������.
	�����, ��� ��������� ��������� ������ ��� ����� ������
	����������� ������ timer_thread_t::start() � �������������
	������ ��� ���������� ������ �����������
	timer_thread_t::shutdown(). �.�. ��� ���������� �������
	� ���� ��������� ���� ������ ����������, ����� ����� ��
	shutdown() ������� ���������� ������ ���� �������, ��
	������ ����� wait(). ��������� �������� wait() �����
	����� ������ shutdown() � ������� �� wait() ��������, ���
	������ ��������� ���������� � ��� �������, �����������
	��������, �����������.

	����� � ���������� ���������� ����� push_delayed_msg
	��������� �������� ��������� �������. ����� ������ ��������
	�� �������� ���������� ��������� � �� ����������� �������
	����� ��������� ������ ���� ������������������.
*/
class SO_5_TYPE timer_thread_t
{
	public:

		timer_thread_t();
		virtual ~timer_thread_t();

		//! ��������� ���� �������.
		virtual ret_code_t
		start() = 0;

		//! ���� ������ ���� ������� ��������� ������.
		virtual void
		shutdown() = 0;

		//! ������� ������� ���������� ������ ���� �������.
		virtual void
		wait() = 0;

		//! ��������� ���������� ��� �������������
		//! ��������� � �������.
		virtual timer_id_t
		schedule_act(
			timer_act_unique_ptr_t & timer_act ) = 0;

		//! �������� ������������� ���������.
		virtual void
		cancel_act(
			timer_id_t msg_id ) = 0;
};

//! ��������� unique_ptr ��� timer_thread_t.
typedef std::unique_ptr< timer_thread_t > timer_thread_unique_ptr_t;

} /* namespace timer_thread */

} /* namespace so_5 */

#endif
