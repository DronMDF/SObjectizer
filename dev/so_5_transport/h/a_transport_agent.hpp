/*
	SObjectizer 5 Transport.
*/

/*!
	\file
	\brief ������������ ������: ��������� � ����������.
*/

#if !defined( _SO_5_TRANSPORT__A_TRANSPORT_AGENT_HPP_ )
#define _SO_5_TRANSPORT__A_TRANSPORT_AGENT_HPP_

#include <set>

#include <so_5_transport/h/declspec.hpp>

#include <so_5/rt/h/message.hpp>
#include <so_5/rt/h/agent.hpp>
#include <so_5/rt/h/event_data.hpp>
#include <so_5/rt/h/so_environment.hpp>

#include <so_5_transport/h/messages.hpp>
#include <so_5_transport/h/channel_controller.hpp>

namespace so_5_transport
{

//
// a_transport_agent_t
//

//! ������� ����� ������������� ������.
/*!
	����� ������ ������������ ��� ����������� �������, ����� ������� ����������
	�������� ������, ��� ������������� ����������.
*/
class SO_5_TRANSPORT_TYPE a_transport_agent_t
	:
		public so_5::rt::agent_t
{
		typedef so_5::rt::agent_t base_type_t;

	public:
		/*!
			������� ����������� mbox � mbox ������������.
		*/
		a_transport_agent_t(
			so_5::rt::so_environment_t & env );

		virtual ~a_transport_agent_t();

		//! ����������� ������ �� ������� ������.
		virtual void
		so_define_agent();

		//! ���������� �������� ���� ����������� �������.
		virtual void
		so_evt_finish();

		//! ��������� ��������� ������ �����������.
		void
		evt_channel_created(
			const so_5::rt::event_data_t< msg_channel_created > & msg );

		//! ��������� ������ �����������.
		void
		evt_channel_lost(
			const so_5::rt::event_data_t< msg_channel_lost > & msg );

		//! �������� mbox ��� ����������� � �������� ������.
		/*!
			��� �������� ���������� ������� ������������� ������
			����� ��������� a_client_base_t ��� a_server_base_t
			(������ ��� ������), ������ �������� mbox, �� �������
			�� ����� ������������ ��������� � �������� ������.
		*/
		inline const so_5::rt::mbox_ref_t &
		query_notificator_mbox() const
		{
			return m_self_mbox;
		}

	protected:
		//! �������������� ���������
		//! ������� ������ ������.
		virtual void
		on_lost( const msg_channel_lost & msg );

		//! ��������� ������������� ���������� ����������.
		const so_5::rt::state_t m_failed_state;

		//! ����������� mbox.
		so_5::rt::mbox_ref_t m_self_mbox;

	private:

		//! ��� ��� ������ ������������� ����������.
		typedef std::set< channel_controller_ref_t >
			channel_controllers_set_t;

		//! ������ ������������� ����������.
		channel_controllers_set_t m_controllers;
};

//
// a_server_transport_agent_t
//

//! ��������� ������������ �����.
/*!
	� ����� ��������� ������� -- so_evt_start(), ��������
	���������� ������. ��������� �������� ������� ����������
	� ���� ��������� msg_create_server_socket_result �� mbox ������.
*/
class SO_5_TRANSPORT_TYPE a_server_transport_agent_t
	:
		public a_transport_agent_t
{
		typedef a_transport_agent_t base_type_t;

	public:
		a_server_transport_agent_t(
			so_5::rt::so_environment_t & env,
			acceptor_controller_unique_ptr_t acceptor_controller );

		virtual ~a_server_transport_agent_t();

		//! ������ ������ ���������� ������������� ������.
		/*!
			���������� ������� ���������� ������ �����
			m_acceptor_controller, ������� ���������������� � ������������.
		*/
		virtual void
		so_evt_start();

		//! ������� ���������� ������ ������.
		/*!
			���������� m_acceptor_controller,
			����� �������� ������������ ���������� so_evt_finish().
		*/
		virtual void
		so_evt_finish();

	private:
		//! ���������� ������.
		acceptor_controller_unique_ptr_t m_acceptor_controller;
};

//
// client_reconnect_params_t
//

//! ��������� ��������������� ��� ������� ����� ����������� ������������� ������.
struct SO_5_TRANSPORT_TYPE client_reconnect_params_t
{
	public:
		client_reconnect_params_t(
			//! �������� �� ���������� �����������
			//! � ������ ������ ������.
			unsigned int on_lost_reconnect_timeout = 1000,
			//! �������� �� ���������� �����������
			//! � ������ ���� ���������� ���������� �� �������.
			unsigned int on_failed_reconnect_timeout = 3000,
			//! ��������� �� ��������������� ������?
			bool do_reconnect = true );

		~client_reconnect_params_t();

		//! �������� �� ���������� �����������
		//! � ������ ������ ������.
		unsigned int m_on_lost_reconnect_timeout;

		//! �������� �� ���������� �����������
		//! � ������ ���� ���������� ���������� �� �������.
		unsigned int m_on_failed_reconnect_timeout;

		//! ��������� �� ��������������� ������?
		bool m_do_reconnect;
};

//
// a_client_transport_agent_t
//

//! ���������� ������������ �����.
/*!
	� ����� ��������� ������� -- so_evt_start(), ��������
	���������� ����������� � �������.
	���� ���������� ����������� �� ������� � ���������������
	��������, �� ������ �������� ������������ �������� ������������
	� �������.
*/
class SO_5_TRANSPORT_TYPE a_client_transport_agent_t
	:
		public a_transport_agent_t
{
		typedef a_transport_agent_t base_type_t;

	public:
		a_client_transport_agent_t(
			so_5::rt::so_environment_t & env,
			connector_controller_unique_ptr_t connector_controller,
			const client_reconnect_params_t & client_reconnect_params
				= client_reconnect_params_t() );

		virtual ~a_client_transport_agent_t();

		virtual void
		so_define_agent();

		virtual void
		so_evt_start();

		//! ���������� m_connector_controller,
		//! ����� �������� ������������ ���������� so_evt_finish().
		virtual void
		so_evt_finish();

		//! ��������� ��� ����������� ���������������.
		struct SO_5_TRANSPORT_TYPE msg_reconnect
			:
				public so_5::rt::message_t
		{};

		//! ��������� ��������� � ���������������.
		void
		evt_reconnect(
			const so_5::rt::event_data_t< msg_reconnect > & );

	protected:
		//! ���������� �������������� ���������
		//! ������ ������, ��� ���� ����� �������������
		//! ���������������.
		virtual void
		on_lost( const msg_channel_lost & );

	private:
		//! ����������������.
		void
		do_reconnect();

		//! ��������� ������� �����������.
		bool
		try_to_connect();

		//! ��������� ���������������.
		const client_reconnect_params_t m_client_reconnect_params;

		//! ���������� ������.
		connector_controller_unique_ptr_t m_connector_controller;
};

} /* namespace so_5_transport */

#endif
