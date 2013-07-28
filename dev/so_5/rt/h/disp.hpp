/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ��������� ���������� - dispatcher_t.
*/

#if !defined( _SO_5__RT__DISP_HPP_ )
#define _SO_5__RT__DISP_HPP_

#include <memory>
#include <map>
#include <string>

#include <so_5/h/declspec.hpp>
#include <so_5/h/ret_code.hpp>

#include <so_5/rt/h/agent_ref.hpp>
#include <so_5/rt/h/event_exception_handler.hpp>

namespace so_5
{

namespace rt
{

//
// disp_evt_except_handler_t
//

//! ���������� ���������� ������� ������� �� ������ ����������.
class SO_5_TYPE disp_evt_except_handler_t
{
	public:
		virtual ~disp_evt_except_handler_t();

		//! ���������� ����������.
		virtual event_exception_response_action_unique_ptr_t
		handle_exception(
			//! ������ �� ��������� ���������� ����������.
			const std::exception & event_exception,
			//! ��� ����������, ������� ����������� �����.
			const std::string & coop_name ) = 0;
};

//
// dispatcher_t
//

//! ����������� ������� ����� ��� ���� �����������.
/*!
	���������� �������� �� ����� ������� �������.

	����� �������������� ���������� �������, ������ �����
	���������� ������������� ����� ������������� dispatcher_binder_t
	� ������ ����������.

	����� ������� ����� mbox ����������� ���������, �� ������� �� ���������,
	��� ���������� � ������� ������, ������� � ���� ������� ������
	����������� ��� ���������� ���������� ������ � ���� � ��������� �������,
	� ����� ���������� � ������ ����������, ����� ��� �������� ������
	�� ���������� ������ � �������� ���, ��� ������ ������.
	�.�. ����������� ���������, ��� ���� ������������� ��������� �������,
	� �� � ���� ������� ���������� �� ���������� �� ������� ���� (�����).
*/
class SO_5_TYPE dispatcher_t
{
	public:
		dispatcher_t();
		virtual ~dispatcher_t();

		inline dispatcher_t *
		self_ptr()
		{
			return this;
		}

		//! ��������� ���������.
		/*!
			\return 0, ���� ��������� ������� �������,
			� rc_disp_start_failed ���� ��������� ���������
			�� �������.
		*/
		virtual ret_code_t
		start() = 0;

		//! ���� ������ ���������� ��������� ������.
		/*!
			������ ������ ��������������� ������� �������������
			���������� ������ ����������. ����� shutdown ��
			������ ���������� ���������� ������.
		*/
		virtual void
		shutdown() = 0;

		//! ������� ������� ���������� ������ ����������.
		/*!
			������� �� ����� ������ ������ �������������� ������
			����� ������ ���������� ��������� ���������.
		*/
		virtual void
		wait() = 0;

		//! ��������� ������ �� ���������� ������� �������.
		//! �.�. ������������� ����� ������� ������.
		virtual void
		put_event_execution_request(
			//! ����� ������� �������� ���� �������������.
			const agent_ref_t & agent_ref,
			//! ���������� �������,
			//! ������� ������ ��������� � ����� ������.
			unsigned int event_count ) = 0;

		//! ���������� ����� disp_evt_except_handler.
		void
		set_disp_event_exception_handler(
			disp_evt_except_handler_t & disp_evt_except_handler );

		disp_evt_except_handler_t &
		query_disp_evt_except_handler();

	private:
		//! ���������� ���������� ������� ������� �� ������ ����������.
		disp_evt_except_handler_t * m_disp_evt_except_handler;
};

//! ��� ��� ������ ��������� �� dispatcher_t.
typedef std::unique_ptr< dispatcher_t > dispatcher_unique_ptr_t;

//! ��� ��� ������ ��������� �� dispatcher_t.
typedef std::shared_ptr< dispatcher_t >
	dispatcher_ref_t;

//! ��� ����� ��� �������� ����������� �����������.
typedef std::map<
		std::string,
		dispatcher_ref_t >
	named_dispatcher_map_t;

} /* namespace rt */

} /* namespace so_5 */

#endif
