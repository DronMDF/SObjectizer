/*
	SObjectizer 5 Transport.
*/

/*!
	\file
	\brief ���������� connector_controller ��� �������.
*/

#if !defined( _SO_5_TRANSPORT__SOCKET__IMPL__CONNECTOR_CONTROLLER_HPP_ )
#define _SO_5_TRANSPORT__SOCKET__IMPL__CONNECTOR_CONTROLLER_HPP_

#include <ace/SOCK_Connector.h>
#include <ace/Connector.h>

#include <so_5_transport/socket/h/pub.hpp>

#include <so_5_transport/socket/impl/h/svc_handler.hpp>


namespace so_5_transport
{

namespace socket
{

namespace impl
{

//
// socket_connector_controller_t
//

/*!
	\brief ���������� connector_controller ��� �������.
*/
class SO_5_TRANSPORT_TYPE socket_connector_controller_t
	:
		public connector_controller_t
{
	private:

		//! ��������� ��� ��������� ������������ ������ �����������.
		connector_params_t m_connector_params;

		//! ��������� ��� ������ ������.
		const channel_params_t m_channel_params;

		//! ������, ������� �������� �� ��������� �������������
		//! ����� ��� SOCK_Stream.
		/*!
			����� ���� ������.
		*/
		option_setter_unique_ptr_t m_option_setter;

		//! ��������� ��� ������������� CONNECTOR-�.
		typedef ACE_Connector<
				svc_handler_t,
				ACE_SOCK_Connector >
			ace_connector_t;

		//! ������-CONNECTOR, ������� ��������� ��������� ����������.
		ace_connector_t m_connector;

	public :
		/*!
			���������������� �����������.
		*/
		socket_connector_controller_t(
			const connector_params_t & connector_params,
			const channel_params_t & channel_params,
			option_setter_unique_ptr_t option_setter,
			ACE_Reactor * reactor );

		//! \name ���������� �������������� �������.
		//! \{
		virtual so_5::ret_code_t
		connect(
			//! Mbox ������������� ������.
			const so_5::rt::mbox_ref_t & ta_mbox );
		//! \}
};

} /* namespace impl */

} /* namespace socket */

} /* namespace so_5_transport */

#endif

