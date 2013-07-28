/*
	SObjectizer 5 Transport.
*/

/*!
	\file
	\brief ������� ����� ������ ����������������� � �������.

	���������� ����������� ������� ������, ����������� ������ �� �������
	� ���������� ��������� hook-�������.
*/

#if !defined( _SO_5_TRANSPORT__A_CHANNEL_BASE_HPP_ )
#define _SO_5_TRANSPORT__A_CHANNEL_BASE_HPP_

#include <set>

#include <so_5/rt/h/agent.hpp>

#include <so_5_transport/h/declspec.hpp>
#include <so_5_transport/h/messages.hpp>

namespace so_5_transport
{

// ��� ������ ������ ���� ��������.
class a_client_base_t;
class a_server_base_t;

//
// a_channel_base_t
//

//!  ������� ����� ���������� ������.
/*!
	������ ������� ������� ��������� �������
	���������� ������ ��������� ����������� � �������� ������,
	� ���������� �� ��������� hook-�������, ������� ������ ���� �������������� �
	�����������.

	������������ �� ������� ������ ��������� �� �������������
	������ ����������� ������ �� ������� ������ � ������� mbox ���
	�����������. ���� ��������� ���������� �������������� ���������
	� ����� �������� ����������� � �������� � �������, �� �����
	�������� ��������� � ������ ��������� ��� �������� �� ������� ������.
*/
class SO_5_TRANSPORT_TYPE a_channel_base_t
	:
		public so_5::rt::agent_t
{
		typedef so_5::rt::agent_t base_type_t;
		friend class a_server_base_t;
		friend class a_client_base_t;

	public:
		a_channel_base_t(
			so_5::rt::so_environment_t & env,
			//! Mbox �� ������� ����� ��������� ����������� � �������� ������.
			const so_5::rt::mbox_ref_t & notification_mbox );
		virtual ~a_channel_base_t();

		//! ���������� �������� ������ �� ������� ������.
		virtual void
		so_define_agent();

		//! ��������� ��������� ������ ����������.
		void
		evt_channel_created(
			const so_5::rt::event_data_t< msg_channel_created > & msg );

		//! ��������� ������ ��������� ������ ����������.
		void
		evt_channel_failed(
			const so_5::rt::event_data_t< msg_channel_failed > & msg );

		//! ��������� ������ ����������.
		void
		evt_channel_lost(
			const so_5::rt::event_data_t< msg_channel_lost > & msg );

		//! ��������� ����������� � �������� ������.
		void
		evt_incoming_package(
			const so_5::rt::event_data_t<
				so_5_transport::msg_incoming_package > & msg );

	protected:
		//! �������� ��������� ��� �������� �� ������� ������.
		/*!
			���� ��������� ���������� �������������� ���������
			� ���� ����� ������� ������ �������������� � ����
			�������������� ����������, �� ����� so_add_state_for_channel_events
			�� ����� �������� � ������ ��������� ��� �������
			�������������� ������� ������. ��������:
			\code
				class my_client_t : public so_5_transport::a_channel_base_t
				{
					typedef so_5_transport::a_channel_base_t base_type_t;

					// ������ ��������� 1.
					const so_5::state_t m_protocol_v1;
					// ������ ��������� 2.
					const so_5::state_t m_protocol_v2;

					public:
						my_client_t(
							so_5::rt::so_environment_t & env,
							const so_5::rt::mbox_ref_t & notification_mbox )
							:
								base_type_t( env )
								m_protocol_v1( self_ptr() ),
								m_protocol_v2( self_ptr() )
						{
							// � �� ����������� �� ���� ����� ������ ���������
							// ����� ������������ ������� ������ ���� ������������.
							so_add_state_for_channel_events( m_protocol_v1 );
							so_add_state_for_channel_events( m_protocol_v2 );
						}

						virtual void
						so_define_agent()
						{
							// �������� �������� ������.
							base_type_t::so_define_agent();

							so_subscribe( query_interface_mbox() )
								.in( m_protocol_v1 )
								.event( &my_client_t::evt_msg_1 );

							so_subscribe( query_interface_mbox() )
								.in( m_protocol_v2 )
								.event( &my_client_t::evt_msg_1 );

							// ...

							so_subscribe( query_interface_mbox() )
								.in( m_protocol_v1 )
								.event( &my_client_t::evt_msg_N );

							so_subscribe( query_interface_mbox() )
								.in( m_protocol_v2 )
								.event( &my_client_t::evt_msg_N );

							// �� ������ ������ ��������� ��������� �����.

							so_subscribe( query_interface_mbox() )
								.in( m_protocol_v2 )
								.event( &my_client_t::evt_ping );

							// ...
						}

						// ...
				};
			\endcode

			\note ���������� ��������� ����� ����� ������
			�� ���� ������� ��� ����� ����� �������� �� ���������.
		*/
		void
		so_add_state_for_channel_events(
			const so_5::rt::state_t & state );

		//! Hook-������ ��������� ������� ������.
		//! \{

		//! ��������� ��������� ������ �������.
		virtual void
		so_handle_client_connected(
			const msg_channel_created & msg ) = 0;

		//! ��������� ��� ��������� ����������� ������ �������.
		virtual void
		so_handle_client_failed(
			const msg_channel_failed & msg ) = 0;

		//! ��������� ���������� �������.
		virtual void
		so_handle_client_disconnected(
			const msg_channel_lost & msg ) = 0;

		//! ��������� ���������� ������ �������.
		virtual void
		so_handle_incoming_package(
			const so_5_transport::msg_incoming_package & msg ) = 0;
		//! \}

		//! �������� mbox �� ������� �������� ����������
		//! � �������� ������.
		inline so_5::rt::mbox_ref_t &
		query_notification_mbox()
		{
			return m_notification_mbox;
		}

	private:
		//! ��� ������ ���������.
		typedef std::set< const so_5::rt::state_t * > state_set_t;

		//! ������ ��������� ��� ������� ����������� ������� ������.
		state_set_t m_states_listeninig_channel_events;

		//! Mbox �� ������� �������� ����������
		//! � �������� ������.
		so_5::rt::mbox_ref_t m_notification_mbox;
};

} /* namespace so_5_transport */

#endif
