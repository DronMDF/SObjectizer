/*
	MBAPI 4
*/

/*!
	\file
	\brief ����� ������������� ��������� mbapi �����.
*/

#if !defined( _MBAPI_4__COMM__A_MBAPI_INCOMING_CHANNEL_HPP_ )
#define _MBAPI_4__COMM__A_MBAPI_INCOMING_CHANNEL_HPP_

#include <memory>

#include <so_5/rt/h/rt.hpp>

#include <so_5_transport/h/a_server_base.hpp>

#include <mbapi_4/defs/h/handshaking_params.hpp>

#include <mbapi_4/h/declspec.hpp>

#include <mbapi_4/comm/h/tables_sync_message.hpp>
#include <mbapi_4/comm/h/transmit_info.hpp>

#include <mbapi_4/h/mbapi_layer.hpp>

namespace mbapi_4
{

namespace comm
{

namespace impl
{

class client_channel_data_processor_table_t;

} /* namespace impl */


//
// a_mbapi_incoming_channel_t
//

//! ����� ������������� ��������� mbapi �����.
class MBAPI_4_TYPE a_mbapi_incoming_channel_t
	:
		public so_5_transport::a_server_base_t
{
		typedef so_5_transport::a_server_base_t base_type_t;

	public:
		a_mbapi_incoming_channel_t(
			so_5::rt::so_environment_t & env,
			const so_5::rt::mbox_ref_t & notification_mbox,
			const handshaking_params_t & handshaking_params );
		virtual ~a_mbapi_incoming_channel_t();

		virtual void
		so_evt_start();

		virtual void
		so_evt_finish();

		//! ��������� �������� ���������
		void
		evt_transmit(
			const so_5::rt::event_data_t< transmit_info_t > & msg );

		//! ��������� ������������� � �������������� ��������.
		void
		evt_sync(
			const so_5::rt::event_data_t< tables_sync_message_t > & msg );

	private:
		//! ������� �� ����������� �������.
		virtual void
		so_handle_client_connected(
			const so_5_transport::msg_channel_created & msg );

		//! ������� �� ����������� ����������� �������.
		virtual void
		so_handle_client_failed(
			const so_5_transport::msg_channel_failed & msg );

		//! ������� �� ���������� �������.
		virtual void
		so_handle_client_disconnected(
			const so_5_transport::msg_channel_lost & msg );

		//! ������� �� ����������� ������ � �����.
		virtual void
		so_handle_incoming_package(
			const so_5_transport::msg_incoming_package & msg );

		//! ��������� ����� �� ����� ������� �������������
		//! � ��������.
		/*!
			��������� ���� �� ������ ������������� ����������
			������. ���� ��� ������������� �������,
			�� ������� �������. ���� ������� �� ��������,
			� ���� ������������� ������, �� ������� ���� �������.
		*/
		void
		check_sync_necessity();

		//! ��������� ��������� handshake.
		const handshaking_params_t m_handshaking_params;

		//! C��� mbapi_4.
		/*!
			��� ������ �� ����� mbapi_4 ���
			������� � ����� SO.
			��� ������������� ����� ���������� ��������� �� ����.
		*/
		mbapi_4::impl::mbapi_layer_impl_t & m_mbapi_layer;

		//! ���������� ��� ������ � ��������.
		std::unique_ptr< impl::client_channel_data_processor_table_t >
			m_channel_table;

		typedef std::map<
				so_5_transport::channel_id_t,
				channel_uid_wrapper_t >
			chanel_uid_table_t;

		//! ������� ������������� ���������������.
		chanel_uid_table_t m_channel_id_table;

		//! ������ ��� ������� �������������.
		so_5::timer_thread::timer_id_ref_t m_sync_timer_id;
};

} /* namespace comm */

} /* namespace mbapi_4 */

#endif
