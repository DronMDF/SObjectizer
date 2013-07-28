/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ����� ��� ����������� ������ � ������������.
*/

#if !defined( _SO_5__RT__IMPL__DISP_CORE_HPP_ )
#define _SO_5__RT__IMPL__DISP_CORE_HPP_

#include <ace/Thread_Mutex.h>

#include <so_5/h/ret_code.hpp>
#include <so_5/rt/h/so_environment.hpp>
#include <so_5/rt/h/disp.hpp>

#include <so_5/rt/h/event_exception_logger.hpp>
#include <so_5/rt/h/event_exception_handler.hpp>

namespace so_5
{

namespace rt
{

namespace impl
{

//
// disp_core_t
//

//! ����� ��� ����������� ������ � ������������.
class disp_core_t
	:
		public disp_evt_except_handler_t
{
	public:
		disp_core_t(
			so_environment_t & so_environment,
			const named_dispatcher_map_t & named_dispatcher_map,
			event_exception_logger_unique_ptr_t && logger,
			event_exception_handler_unique_ptr_t && handler );

		virtual ~disp_core_t();

		//! �������� ��������� �� ���������.
		dispatcher_t &
		query_default_dispatcher();

		//! �������� ����������� ���������.
		/*!
			\return ���� ��������� � ����� ������ ����,
			�� �������� dispatcher_ref_t, ������� ��������� �� ����,
			� ��������� ������ �������� dispatcher_ref_t �� 0.
		*/
		dispatcher_ref_t
		query_named_dispatcher(
			const std::string & disp_name );

		//! ��������� ������ ���� �����������.
		ret_code_t
		start();

		//! �������� ���� �����������, ��� ���� ��������� ������.
		void
		shutdown();

		//! ����� ���������� ���� �����������.
		void
		wait();

		//! ���������� ������ ����� ����������.
		void
		install_exception_logger(
			event_exception_logger_unique_ptr_t && logger );

		//! ���������� ������ ���������� ����������.
		void
		install_exception_handler(
			event_exception_handler_unique_ptr_t && handler );

		//! ���������� ����������.
		/*!
			�������� ���������� � ������� ��� ����������.
		*/
		virtual event_exception_response_action_unique_ptr_t
		handle_exception(
			//! ������ �� ��������� ���������� ����������.
			const std::exception & event_exception,
			//! ��� ����������, ������� ����������� �����.
			const std::string & coop_name );

	private:
		//! ����� SObjectizer.
		so_environment_t & m_so_environment;

		//!��������� �� ���������.
		dispatcher_unique_ptr_t m_default_dispatcher;

		//! ����������� ����������.
		named_dispatcher_map_t m_named_dispatcher_map;

		//! ����� ��� ��������� ������ ����������.
		ACE_Thread_Mutex m_exception_logger_lock;

		//! ����� ����������.
		event_exception_logger_unique_ptr_t m_event_exception_logger;

		//! ���������� ����������.
		event_exception_handler_unique_ptr_t m_event_exception_handler;
};

} /* namespace impl */

} /* namespace rt */

} /* namespace so_5 */

#endif
