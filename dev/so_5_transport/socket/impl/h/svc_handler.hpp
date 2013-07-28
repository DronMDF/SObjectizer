/*
	SObjectizer 5 Transport.
*/

/*!
	\file
	\brief ���������� ���������� Svc_Handler.
*/

#if !defined( _SO_5_TRANSPORT__SOCKET__IMPL__SVC_HANDLER_HPP_ )
#define _SO_5_TRANSPORT__SOCKET__IMPL__SVC_HANDLER_HPP_

#include <cassert>

#include <ace/Svc_Handler.h>
#include <ace/SOCK_Stream.h>
#include <ace/INET_Addr.h>

#include <cpp_util_2/h/lexcast.hpp>

#include <so_5_transport/h/ret_code.hpp>
#include <so_5_transport/h/raw_block.hpp>
#include <so_5_transport/h/scattered_block_stream.hpp>

#include <so_5/rt/h/mbox.hpp>
#include <so_5/rt/h/mbox_ref.hpp>

#include <so_5_transport/h/messages.hpp>
#include <so_5_transport/h/channel_params.hpp>

#include <so_5_transport/socket/impl/h/io_timestamps.hpp>

// #include <so_5_transport/impl/h/block_stream_notifiers.hpp>
// #include <so_5_transport/impl/h/generic_controller.hpp>
// #include <so_5_transport/impl/h/helpers.hpp>

namespace so_5_transport
{

namespace socket
{

namespace impl
{

//
// last_problem_t
//

//! ����� ��� �������� ��������� � ��������� ������.
class last_problem_t
{
	public:
		//! ���������� ����� ��������� �� ������
		//! � ������� c_str() ���� ������.
		const char *
		operator () ( const std::string & problem );

		const std::string &
		str() const;

	private:
		std::string m_last_problem;
};


//
// svc_handler_t
//

//! ���������� ACE_Svc_Handler ��� ������ � ������� �����������.
/*!
	�������� �� ����������� ������ � �� ������� �� �������� �����,
	� ����� �� ������ ������ � �����.
*/
class svc_handler_t
	:
		public ACE_Svc_Handler< ACE_SOCK_Stream, ACE_NULL_SYNCH >
{
		//! ��������� �������� ����.
		typedef ACE_Svc_Handler< ACE_SOCK_Stream, ACE_NULL_SYNCH > base_type_t;

	public:
		virtual ~svc_handler_t();

		svc_handler_t();

		//! ���������������� �����������.
		svc_handler_t(
			//! �������, �� ������� ������ �������� �����.
			ACE_Reactor * reactor,
			//! ������������� ������.
			channel_id_t channel_id,
			//! Mbox ������������� ������.
			const so_5::rt::mbox_ref_t & ta_mbox,
			//! ��������� ������.
			const channel_params_t & channel_params );

		//! \name ���������� �������������� �� Svc_Handler �������.
		//! \{

		/*!
			\brief ������� �� �������� ������������ ����������.

			���� ����� ���������� connector-��/acceptor-�� ����� ����, ���
			��������� ������������ ���������� ����� ���������.

			��������� �������������� ����� open() �������� ������:
			 - ���� ����� �������, �� ������������ ���� � �������� � �������
			  ������ (�.�. ��� ����������� ���������� ������ � ������ ���
			  ������).
		*/
		virtual int
		open( void * );

		/*!
			\brief ������� �� �������� ������ ��� �� ��������� �������
				�������� ������.

			���� ����� ���������� connector-��/acceptor-�� ��� ���������
			������� �������� ������. ���� �� ������ close() ��� ����������
			�������.

			���� ����� ����������� ����� ������ ��������� � ���������
			awaiting_completion, �� ��������� ������ ���������� �����������
			� ��������� ������� �������� ������.

			���� ����� ����������� ����� ������ ��������� � ���������
			established, �� ��������� ������ ���������� �����������
			� ������ ������.
		 */
		virtual int
		handle_close(
			ACE_HANDLE handle,
			ACE_Reactor_Mask mask );

		/*!
			\brief ������� �� ���������� ������ � ��������� ������ ������.
		 */
		int
		handle_input( ACE_HANDLE );

		/*!
			\brief ������� �� ���������� ������ � ��������� ������ ������.
		*/
		virtual int
		handle_output( ACE_HANDLE );

		/*!
			\brief ������� �� ������������ ������� �������� �� ���������� ������.

			��������� ����� ��������������� ������ � ����� ����� ���������
			�������� ������.
		*/
		virtual int
		handle_timeout(
			const ACE_Time_Value & current_time,
			const void * );

		/*!
			����� ����� ������ ������������ ��
			controller_close(), ������� ������ �� �� ���� ��������, ���
			����, ����� ���������� svc_handler �� ���� ��������.

			���� ����� ���������� ��-�� controller_close(), ��
			��� svc_handler ������������ ��������� CONNECTION_MUST_BE_DESTROYED
			� ������������ -1 ��� ����������� svc_handler-�.

			� ��������� ������ ������ �� �������� � ������������ 0.
		*/
		virtual int
		handle_exception( ACE_HANDLE fd );

		//! ������������� �������� ������.
		void
		output_notify();

		//! ������������� �������� ������.
		void
		block_input();

		//! ������������� �������� ������.
		void
		unblock_input();

		void
		enforce_input_detection();

		int
		initiate_except();

	private:
		/*!
			������������� ������.
			����������� acceptor_controller-�� ��� connector_controller-��.
		*/
		const channel_id_t m_channel_id;

		//! Mbox ������������� ������.
		/*!
			����� �������� ������ �� ��������� open.
			��� ���������� �������� ����������� ���������� ������.
		*/
		so_5::rt::mbox_ref_t m_ta_mbox;

		/*!
			��������� ������.
			���������������� � ������������.
		*/
		const channel_params_t m_channel_params;

		//! ��������� ������� �������� ������/������.
		io_timestamps_t m_io_timestamps;

		//! ���������� ������.
		channel_controller_ref_t m_controller;

		//! ������ ��� ���������� ������ � ��� ������ ������.
		scattered_block_stream_ref_t m_input_stream;
		scattered_block_stream_ref_t m_output_stream;

		/*!
			������������� ���������� �������, ���������� ��
			ACE_Reactor::schedule_timer.
			��� �������� ������ ��������� ������� ����������
			�� ����� ��������������.
		*/
		long m_timer_id;

		volatile bool m_enforce_input_detection;
		volatile bool m_is_input_blocked;
		volatile bool m_is_output_blocked;

		std::unique_ptr< block_stream_notifiers::flush_notifier_t >
			m_output_flush_notifier;
		std::unique_ptr< block_stream_notifiers::free_blocks_availability_notifier_t >
			m_input_free_blocks_availability_notifier;
		std::unique_ptr< block_stream_notifiers::stream_operation_notifier_t >
			m_stream_operation_notifier;

		//! �������� ��������� ��������� ��������.
		last_problem_t m_last_problem;

		//! ������� ������.
		void
		create_streams();

		//! �������� ������.
		int
		read_channel(
			bool & channel_closed_on_remote_host );

		//! �������� ������.
		int
		write_channel();
};

} /* namespace impl */

} /* namespace socket */

} /* namespace so_5_transport */

#endif
