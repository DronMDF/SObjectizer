/*
	SObjectizer 5 Transport.
*/

/*!
	\file
	\brief ������� ����� ������ ������������ ���� �������.
*/

#if !defined( _SO_5_TRANSPORT__A_SERVER_BASE_HPP_ )
#define _SO_5_TRANSPORT__A_SERVER_BASE_HPP_

#include <so_5_transport/h/declspec.hpp>
#include <so_5_transport/h/messages.hpp>
#include <so_5_transport/h/a_channel_base.hpp>

namespace so_5_transport
{

//
// a_server_base
//

//! ������� ����� ������ ������������ ���� �������.
/*!
	���������� ������ ��������� ����������� � �������� ������,
	� ���������� ��������� � hook-������, ������� ����� ���� �������������� �
	�����������.
*/
class SO_5_TRANSPORT_TYPE a_server_base_t
	:
		public a_channel_base_t
{
		typedef a_channel_base_t base_type_t;

	public:
		a_server_base_t(
			//! ����� � ������� ������������� �����.
			so_5::rt::so_environment_t & env,
			//! Mbox �� ������� �������� ����������� � �������� � �������.
			const so_5::rt::mbox_ref_t & notification_mbox );

		virtual ~a_server_base_t();

		virtual void
		so_define_agent();

		//! ��������� ������� �������� ���������� ������.
		void
		evt_create_server_socket_result(
			const so_5::rt::event_data_t< msg_create_server_socket_result > &
				msg );

	protected:
		//! Hook-����� ��������� ������� �������� ������.
		/*!
			���������� �� ��������� ������ �� ������,
			���� ��������� ���������� ������ ������ �������,
			� �������������� ����������, ���� ���������
			���������� ������ �� �������.
		*/
		virtual void
		so_handle_create_server_socket_result(
			const msg_create_server_socket_result & msg );
};

} /* namespace so_5_transport */

#endif
